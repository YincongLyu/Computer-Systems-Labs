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
    char *argv[MAXLINE];
    char buf[MAXLINE];
    int FG_BG;
    pid_t pid;

    strcpy(buf, cmdline);
    FG_BG = parseline(buf, argv) + 1; //宏中定义了FG与BG的数值
    if (argv[0] ==NULL)
        return; //空白命令

    if (!builtin_cmd(argv)) //如果不是内置命令，创建子进程
    {
        sigset_t mask_all, mask_one, prev_one;

        sigfillset(&mask_all);
        sigemptyset(&mask_one);
        sigemptyset(&prev_one);
        sigaddset(&mask_one, SIGCHLD);

        sigprocmask(SIG_BLOCK, &mask_one, &prev_one); //在调用fork之前阻塞信号，防止delete先于add发生
        if ((pid = fork()) == 0) //是子进程
        {
            setpgid(0,0); //设置新的进程组，隔离信号
            sigprocmask(SIG_SETMASK, &prev_one, NULL); //解除阻塞
            if (execve(argv[0], argv, environ) == -1) //命令未找到
            {
                printf("%s: Command not found\n", argv[0]);
                exit(0);
            }
        }
        else //父进程
        {
            sigprocmask(SIG_BLOCK, &mask_all, NULL); //全部阻塞
            addjob(jobs, pid, FG_BG, buf);
            sigprocmask(SIG_SETMASK, &mask_one, NULL); //全部解除

            if (FG_BG == FG) //前台
            {
                waitfg(pid);
            }
            else
            {
                sigprocmask(SIG_SETMASK, &mask_all, NULL);
                int jid = pid2jid(pid);
                if (FG_BG == BG) //后台
                    printf("[%d] (%d) %s", jid, pid, buf); //后台运行的提示语句
            }
        }
        sigprocmask(SIG_SETMASK, &prev_one, NULL); //全部解除
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
    if (!strcmp(argv[0], "quit")) //quit命令直接终止
        exit(0);

    int ifjobs = !(strcmp(argv[0], "jobs"));
    int iffg = !(strcmp(argv[0], "fg"));
    int ifbg = !(strcmp(argv[0], "bg"));

    if (!ifjobs && !iffg && !ifbg)
        return 0; /* not a builtin command */
    
    if(ifjobs)
        listjobs(jobs);
    else if (iffg || ifbg)
        do_bgfg(argv);

    return 1; //确认为内置命令
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) 
{
    int FG_BG = (!strcmp(argv[0], "bg")) + 1;

    if (argv[1] == NULL)
    {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }

    int ifpid = (argv[1] && argv[1][0] >= '0' && argv[1][0] <= '9'); //维护pid的格式
    int ifjid = (argv[1] && argv[1][0] == '%'); //维护jid的格式
    if (!ifpid && !ifjid)
    {
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        return;
    }

    struct job_t *job_ptr = NULL;
    if (ifpid)
        job_ptr = getjobpid(jobs, atoi(argv[1]));
    else
        job_ptr = getjobjid(jobs, atoi(argv[1]+1));

    if(!job_ptr)
    {
        if(ifpid)
            printf("(%d): No such process\n", atoi(argv[1]));
        else
            printf("%s: No such job\n", argv[1]);
        return;
    }

    job_ptr->state = FG_BG; //更新状态
    kill(-job_ptr->pid, SIGCONT); //更新后重新应用

    if (FG_BG == BG)
        printf("[%d] (%d) %s", job_ptr->jid, job_ptr->pid, job_ptr->cmdline); //切换到后台输出提示语句
    if (FG_BG == FG)
        waitfg(job_ptr->pid); //切换到前台需要等待运行结束
        
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    sigset_t mask, prev;

    sigemptyset(&mask);

    while (1)
    {
        sigprocmask(SIG_SETMASK,&mask, &prev); //阻塞SIGCHLD
        if (!fgpid(jobs)) //没有前台进程了
            break;
        sleep(1);
        sigprocmask(SIG_SETMASK, &prev, NULL); //解除阻塞
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
void sigchld_handler(int sig) 
{
    int olderrno = errno, status; //保存当前错误码以供后续恢复
    sigset_t mask_all, prev_all;
    pid_t pid;

    sigemptyset(&prev_all);
    sigfillset(&mask_all);

    while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED)) > 0) //回收子进程
    {
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all); //阻塞信号
        struct job_t *job = getjobpid(jobs, pid);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT && job->state != UNDEF) //外部的SIGINT
            printf("Job [%d] (%d) terminated by signal 2\n", job->jid, job->pid);
        else if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTSTP && job->state != ST) //外部SIGTSTP
        {
            printf("Job [%d] (%d) terminated by signal 20\n", job->jid, job->pid);
            job->state = ST; //相当于STOP
        }
        if (job->state != ST)
            deletejob(jobs, pid); //如果终止，则从列表中删除子进程；如果STOP则不删除
        sigprocmask(SIG_SETMASK, &prev_all, NULL);// 解除阻塞
    }
    errno = olderrno; //恢复错误码
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) 
{
    int pid = fgpid(jobs);
    int jid = pid2jid(pid);
    sigset_t mask_all, prev_all;

    sigemptyset(&prev_all);
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all); //阻塞信号

    if (!pid) //没有前台进程
        return;
    
    struct job_t *job = getjobpid(jobs, pid);
    job->state = UNDEF; //修改状态为终止
    kill(-pid, 2); //向全部前台进程发送信号
    printf("Job [%d] (%d) terminated by signal 2\n", jid, pid);
    sigprocmask(SIG_SETMASK, &prev_all, NULL); //解除阻塞
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) 
{
    int pid = fgpid(jobs);
    int jid = pid2jid(pid);
    sigset_t mask_all, prev_all;

    sigemptyset(&prev_all);
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all); //阻塞信号

    if (!pid) //没有前台进程
        return;

    struct job_t *job = getjobpid(jobs, pid);
    job->state = ST; //修改状态为STOP
    kill(-pid, 20); //向全部前台进程发送信号
    printf("Job [%d] (%d) stopped by signal 20\n", jid, pid);
    sigprocmask(SIG_SETMASK, &prev_all, NULL); //解除阻塞
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


