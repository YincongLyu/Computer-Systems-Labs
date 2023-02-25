/* 
 * tsh - A tiny shell program with job control
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
 
/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */
 
/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */
 
/* 
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */
 
/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */
 
struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */
 
/* Function prototypes */
 
/* Here are the functions that you will implement */
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);
 
void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);
 
/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv); 
void sigquit_handler(int sig);
 
void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs); 
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid); 
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid); 
int pid2jid(pid_t pid); 
void listjobs(struct job_t *jobs);
 
void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
 
/*
 * main - The shell's main routine 
 */
int main(int argc, char **argv) 
{
    char c;
    char cmdline[MAXLINE];
    int emit_prompt = 1; /* emit prompt (default) */
 
    /* Redirect stderr to stdout (so that driver will get all output
     * on the pipe connected to stdout) */
    dup2(1, 2);
 
    /* Parse the command line */
    while ((c = getopt(argc, argv, "hvp")) != EOF) {
        switch (c) {
        case 'h':             /* print help message */
            usage();
	    break;
        case 'v':             /* emit additional diagnostic info */
            verbose = 1;
	    break;
        case 'p':             /* don't print a prompt */
            emit_prompt = 0;  /* handy for automatic testing */
	    break;
	default:
            usage();
	}
    }
 
    /* Install the signal handlers */
 
    /* These are the ones you will need to implement */
    Signal(SIGINT,  sigint_handler);   /* ctrl-c */
    Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
    Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */
 
    /* This one provides a clean way to kill the shell */
    Signal(SIGQUIT, sigquit_handler); 
 
    /* Initialize the job list */
    initjobs(jobs);
 
    /* Execute the shell's read/eval loop */
    while (1) {
 
	/* Read command line */
	if (emit_prompt) {
	    printf("%s", prompt);
	    fflush(stdout);
	}
	if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
	    app_error("fgets error");
	if (feof(stdin)) { /* End of file (ctrl-d) */
	    fflush(stdout);
	    exit(0);
	}
 
	/* Evaluate the command line */
	eval(cmdline);
	fflush(stdout);
	fflush(stdout);
    } 
 
    exit(0); /* control never reaches here */
}
  
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
/*eval-评估用户刚刚输入的命令行
   如果用户已请求内置命令（退出，作业，bg或fg），则立即执行该命令。 
   否则，请派生一个子进程并在该子进程的上下文中运行该作业。 
   如果作业在前台运行，请等待其终止然后返回。 
   注意：每个子进程必须具有唯一的进程组ID，
   以便当我们在键盘上键入ctrl-c（ctrl-z）时，
   我们的后台子进程不会从内核接收SIGINT（SIGTSTP）。
*/
void eval(char *cmdline) 
{
    /*初始化各变量以及信号阻塞集合*/
    char *argv[MAXARGS];//execve ()的参数列表
    int bf;//job是在 bg 还是 fg 进行？
    pid_t pid;//储存当前前台pid
    sigset_t mask_all, mask_one, prev_one;
 
    bf=parseline(cmdline,argv);//bg:1  fg:0
 
    //若命令行为空
    if (argv[0] == NULL)
        return;
    sigfillset(&mask_all); //把每个信号都添加到mask_all集合中
    sigemptyset(&mask_one); //初始化mask_one集合为空集合
    sigaddset(&mask_one, SIGCHLD);//把SIGCHLD添加进mask_one中
 
    //控制SIGCHID handler何时执行
    sigprocmask(SIG_BLOCK, &mask_one, &prev_one);//阻塞SIGCHLD
    if(!builtin_cmd(argv)){//如果不是内置命令
        //区分子进程和父进程
        if((pid=fork())==0){//子进程
            sigprocmask(SIG_SETMASK, &prev_one, NULL);//取消对SIGCHLD的阻塞
            setpgid(0,0);//创建一个新的进程组，其进程组ID与当前子进程pid一样，并将当前子进程加入该进程组
            if(execve(argv[0],argv,environ)<0){
                printf("%s: Command not found\n",argv[0]);
                exit(0);
            }
        }
 
        //父进程
        sigprocmask(SIG_BLOCK, &mask_all, NULL); //阻塞所有信号
        addjob(jobs, pid, bf+1, cmdline); //把子进程加入作业列表 fg:1 bg:2
        sigprocmask(SIG_SETMASK, &prev_one, NULL);//取消对所有信号的阻塞，当前阻塞的信号集合为SIGCHLD
 
        if(!bf){//如果是前台作业，则父进程需要等待其终止
            waitfg(pid);//阻塞到前台作业变成后台/被回收为止
        }else{
            printf("[%d] (%d) %s",pid2jid(pid),pid,cmdline);//找到pid对应的jid,按照给定格式输出
        }
    }
    return;
}
 
/* 
 * parseline - Parse the command line and build the argv array.
 * 
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.  
 */
int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */
 
    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;
 
    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }
 
    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;
 
	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;
 
    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}
 
/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
 /*内置cmd：识别并解释内置命令：quit，fg，bg和jobs。*/
int builtin_cmd(char **argv) 
{
    if(!strcmp(argv[0],"quit")) exit(0);//quit命令
    else if(!strcmp(argv[0],"&")) return 1;//&表示的后台
    else if(!strcmp(argv[0],"fg")||!strcmp(argv[0],"bg")){//以fg,bg表示的前后台
        do_bgfg(argv);
        return 1;
    }
    else if(!strcmp(argv[0],"jobs")){//处理jobs
        listjobs(jobs);
        return 1;
    }
    return 0;     /* not a builtin command */
}
 
/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
//bg <job>命令通过向其发送SIGCONT信号重新启动<job>，然后在后台运行它。 <job>参数可以是PID或JID。
//fg <job>命令通过向其发送SIGCONT信号来重新启动<job>，然后在前台运行它。 <job>参数可以是PID或JID。
void do_bgfg(char **argv) 
{
    struct job_t *j;
    int id;
 
    if(argv[1] == NULL){//1:后面不跟任何东西
    	printf("%s command requires PID or %%jobid argument\n", argv[0]);
    	return;
    }
    if(sscanf(argv[1], "%%%d", &id) > 0){//4：后面为%数字，看看是不是作业jid
        j = getjobjid(jobs, id);
        
        if(j != NULL){
            kill(-(j->pid),SIGCONT);//发送SIGCONT给进程组|pid|里每个进程,重新启动<job>
            if(strcmp(argv[0],"bg")==0){//bg <job>命令:在后台运行它
                j->state=BG;
                printf("[%d] (%d) %s",j->jid, j->pid, j->cmdline);
            }else if(strcmp(argv[0],"fg") == 0){//fg<job>命令:在前台运行它
                j->state = FG;
                waitfg(j->pid);
            }
        }else{
    	    printf("%s: No such job\n", argv[1]);
        }
    }else if(sscanf(argv[1], "%d", &id) > 0){//3：后面为纯数字，则看是不是进程pid
    	printf("(%d): No such process\n", id);
    	return;
    }else{//2：后面所跟既不为纯数字也不为%数字
    	printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    }
    return;
}
 
/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
/*用sigsuspend，根据子进程是否结束，发出SIG_CHILD来决定是否返回。*/
void waitfg(pid_t pid)
{
    sigset_t mask;
    sigemptyset(&mask);
 
    //看看是否到了要停止阻塞的时候
    if(pid==0) return;
    else{
        while(pid==fgpid(jobs))//fgpid:返回当前前台作业的pid,如果没有这样的作业，返回0
            sigsuspend(&mask);
    }
    return;
}
 
/*****************
 * Signal handlers
 *****************/
 
/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
 /*sigchld_handler-每当子作业终止（成为僵尸）或由于接收到SIGSTOP或SIGTSTP信号而停止时，
内核都会将SIGCHLD发送到外壳。 处理程序会收割所有可用的僵死子进程，但不等待其他任何正在
运行的子进程终止。
*/
void sigchld_handler(int sig) 
{
    int olderrno=errno;
    sigset_t mask_all,prev_all;
    pid_t pid;
    int status;
 
    sigfillset(&mask_all);
    while((pid=waitpid(-1,&status,WNOHANG|WUNTRACED))>0){
        //立即返回，如果等待集合中的子进程都没有被停止或终止，则返回值为0.
        //如果有一个停止或终止，则返回值为该子进程的pid
        //在status中放上关于导致返回的子进程的状态信息
        sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
        //deletejob(jobs,pid);
        if(WIFEXITED(status)){//子进程因为exit或者返回正常终止，直接收割即可
            deletejob(jobs,pid);
        }else if(WIFSIGNALED(status)){//子进程是因为一个未被捕获的信号终止
            printf("Job [%d] (%d) terminated by signal 2\n", pid2jid(pid), pid);
            deletejob(jobs,pid);//收割子进程
        }else if(WIFSTOPPED(status)){//引起返回的子进程是停止的，不收割，仅按要求输出并更改好状态即可
            printf("Job [%d] (%d) stopped by signal 20\n", pid2jid(pid), pid);
            getjobpid(jobs,pid)->state=ST;
        }
        sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    errno=olderrno;
    return;
}
 
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
 //每当用户在键盘上键入ctrl-c时，内核就会将SIGINT发送到外壳。 捕获并将其发送到前台作业
void sigint_handler(int sig) 
{
    pid_t pid=fgpid(jobs);//找到前台进程的pid
    if(pid!=0){//pid有效//如果pid有效
        kill(-pid,sig);//发送终止信号
    }
    return;
}
 
/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
/*每当用户在键盘上键入ctrl-c时，内核就会将SIGINT发送到外壳。捕获并将其发送到前台作业。*/
void sigtstp_handler(int sig) 
{
    pid_t pid=fgpid(jobs);//找到前台进程的pid
    if(pid!=0){//如果pid有效
        kill(-pid,sig);//发送终止信号
    }
    return;
}
 
/*********************
 * End signal handlers
 *********************/
 
/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/
 
/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
    job->pid = 0;
    job->jid = 0;
    job->state = UNDEF;
    job->cmdline[0] = '\0';
}
 
/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
    int i;
 
    for (i = 0; i < MAXJOBS; i++)
	clearjob(&jobs[i]);
}
 
/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs) 
{
    int i, max=0;
 
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid > max)
	    max = jobs[i].jid;
    return max;
}
 
/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline) 
{
    int i;
    
    if (pid < 1)
	return 0;
 
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == 0) {
	    jobs[i].pid = pid;
	    jobs[i].state = state;
	    jobs[i].jid = nextjid++;
	    if (nextjid > MAXJOBS)
		nextjid = 1;
	    strcpy(jobs[i].cmdline, cmdline);
  	    if(verbose){
	        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
            }
            return 1;
	}
    }
    printf("Tried to create too many jobs\n");
    return 0;
}
 
/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid) 
{
    int i;
 
    if (pid < 1)
	return 0;
 
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid == pid) {
	    clearjob(&jobs[i]);
	    nextjid = maxjid(jobs)+1;
	    return 1;
	}
    }
    return 0;
}
 
/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
    int i;
 
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].state == FG)
	    return jobs[i].pid;
    return 0;
}
 
/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
    int i;
 
    if (pid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid)
	    return &jobs[i];
    return NULL;
}
 
/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid) 
{
    int i;
 
    if (jid < 1)
	return NULL;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].jid == jid)
	    return &jobs[i];
    return NULL;
}
 
/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid) 
{
    int i;
 
    if (pid < 1)
	return 0;
    for (i = 0; i < MAXJOBS; i++)
	if (jobs[i].pid == pid) {
            return jobs[i].jid;
        }
    return 0;
}
 
/* listjobs - Print the job list */
void listjobs(struct job_t *jobs) 
{
    int i;
    
    for (i = 0; i < MAXJOBS; i++) {
	if (jobs[i].pid != 0) {
	    printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
	    switch (jobs[i].state) {
		case BG: 
		    printf("Running ");
		    break;
		case FG: 
		    printf("Foreground ");
		    break;
		case ST: 
		    printf("Stopped ");
		    break;
	    default:
		    printf("listjobs: Internal error: job[%d].state=%d ", 
			   i, jobs[i].state);
	    }
	    printf("%s", jobs[i].cmdline);
	}
    }
}
/******************************
 * end job list helper routines
 ******************************/
 
 
/***********************
 * Other helper routines
 ***********************/
 
/*
 * usage - print a help message
 */
void usage(void) 
{
    printf("Usage: shell [-hvp]\n");
    printf("   -h   print this message\n");
    printf("   -v   print additional diagnostic information\n");
    printf("   -p   do not emit a command prompt\n");
    exit(1);
}
 
/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}
 
/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
    fprintf(stdout, "%s\n", msg);
    exit(1);
}
 
/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;
 
    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */
 
    if (sigaction(signum, &action, &old_action) < 0)
	unix_error("Signal error");
    return (old_action.sa_handler);
}
 
/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig) 
{
    printf("Terminating after receipt of SIGQUIT signal\n");
    exit(1);
}