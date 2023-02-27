/* 
 * tsh - A tiny shell program with job control
 * 
 * <汪柔柔  10215501439>
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

typedef short bool;
/* bool define */
#define false 0
#define true 1

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
void eval(char* cmdline)
{
    char* argv[MAXARGS];
    char buf[MAXLINE];
    int bg;
    pid_t pid;
    sigset_t mask_all, prev_all;
    
    //复制命令行给buf
    strcpy(buf, cmdline);
    //调用parseline解析并构建argv数组
    bg = parseline(cmdline, argv);
    
    //对于第一个参数为0时，无任务，无命令，直接返回
    if (argv[0] == NULL)
        return;

    //通过先排除第一个参数为0，和当前排除是内置命令的情况，避免if语句嵌套
    if (builtin_cmd(argv)) 
        return;

    //阻塞所有信号，保证安全
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    

    //对于需要运行的可执行文件而言，针对子进程
    if ((pid = fork()) == 0) 
    {
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        //更改子进程的进程组id
        setpgid(0, 0);
        

        //倘若运行失败，则推出（最好是安全退出）（潜在包含了加载、执行的命令）
        if (execve(argv[0], argv, environ) < 0)
        {
            printf("%s: Command not found.\n", argv[0]);
            exit(0);
        }
    }

    // 子进程不运行阶段
    //倘若是前台进程
    if (!bg)
    {
        //将工作加入前台列表
        addjob(jobs, pid, FG, cmdline);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        //回收当前状态的僵尸子进程
        waitfg(pid);
    }
    else // 后台进程的情况，不需要等待后台子进程结束
    {
        addjob(jobs, pid, BG, cmdline);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
    }
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
    //对于quit，直接效仿书上，退出
    if (strcmp(argv[0], "quit") == 0)
        exit(0);
    //对于jobs，打印工作列表
    else if (strcmp(argv[0], "jobs") == 0)
    {
        listjobs(jobs);
        return 1;
    }
    //对于bg和fg命令，用do_bg专门处理bg和fg命令
    else if (strcmp(argv[0], "fg") == 0 || strcmp(argv[0], "bg") == 0)
    {
        do_bgfg(argv); 
        return 1;
    }
    //并不单独处理“&”
    else if (strcmp(argv[0], "&") == 0) 
        return 1;
    //对于非内置命令，也即对于可执行文件直接返回0
    return 0;     
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char** argv)
{
    //没有额外参数
    if (argv[1] == NULL)
    {
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return;
    }

    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    

    struct job_t* job;
    int jid = 0, pid = 0;

    // 获取 jid / pid
    if (argv[1][0] == '%')
        jid = atoi(argv[1] + 1);
    else
        pid = atoi(argv[1]);

    // 错误情况一：%0  /  0
    if ((jid == 0 && argv[1][0] == '%') || (pid == 0 && argv[1][0] != '%'))
    {
        printf("%s: argument must be a PID or %%jobid\n", argv[0]);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        return;
    }

    //错误情况二： jid=0
    if (jid == 0)
        jid = pid2jid(pid);
    job = getjobjid(jobs, jid);

    // 错误情况三：  工作不存在
    if (job == NULL)
    {
        if (argv[1][0] != '%')
            printf("(%s): ", argv[1]);
        else
            printf("%s: ", argv[1]);
        printf("No such %s\n", argv[1][0] == '%' ? "job" : "process");

        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        return;
    }

    // 在正确的情况下，输入bg
    if (strcmp(argv[0], "bg") == 0)
    {
        job->state = BG;
        printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
        kill(-(job->pid), SIGCONT);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
    }
    // 在正确的情况下，输入fg
    else
    {
        if (job->state == ST)
            kill(-(job->pid), SIGCONT);
        job->state = FG;
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        waitfg(job->pid);
    }
}
/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);  //对于当前信号集初始化为满状态

    while (1)
    {
         // 在访问全局变量前，阻塞有关的信号
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
        struct job_t* foreground_job = getjobpid(jobs, pid);
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        
        // 当找不到前台进程，或者前台进程已经挂起，就退出循环
        if (!foreground_job || foreground_job->state != FG)
            break;

        sleep(1);
    }
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
    // 存储 errno 
    int olderrno = errno;
    int status;
    pid_t pid;
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);

    // 注意第三个参数
    while ((pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0)
    {
        sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
    
        //倘若是由于ctrl+c引起的进程终止
        if (WIFSIGNALED(status))
        {
            printf("Job [%d] (%d) terminated by signal %d\n", 
                    pid2jid(pid), pid, WTERMSIG(status));
            
            deletejob(jobs, pid);
        }
        //倘若是由于ctrl+z引起的进程停止
        else if (WIFSTOPPED(status))
        {
            printf("Job [%d] (%d) stopped by signal %d\n", 
                    pid2jid(pid), pid, WSTOPSIG(status));
            //将子进程的状态修改为ST（暂停状态），而不需要将进程从工作列表中删除
            struct job_t* job = getjobpid(jobs, pid);
            job->state = ST;
        }
        //倘若子进程正常终止
        else 
            deletejob(jobs, pid);
    }

    //解除所有信号的阻塞
    sigprocmask(SIG_SETMASK, &prev_all, NULL);
    //恢复errno
    errno = olderrno;
}
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig)
{
    //首先暂时缓存errno
    int olderrno = errno;
    
    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    //阻塞所有信号
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);
 
    //获取前台所有进程pid
    pid_t pid = fgpid(jobs);
    //若无前台进程，则返回0
    if (pid == 0)
    {
        // 复原errno，并解除阻塞，再返回
        errno = olderrno;
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        return;
    }
    
    //若存在前台进程，则将SIGINT信号发送给所有前台进程
    kill(-pid, SIGINT);
    sigprocmask(SIG_SETMASK, &prev_all, NULL);

    errno = olderrno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig)
//除了发送信号不同以外，其余皆和上面sigint_handler类似
{
    //首先暂时缓存errno
    int olderrno = errno;

    sigset_t mask_all, prev_all;
    sigfillset(&mask_all);
    //阻塞所有信号，以上面一行代码可以看出mask_all是满信号集
    sigprocmask(SIG_BLOCK, &mask_all, &prev_all);

    pid_t pid = fgpid(jobs);
    //若无前台进程，则返回0
    if (pid == 0)
    {
        // 复原errno，并解除阻塞，再返回
        errno = olderrno;
        sigprocmask(SIG_SETMASK, &prev_all, NULL);
        return;
    }

    // 发送SIGTSTP信号给所有原始进程创建的子进程和孙进程
    kill(-pid, SIGTSTP);
    //解除阻塞
    sigprocmask(SIG_SETMASK, &prev_all, NULL);

    //复原errno
    errno = olderrno;
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
