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

/* fdddd */
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
int verbose = 0;            /* if true, print additional output;verbose:冗长的;啰唆的*/
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

/*Error-handling Wrappers*/
pid_t Fork(void){
    pid_t pid;
    if((pid=fork())<0)
        unix_error("Fork error");
    return pid;
}

void Execve(const char *filename, char *const argv[], char *const environ[])
{
    if(execve(filename,argv,environ)<0){
        printf("%s: Command not found.\n",argv[0]);
        exit(0);
    }
}

void Kill(pid_t pid, int signum) 
{
    int kr;
    if ((kr = kill(pid, signum)) < 0)
        unix_error("Kill error");
    return;

}

void Sigemptyset(sigset_t *set)
{
    if(sigemptyset(set)<0)
        unix_error("Sigemptyset error");
    return;
}


void Sigaddset(sigset_t *set,int sign)
{
    if(sigaddset(set,sign)<0)
        unix_error("Sigaddset error");
    return;
}


void Sigprocmask(int how, sigset_t *set, sigset_t *oldset)
{
    if(sigprocmask(how,set,oldset)<0)
        unix_error("Sigprocmask error");
    return;
}


void Sigfillset(sigset_t *set)
{
    if(sigfillset(set)<0)
        unix_error("Sigfillset error");
    return;
}


void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
        unix_error("Setpgid error");
    return;
}

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

    /* Parse the command line ，查看运行程序tsh时是否带了可执行参数(-h,-v,-p)*/
    while ((c = getopt(argc, argv, "hvp")) != EOF) { /*EOF:End of File*/
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

<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
/*利用书上已有的框架和543提到的避免并发竞争发生*/


void eval(char *cmdline) 
{
    char *argv[MAXARGS];/*Argument list execve()*/
    char buf[MAXLINE];  /*Holds modified command line*/
    int bg;             /*Should the job run in bg or fg? */
    pid_t pid;          /*Process id*/

    strcpy(buf,cmdline);
    bg=parseline(buf,argv); 

    if(argv[0]==NULL)
        return; /*Ignore empty lines*/
    sigset_t mask_all,mask_one,prev_one;
    
    Sigfillset(&mask_all);
    Sigemptyset(&mask_one);
    Sigaddset(&mask_one,SIGCHLD);
    if(!builtin_cmd(argv)){ /*如果是非shell内置命令*/
        Sigprocmask(SIG_BLOCK,&mask_one,&prev_one);/*Block SIGCHLD*/
        if((pid=Fork())==0){ /*Child process*/
            Setpgid(0,0);/*setpgid将子进程组和tsh进程组分开来，避免停止子进程组把tsh一起停止掉，同时子进程组id就等于pid，发送消息很方便*/
            Sigprocmask(SIG_SETMASK,&prev_one,NULL); /*Unblock SIGCHLD p542以避免子进程fork出来的子进程无法被回收*/
            Execve(argv[0],argv,environ);
        }
        
        /*Parents waits for foreground job to terminate*/
        if(!bg){ /*在前台工作*/
            Sigprocmask(SIG_BLOCK,&mask_all,NULL);
            addjob(jobs,pid,FG,cmdline);
            Sigprocmask(SIG_SETMASK,&prev_one,NULL);
            waitfg(pid); //显示地等待信号,p543;不在这里用waitpid的原因:handler里面有waitpid了,由handler来回收子进程,这样不仅可以回收前台子进程,也可以回收后台子进程
        }
        else{    /*在后台工作*/
            Sigprocmask(SIG_BLOCK,&mask_all,NULL);
            addjob(jobs,pid,BG,cmdline);
            printf("[%d] (%d) %s", pid2jid(pid),pid, cmdline);
            Sigprocmask(SIG_SETMASK,&prev_one,NULL);/*打印全局变量，仍然需要加塞，防止途中被中断，可能造成还未读(写)内存而内存的值却被修改的情况,见书p536 G3:对全局数据访问时要阻塞所有信号*/
        }
=======
void eval(char *cmdline)
{
    char* argv[MAXARGS];   //execve()函数的参数
    int state = UNDEF;  //工作状态，FG或BG 
    sigset_t set;
    pid_t pid;  //进程id
    // 处理输入的数据
    if(parseline(cmdline, argv) == 1)  //解析命令行，返回给argv数组
        state = BG;
    else
        state = FG;
    if(argv[0] == NULL)  //命令行为空直接返回
        return;
    // 如果不是内置命令
    if(!builtin_cmd(argv))
    {
        if(sigemptyset(&set) < 0)
            unix_error("sigemptyset error");
        if(sigaddset(&set, SIGINT) < 0 || sigaddset(&set, SIGTSTP) < 0 || sigaddset(&set, SIGCHLD) < 0)
            unix_error("sigaddset error");
        //在它派生子进程之前阻塞SIGCHLD信号，防止竞争 
        if(sigprocmask(SIG_BLOCK, &set, NULL) < 0)
            unix_error("sigprocmask error");

        if((pid = fork()) < 0)  //fork创建子进程失败 
            unix_error("fork error");
        else if(pid == 0)  //fork创建子进程
        {
            // 子进程的控制流开始
            if(sigprocmask(SIG_UNBLOCK, &set, NULL) < 0)  //解除阻塞
                unix_error("sigprocmask error");
            if(setpgid(0, 0) < 0)  //设置子进程id 
                unix_error("setpgid error");
            if(execve(argv[0], argv, environ) < 0){
                printf("%s: command not found\n", argv[0]);
                exit(0);
            }
        }
        // 将当前进程添加进job中，无论是前台进程还是后台进程
        addjob(jobs, pid, state, cmdline);
        // 恢复受阻塞的信号 SIGINT SIGTSTP SIGCHLD
        if(sigprocmask(SIG_UNBLOCK, &set, NULL) < 0)
            unix_error("sigprocmask error");

        // 判断子进程类型并做处理
        if(state == FG)
            waitfg(pid);  //前台作业等待
        else
            printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);  //将进程id映射到job id   
>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
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
int parseline(const char *cmdline, char **argv) /*类似于Python中str.strip(' ') */
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
<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
    if(!strcmp(argv[0],"quit"))  /*quit command*/
        exit(0);
    else if(!strcmp(argv[0],"&")) /*Ignore singleton单独的 & */
        return 1;
    else if(!strcmp(argv[0],"jobs")){/*jobs command*/
        listjobs(jobs);
        return 1;
    }
    else if(!strcmp(argv[0],"bg")){
        do_bgfg(argv);
        return 1;
    }
    else if(!strcmp(argv[0],"fg")){
        do_bgfg(argv);
        return 1;
    }
    return 0;     /* not a builtin command */
=======
    if(!strcmp(argv[0], "quit"))  //如果命令是quit，退出
        exit(0);
    else if(!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg"))  //如果是bg或者fg命令，执行do_fgbg函数 
        do_bgfg(argv);
    else if(!strcmp(argv[0], "jobs"))  //如果命令是jobs，列出正在运行和停止的后台作业
        listjobs(jobs);
    else
        return 0;     /* not a builtin command */
    return 1;
>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
    struct job_t *job=NULL;
    if(argv[1]==NULL){
        printf("%s command requires PID or %%jobid argument\n",argv[0]);
        return;
    }
    int id;
    //解析pid
    if(sscanf(argv[1],"%d",&id)>0){
        if((job=getjobpid(jobs,id))==NULL){
            printf("%s: No such process\n",argv[1]);
            return;
        }
    }
    //解析jid
    else if(sscanf(argv[1],"%%%d",&id)>0){
        if((job=getjobjid(jobs,id))==NULL){
            printf("%s:No such job\n",argv[1]);
            return;
        }
    }
    //不是pid也不是jid
    else{
        printf("%s: argument must be a PID or %%jobid\n",argv[0]);
        return;
    }
    
    //发送信号
    if(!strcmp(argv[0],"bg")){  //signal为bg
        printf("[%d] (%d) %s",job->jid,job->pid,job->cmdline);
        job->state=BG;
        Kill(-(job->pid),SIGCONT);
    }
    else if(!strcmp(argv[0],"fg")){ //signal为fg         
        Kill(-(job->pid),SIGCONT);
        job->state=FG;                    
        waitfg(job->pid);
=======
    int num;
    struct job_t *job;
    // 没有参数的fg/bg应该被丢弃
    if(!argv[1]){  //命令行为空
        printf("%s command requires PID or %%jobid argument\n", argv[0]);
        return ;
    }
    // 检测fg/bg参数，其中%开头的数字是JobID，纯数字的是PID
    if(argv[1][0] == '%'){  //解析jid
        if((num = strtol(&argv[1][1], NULL, 10)) <= 0){
            printf("%s: argument must be a PID or %%jobid\n",argv[0]);//失败,打印错误消息
            return;
        }
        if((job = getjobjid(jobs, num)) == NULL){
            printf("%%%d: No such job\n", num); //没找到对应的job 
            return;
        }
    } else {
        if((num = strtol(argv[1], NULL, 10)) <= 0){
            printf("%s: argument must be a PID or %%jobid\n",argv[0]);//失败,打印错误消息
            return;
        }
        if((job = getjobpid(jobs, num)) == NULL){
            printf("(%d): No such process\n", num);  //没找到对应的进程 
            return;
        }
    }

    if(!strcmp(argv[0], "bg")){
        // bg会启动子进程，并将其放置于后台执行
        job->state = BG;  //设置状态 
        if(kill(-job->pid, SIGCONT) < 0)  //采用负数发送信号到进程组 
            unix_error("kill error");
        printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
    } else if(!strcmp(argv[0], "fg")) {
        job->state = FG;  //设置状态 
        if(kill(-job->pid, SIGCONT) < 0)  //采用负数发送信号到进程组 
            unix_error("kill error");
        // 当一个进程被设置为前台执行时，当前tsh应该等待该子进程结束
        waitfg(job->pid);
    } else {
        puts("do_bgfg: Internal error");
        exit(0);
>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
    }
    return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
// void waitfg(pid_t pid)
// {
//     sigset_t mask, prev;
//     Sigemptyset(&mask);
//     Sigaddset(&mask, SIGCHLD);
//     Sigprocmask(SIG_BLOCK, &mask, &prev);/*Block SIGCHLD*/
//     while (fgpid(jobs) != 0){
//         sigsuspend(&mask);
//         printf("wait here");
//     }
//     Sigprocmask(SIG_SETMASK, &prev, NULL);
//     return;
// }
void waitfg(pid_t pid)
{
<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
    while(fgpid(jobs)==pid){//fgpid返回当前前台job的pid
        sleep(1);//spin p545
    }
=======
    struct job_t *job = getjobpid(jobs, pid);
    if(!job) return;

  
    while(job->state == FG)
   
        sleep(1);

>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
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
<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
    int olderrno=errno;
    pid_t pid;
    int status;
    struct job_t *job;
    sigset_t mask_all,prev_all;
    Sigfillset(&mask_all);
    while((pid=waitpid(-1,&status,WNOHANG | WUNTRACED))>0){/*Reap a zombie child*/
        Sigprocmask(SIG_BLOCK,&mask_all,&prev_all); //阻塞一切信号，防止deletejob被打断
        if(WIFEXITED(status))
            deletejob(jobs,pid);
        else if(WIFSIGNALED(status)){
            printf ("Job [%d] (%d) terminated by signal %d\n", pid2jid(pid), pid, WTERMSIG(status));
            deletejob(jobs,pid);
        }
        else if(WIFSTOPPED(status)){
            printf ("Job [%d] (%d) stoped by signal %d\n", pid2jid(pid), pid, WSTOPSIG(status));
            job = getjobpid(jobs,pid);
            job->state=ST;
        }
        Sigprocmask(SIG_SETMASK,&prev_all,NULL);
    }
    errno=olderrno;
=======
    int status, jid;
    pid_t pid;
    struct job_t *job;

    if(verbose)
        puts("sigchld_handler: entering");

    /*
    以非阻塞方式等待所有子进程
    waitpid 参数3：
        1.     0     ： 执行waitpid时， 只有在子进程 **终止** 时才会返回。
        2. WNOHANG   : 若子进程仍然在运行，则返回0 。
                注意只有设置了这个标志，waitpid才有可能返回0
        3. WUNTRACED : 如果子进程由于传递信号而停止，则马上返回。
                只有设置了这个标志，waitpid返回时，其WIFSTOPPED(status)才有可能返回true
    */
    while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0){

        // 如果当前这个子进程的job已经删除了，则表示有错误发生
        if((job = getjobpid(jobs, pid)) == NULL){
            printf("Lost track of (%d)\n", pid);
            return;
        }

        jid = job->jid;
        //接下来判断三种状态 
        // 如果这个子进程收到了一个暂停信号（还没退出） 
        if(WIFSTOPPED(status)){
            printf("Job [%d] (%d) stopped by signal %d\n", jid, job->pid, WSTOPSIG(status));
            job->state = ST;  //状态设为挂起 
        }
        // 如果子进程通过调用 exit 或者一个返回 (return) 正常终止
        else if(WIFEXITED(status)){
            if(deletejob(jobs, pid))
                if(verbose){
                    printf("sigchld_handler: Job [%d] (%d) deleted\n", jid, pid);
                    printf("sigchld_handler: Job [%d] (%d) terminates OK (status %d)\n", jid, pid, WEXITSTATUS(status));
                }
        }
        // 如果子进程是因为一个未被捕获的信号终止的，例如SIGKILL
        else {
            if(deletejob(jobs, pid)){  //清除进程
                if(verbose)
                    printf("sigchld_handler: Job [%d] (%d) deleted\n", jid, pid);
            }
            printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, WTERMSIG(status));  //返回导致子进程终止的信号的数量
        }
    }

    if(verbose)
        puts("sigchld_handler: exiting");

>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
    return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it alone
 *    to the foreground job.  
 */
void sigint_handler(int sig)
{
<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
    pid_t pid;
    if ((pid = fgpid(jobs)) > 0)
        Kill(-pid, sig);     
=======
    if(verbose)
        puts("sigint_handler: entering");
    pid_t pid = fgpid(jobs);

    if(pid){
      
        if(kill(-pid, SIGINT) < 0)
            unix_error("kill (sigint) error");
        if(verbose){
            printf("sigint_handler: Job (%d) killed\n", pid);
        }
    }
    if(verbose)
        puts("sigint_handler: exiting");
>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig)
{
<<<<<<< HEAD:lab6/shlab-handout/10215501431tsh.c
    pid_t pid;
    if ((pid = fgpid(jobs)) > 0)
        kill(-pid, sig);
=======
    if(verbose)
        puts("sigstp_handler: entering");

    pid_t pid = fgpid(jobs);
    struct job_t *job = getjobpid(jobs, pid);

    if(pid){
        if(kill(-pid, SIGTSTP) < 0)
            unix_error("kill (tstp) error");
        if(verbose){
            printf("sigstp_handler: Job [%d] (%d) stopped\n", job->jid, pid);
        }
    }
    if(verbose)
        puts("sigstp_handler: exiting");
>>>>>>> dd4833069f47401712bb4c770c652147f971905b:lab6/shlab-handout/tsh.c
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



