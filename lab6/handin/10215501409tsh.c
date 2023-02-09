/* 
 * tsh - A tiny shell program with job control
 * 
 * <Put your name and login ID here>
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
void eval(char *cmdline);           //解析并执行命令
int builtin_cmd(char **argv);       //检测命令是否为内置命令quit、fg、bg、jobs
void do_bgfg(char **argv);          //实现bg、fg命令
void waitfg(pid_t pid);             //等待前台命令执行完成

void sigchld_handler(int sig);      //处理SIGCHLD信号，即子进程停止或终止
void sigtstp_handler(int sig);      //处理SIGTSTP信号，即终端停止信号ctrl-z
void sigint_handler(int sig);       //处理SIGINT信号，即来自键盘的中断ctrl-c

/* Here are helper routines that we've provided for you */
int parseline(const char *cmdline, char **argv);  //获取参数列表char **argv，返回是否为后台运行命令（true）
void sigquit_handler(int sig);          //处理SIGQUIT信号

void clearjob(struct job_t *job);       //清除job结构
void initjobs(struct job_t *jobs);      //初始化jobs链表
int maxjid(struct job_t *jobs);         //返回jobs链表中最大的jid号
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);        //在jobs链表中添加job
int deletejob(struct job_t *jobs, pid_t pid);       //在jobs链表中删除pid的job
pid_t fgpid(struct job_t *jobs);        //返回当前前台运行job的pid号
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);     //返回pid号的job
struct job_t *getjobjid(struct job_t *jobs, int jid);       //返回jid号的job
int pid2jid(pid_t pid);                 //将pid号转化为jid
void listjobs(struct job_t *jobs);      //打印jobs

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
void eval(char *cmdline) 
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;

    sigset_t mask_all , mask_one , prev_one;
    sigfillset(&mask_all);
    sigemptyset(&mask_one);
    sigaddset(&mask_one,SIGCHLD);

    strcpy(buf,cmdline);
    bg=parseline(buf,argv);

    if (argv[0]==NULL) return;      //忽略空白行
    if (!builtin_cmd(argv)){
        sigprocmask(SIG_BLOCK,&mask_one,&prev_one); //block
        if ((pid=fork())==0){
            sigprocmask(SIG_SETMASK,&prev_one,NULL); //unblock
            setpgid(0,0);
            if (execve(argv[0],argv,environ)<0){    
                printf("%s: Command not found.\n",argv[0]);
                exit(0);        //如果命令不存在，要退出改子进程
            }

        }
        sigprocmask(SIG_BLOCK,&mask_all,NULL); //block，为什么需要这行
        int st =(bg==0) ? FG : BG;
        addjob(jobs,pid,st,cmdline);
        sigprocmask(SIG_SETMASK,&prev_one,NULL);   //unblock,应该放到前后台执行前 
        //前台
        if (!bg){
            waitfg(pid);
            // int status;
            // if (waitpid(pid, &status,0) < 0)
            //     unix_error("waitfg: waitpid error");
        }
        // 后台
        else{
            printf("[%d] (%d) %s",pid2jid(pid),pid,cmdline);   
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
int builtin_cmd(char **argv) 
{
    if(!strcmp(argv[0],"quit")){ /* quit command */
        exit(0);
    } 
    if (!strcmp(argv[0], "&")){ /* Ignore singleton & */
        return 1; 

    }
    if(!strcmp((argv[0]),"jobs"))/* jobs command */
    {
        listjobs(jobs);
        return 1;
    }
    if(!strcmp((argv[0]),"fg") || !strcmp((argv[0]),"bg"))/* bg/fg command */
    {
        do_bgfg(argv);
        return 1;
    }

    return 0;     /* not a builtin command */
}


/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    pid_t pid;
    struct job_t* job;
    // 去除非正常输入
    if(!argv[1]){
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }
    if (!isdigit(argv[1][0]) && argv[1][0] != '%') {            // Checks if the second argument is valid
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        return;
    }
    
    //根据输入找到进程
    if (argv[1][0]=='%'){
        int jid=atoi(&argv[1][1]);//从第二个字符开始，并转化成整形
        pid=jobs[jid-1].pid;  //jobs从0开始
        job=getjobjid(jobs,jid);  
        //如果没有给出进程
        if(!job){
            printf("%s: No such job\n", argv[1]);
            return;
        }      
    }
    else{
        pid=atoi(argv[1]);
        job=getjobpid(jobs,pid);
        //如果没有给出进程
        if (!job) {                 
            printf("(%d): No such process\n", atoi(argv[1]));
            return;
        }
    }


    //发送继续的信号
    kill(-pid,SIGCONT);
    //bg
    if (!strcmp((argv[0]),"bg")){
        job->state=BG;
        printf("[%d] (%d) %s",pid2jid(pid),pid,job->cmdline);   
    }
    //fg
    else{
        job->state=FG;
        waitfg(pid);
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)      //循环等待
{
    while (fgpid(jobs)){
        usleep(1000);
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
// 无论怎样结束都会发送sigchld，因此要判别是怎样结束的
void sigchld_handler(int sig) 
{
    int olderrno=errno;  //errno专有
    sigset_t mask_all,prev_all;
    pid_t pid;
    int status;

    sigfillset(&mask_all);
    // while ((pid=waitpid(-1,NULL,0))>0){
    while ((pid=waitpid(-1,&status,WNOHANG | WUNTRACED))>0){
        // 正常退出
        if (WIFEXITED(status)){
            sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
            deletejob(jobs,pid);
            sigprocmask(SIG_SETMASK,&prev_all,NULL);
        }
        // 退出
        else if (WIFSIGNALED(status)){
            sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
            printf("Job [%d] (%d) terminated by signal 2\n",pid2jid(pid),pid);
            deletejob(jobs,pid);
            sigprocmask(SIG_SETMASK,&prev_all,NULL);
        }
        // 停止
        else{
            struct job_t* job=getjobpid(jobs,pid);
            sigprocmask(SIG_BLOCK,&mask_all,&prev_all);
            //修改job状态
            job->state=ST;
            printf("Job [%d] (%d) stopped by signal 20\n",pid2jid(pid),pid);
            sigprocmask(SIG_SETMASK,&prev_all,NULL);
        }
    }
    errno=olderrno;
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
// 按Ctrl+C时，内核会自动停止程序，发送SIGINT信号
void sigint_handler(int sig) 
{
    int olderrno=errno;  //errno专有
    pid_t pid=fgpid(jobs);

    if (pid){
        //给进程组|fg|中每个进程发送同样的信号
        //一般情况下,一个进程一个组，除非子进程fork之后还未execve
        kill(-pid,sig);      
    }
    errno=olderrno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    int olderrno=errno;  //errno专有
    pid_t pid=fgpid(jobs);

    if (pid){
        //给进程组|fg|中每个进程发送同样的信号
        //一般情况下,一个进程一个组，除非子进程fork之后还未execve
        kill(-pid,sig);      
    }
    errno=olderrno;
    return;}

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



