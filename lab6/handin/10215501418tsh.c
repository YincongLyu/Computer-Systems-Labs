/* 
 * tsh - A tiny shell program with job control
 * 
 * 10215501418
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
void eval(char *cmdline) 
{
    char *argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    
    sigset_t prev;
    sigset_t mask;
    pid_t pid;
    sigprocmask(SIG_BLOCK,NULL,&mask);
    sigaddset(&mask,SIGCHLD);
    strcpy(buf,cmdline);
    
    bg=parseline(buf,argv);  //调用parseline函数解析命令行，如果为空直接返回，接着使用builtin_cmd函数判断是否为内置命令，返回0说明不是内置命令，如果是内置命令直接执行。
    
    if(argv[0]==NULL) return;
    if(!builtin_cmd(argv))           // 当指令不是内置指令时：先阻塞信号，再调用fork创建子进程。在子进程中，首先解除阻塞，设置自己的id号，然后调用execve函数来执行job。
    {
        sigprocmask(SIG_BLOCK,&mask,&prev);// 父进程在创建子进程时，不要响应子进程结束中断，故添加SIGCHLD到阻塞队列
        if((pid=fork())==0)//子进程
        {
            sigprocmask(SIG_SETMASK,&prev,NULL);// 取消阻塞，因为子进程继承了父进程的阻塞队列，所以要将子进程的阻塞队列清空
            setpgid(0,0); // 将子进程单独放入一个进程组里面，否则在删除job的时候可能删除shell进程的别的子进程
            if(execve(argv[0],argv,environ)<0)
            {
                printf("%s:Command not found.\n",argv[0]);
                exit(0);
            }
        }
   
        else//父进程：父进程判断作业是否后台运行，是的话调用addjob函数将子进程job加入job链表中，并解除阻塞，然后调用waitfg函数等待前台运行完成。如果不在后台工作则打印进程组jid和子进程pid以及命令行字符串。
        {
            if(bg)//后台任务
                addjob(jobs,pid,BG,cmdline);
            else//前台任务
                addjob(jobs,pid,FG,cmdline);
            sigprocmask(SIG_SETMASK,&prev,NULL);
            if(bg)//后台任务不等待任务结束
                printf("[%d](%d)%s",pid2jid(pid),pid,cmdline);
            else//前台任务，等待任务结束
                waitfg(pid);
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
int builtin_cmd(char** argv)
{
    if (!strcmp(argv[0], "quit")) //当命令为quit时，直接终止shell
        exit(0);
    
    if (!strcmp(argv[0], "fg") || !strcmp(argv[0], "bg")) //当命令为bg或fg时，调用do_bgfg函数
    {
        do_bgfg(argv);
        return 1;
    }
    
    if(!strcmp(argv[0], "jobs")) //当命令为jobs时，调用listjobs函数
    {
        listjobs(jobs);
        return 1;
    }

    return 0; /* not a builtin command */
}


/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    sigset_t mask_all, prev_mask;
    sigfillset(&mask_all);
    // 访问全局变量jobs，阻塞所有信号
    sigprocmask(SIG_SETMASK, &mask_all, &prev_mask);

    struct job_t *job;
    int pid;
    if(argv[1] == NULL)   //当命令行为空
    {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }
    //判断argv[]是否带%，若为整数则传入pid，若带%则传入jid
    else if(argv[1][0] == '%')  //传入jid
    {
        int jid = atoi(argv[1] + 1);
        job = getjobjid(jobs, jid);
        if(job == NULL)  //未找到对应job
        {
            printf("%%%d: No such job\n", jid);
            return;
        }
        pid = job->pid;
    }
    else              //传入pid
    {
        pid = atoi(argv[1]);
        if(pid <= 0)
        {
            printf("%s: argument must be a PID or %%jobid\n", argv[0]);
            return;
        }
        job = getjobpid(jobs, pid);
        if(job == NULL)
        {
            printf("(%d): No such process\n", pid); //未找到对应进程
            return;
        }
    }
    //判断是bg命令还是fg命令
    if(!strcmp(argv[0], "bg"))//若是bg，使目标进程重新开始工作，设置状态为BG，打印进程信息
    {
        job->state = BG;
        printf("[%d] (%d) %s", job->jid, pid, job->cmdline);
        sigprocmask(SIG_SETMASK, &prev_mask, NULL);
        kill(-pid, SIGCONT); // 使用负数对其子进程及后代发送信号
        return;
    }
    
    else if(!strcmp(argv[0], "fg"))//若是fg，使目标进程重新开始工作，设置状态为FG，等待进程结束
    {
        job->state = FG;
        sigprocmask(SIG_SETMASK, &prev_mask, NULL);
        kill(-pid, SIGCONT); // 使用负数对其子进程及后代发送信号
        waitfg(pid);  // 当一个进程被设置为前台执行时，当前tsh应该等待该子进程结束
        return;
    }
    return;
}



/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    sigset_t m;
    sigemptyset(&m);
    while(pid==fgpid(jobs))
        sigsuspend(&m);//有信号时被唤醒检查前台进程pid是否变化，变化则说明前台进程结束。
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
void sigchld_handler(int sig) 
{
    pid_t pid;
    int status;
    while((pid=waitpid(-1,&status,WNOHANG|WUNTRACED))>0)//调用waitpid直到它所有的子进程终止
    {
        if(WIFEXITED(status))//当子进程通过调用exit 或者return正常终止
            deletejob(jobs,pid);
        
        if(WIFSTOPPED(status))//当子进程收到了一个暂停信号（还没退出），即任务挂起
        {
            struct job_t *job=getjobpid(jobs,pid);
            int jid=pid2jid(pid);
            printf("Job [%d] (%d) stopped by signal %d\n",jid,pid,WSTOPSIG(status));
            job->state=ST;   //状态设为挂起 
        }
        
        if(WIFSIGNALED(status))//当子进程是因为一个未被捕获的信号终止，即任务被终止
        {
            int jid=pid2jid(pid);
            printf("Job [%d] (%d) terminated by signal %d\n",jid,pid,WTERMSIG(status));
            deletejob(jobs,pid);
        }

    }
    return;
}


/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
    pid_t pid=fgpid(jobs);  //返回前台进程pid
    if(pid!=0)    //如果当前进程pid不为0，发送SIGINT信号给前台进程组
        kill(-pid,sig);   
    return;
}


/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    pid_t pid=fgpid(jobs); //返回前台进程pid
    if(pid>0)
        kill(-pid,sig);    //发送SIGTSPT信号给前台进程组
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



