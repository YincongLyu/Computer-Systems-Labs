/* 
 * tsh - A tiny shell program with job control
 * 2023-dase-10211900416-tommy
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
#define MAXLINE 1024   /* max line size */
#define MAXARGS 128	   /* max args on a command line */
#define MAXJOBS 16	   /* max jobs at any point in time */
#define MAXJID 1 << 16 /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1	/* running in foreground */
#define BG 2	/* running in background */
#define ST 3	/* stopped */

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
extern char **environ;	 /* defined in libc */
char prompt[] = "tsh> "; /* command line prompt (DO NOT CHANGE) */
int verbose = 0;		 /* if true, print additional output */
int nextjid = 1;		 /* next job ID to allocate */
char sbuf[MAXLINE];		 /* for composing sprintf messages */

struct job_t
{						   /* The job struct */
	pid_t pid;			   /* job PID */
	int jid;			   /* job ID [1, 2, ...] */
	int state;			   /* UNDEF, BG, FG, or ST */
	char cmdline[MAXLINE]; /* command line */
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
	while ((c = getopt(argc, argv, "hvp")) != EOF)
	{
		switch (c)
		{
		case 'h': /* print help message */
			usage();
			break;
		case 'v': /* emit additional diagnostic info */
			verbose = 1;
			break;
		case 'p':			 /* don't print a prompt */
			emit_prompt = 0; /* handy for automatic testing */
			break;
		default:
			usage();
		}
	}

	/* Install the signal handlers */

	/* These are the ones you will need to implement */
	Signal(SIGINT, sigint_handler);	  /* ctrl-c */
	Signal(SIGTSTP, sigtstp_handler); /* ctrl-z */
	Signal(SIGCHLD, sigchld_handler); /* Terminated or stopped child */

	/* This one provides a clean way to kill the shell */
	Signal(SIGQUIT, sigquit_handler);

	/* Initialize the job list */
	initjobs(jobs);

	/* Execute the shell's read/eval loop */
	while (1)
	{

		/* Read command line */
		if (emit_prompt)
		{
			printf("%s", prompt);
			fflush(stdout);
		}
		if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
			app_error("fgets error");
		if (feof(stdin))
		{ /* End of file (ctrl-d) */
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
	/* $begin handout */
	char *argv[MAXARGS]={NULL}; /* argv for execve() */
	int bg;				 /* should the job run in bg or fg? */
	pid_t pid;			 /* process id */
	sigset_t mask;		 /* signal mask */

	/*解析命令行*/
	bg = parseline(cmdline, argv);
	if (argv[0] == NULL)return;//空行，直接返回

	if (!builtin_cmd(argv))
	{

		//这里需要一些技巧，就是在我们将job加到list之前一定要阻塞SIGCHLD,SIGINT,SIGTSTP
		//这消除了在添加任务到列表和SIGCHLD,SIGINT,SIGTSTP信号抵达的竞争问题

		if (sigemptyset(&mask) < 0)unix_error("sigemptyset error");
		if (sigaddset(&mask, SIGCHLD))unix_error("sigaddset error");
		if (sigaddset(&mask, SIGINT)) // SIGINT:Ctrl+c
			unix_error("sigaddset error");
		if (sigaddset(&mask, SIGTSTP)) // SIGTSTP:Ctrl+z
			unix_error("sigaddset error");
		if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)unix_error("sigprocmask error");

		//child 
		if ((pid = fork()) < 0)unix_error("fork error");

		if (pid == 0){
			//先解除堵塞
			sigprocmask(SIG_UNBLOCK, &mask, NULL);

		   //每一个新job一定要有一个新的进程组id
		   //否则内核就会将ctrl-c,ctrl-z信号发给所有的job
			if (setpgid(0, 0) < 0)//改变进程的进程组，不要跟tsh进程在一个进程组，然后调用exevce函数执行相关的文件。
				unix_error("setpgid error");

			//执行
			if (execve(argv[0], argv, environ) < 0){
				printf("%s: Command not found\n", argv[0]);
				exit(0);//子线程执行完毕后一定要退出,否则当execve函数无法执行的时候，子进程开始运行主进程的代码，出现不可预知的错误。
			}
		}

		//parent
	   	//parent先添加job,然后解锁信号以便再次运行
		addjob(jobs, pid, (bg == 1 ? BG : FG), cmdline);
		sigprocmask(SIG_UNBLOCK, &mask, NULL);
		if (!bg)
			waitfg(pid);
		else
			printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
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
	char *buf = array;			/* ptr that traverses command line */
	char *delim;				/* points to first space delimiter */
	int argc;					/* number of args */
	int bg;						/* background job? */

	strcpy(buf, cmdline);
	buf[strlen(buf) - 1] = ' ';	  /* replace trailing '\n' with space */
	while (*buf && (*buf == ' ')) /* ignore leading spaces */
		buf++;

	/* Build the argv list */
	argc = 0;
	if (*buf == '\'')
	{
		buf++;
		delim = strchr(buf, '\'');
	}
	else
	{
		delim = strchr(buf, ' ');
	}

	while (delim)
	{
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* ignore spaces */
			buf++;

		if (*buf == '\'')
		{
			buf++;
			delim = strchr(buf, '\'');
		}
		else
		{
			delim = strchr(buf, ' ');
		}
	}
	argv[argc] = NULL;

	if (argc == 0) /* ignore blank line */
		return 1;

	/* should the job run in the background? */
	if ((bg = (*argv[argc - 1] == '&')) != 0)
	{
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
	if (!strcmp(argv[0], "quit")) //命令行为内置命令quit，直接退出
		exit(0);
	else if (!strcmp(argv[0], "bg") || !strcmp(argv[0], "fg")) // 命令行为内置命令bg或fg，调用do_bgfg函数执行，并返回1（表示执行命令为内置命令）
	{
		do_bgfg(argv);
		return 1;
	}
	else if (!strcmp(argv[0], "jobs")) // 命令行为内置命令jobs，调用listjobs函数执行，并返回1（表示执行命令为内置命令）
	{
		listjobs(jobs);
		return 1;
	}
	return 0; /* not a builtin command */ // 命令非内置命令，返回0
}

/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
	struct job_t *jobp = NULL;

	if (argv[1] == NULL){//没带参数
		printf("%s command requires PID or %%jobid argument\n", argv[0]);
		return;
	}

	if (isdigit(argv[1][0])){ //pid
		pid_t pid = atoi(argv[1]);
		if (!(jobp = getjobpid(jobs, pid)))
		{
			printf("(%d): No such process\n", pid);
			return;
		}
	}
	else if (argv[1][0] == '%'){ //jid
		int jid = atoi(&argv[1][1]);
		if (!(jobp = getjobjid(jobs, jid))){
			printf("%s: No such job\n", argv[1]);
			return;
		}
	}else{
		printf("%s: argument must be a PID or %%jobid\n", argv[0]);
		return;
	}
	//bg
	if (!strcmp(argv[0], "bg")){
		if (kill(-(jobp->pid), SIGCONT) < 0)unix_error("kill (bg) error");
		jobp->state = BG;
		printf("[%d] (%d) %s", jobp->jid, jobp->pid, jobp->cmdline);
	}

	//fg
	else if (!strcmp(argv[0], "fg")){
		if (kill(-(jobp->pid), SIGCONT) < 0)unix_error("kill (fg) error");
		jobp->state = FG;
		waitfg(jobp->pid);
	}else{
		printf("do_bgfg: Internal error\n");
		exit(0);
	}
	return;
}

/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
	sigset_t mask;
    sigemptyset(&mask);   
    while (fgpid(jobs) != 0){
        sigsuspend(&mask);//暂停时取消阻塞
    }
    return;
	/*
	while (pid == fgpid(jobs)) // 等待前台程序不再是pid（fgpid函数用于获取此时正在运行的前台进程）
	{
		sleep(1); // 休眠1s
	}
	return;
	*/
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
	int status;
	int olderrno = errno; // 保存原始errno
	sigset_t mask, prev;
	pid_t pid;

	sigfillset(&mask); // 将所有信号都添加到mask中

	while ((pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0) 
	{// 若等待集合中的子进程都未停止/终止，则waitpid返回0，不执行while循环；while循环只在子进程出现停止/终止时运行
		if (WIFEXITED(status)) {// 子进程通过调用exit或return正常终止
			sigprocmask(SIG_BLOCK, &mask, &prev);  // 阻塞所有信号，完成后，blocked中所有信号均阻塞，prev中保存着blocked的原始情况
			deletejob(jobs, pid);				   // 从任务列表中删除相应jobs
			sigprocmask(SIG_SETMASK, &prev, NULL); // 解除信号阻塞，还原blocked的原始情况
		}
		else if (WIFSIGNALED(status)){// 子进程因为一个未捕获的信号而终止
			printf("Job [%d] (%d) terminated by signal %d\n", pid2jid(pid), pid, WTERMSIG(status));
			sigprocmask(SIG_BLOCK, &mask, &prev);  // 阻塞所有信号，完成后，blocked中所有信号均阻塞，prev中保存着blocked的原始情况
			deletejob(jobs, pid);				   // 从任务列表中删除相应jobs
			sigprocmask(SIG_SETMASK, &prev, NULL); // 解除信号阻塞，还原blocked的原始情况
		}
		else if (WIFSTOPPED(status)){ // 引起返回的子进程当前是停止的
			printf("Job [%d] (%d) stopped by signal %d\n", pid2jid(pid), pid, WSTOPSIG(status));
			getjobpid(jobs, pid)->state = ST; // 将该进程的状态改为挂起
		}
	}
	errno = olderrno; // 恢复errno
	return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig)
{
	int olderrno = errno; // 保存原始errno
	sigset_t mask, prev;

	sigfillset(&mask);					  // 将所有信号都添加到mask中
	sigprocmask(SIG_BLOCK, &mask, &prev); // 阻塞所有信号，完成后，blocked中所有信号均阻塞，prev中保存着blocked的原始情况

	pid_t pid = fgpid(jobs); // 寻找对应进程
	if (pid)
		if (kill(-pid, SIGINT) < 0) // 给对应进程发送SIGINT信号，若出错则输出提示
			unix_error("kill error\n");

	sigprocmask(SIG_SETMASK, &prev, NULL); // 解除信号阻塞，还原blocked的原始情况
	errno = olderrno;					   // 恢复errno
	return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig)
{
	int olderrno = errno; // 保存原始errno
	sigset_t mask, prev;

	sigfillset(&mask);					  // 将所有信号都添加到mask中
	sigprocmask(SIG_BLOCK, &mask, &prev); // 阻塞所有信号，完成后，blocked中所有信号均阻塞，prev中保存着blocked的原始情况

	pid_t pid = fgpid(jobs); // 寻找对应进程
	if (pid)
		if (kill(-pid, SIGTSTP) < 0) // 给对应进程发送SIGTSTP信号，若出错则输出提示
			unix_error("kill error\n");

	sigprocmask(SIG_SETMASK, &prev, NULL); // 解除信号阻塞，还原blocked的原始情况
	errno = olderrno;					   // 恢复errno
	return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job)
{
	job->pid = 0;
	job->jid = 0;
	job->state = UNDEF;
	job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs)
{
	int i;

	for (i = 0; i < MAXJOBS; i++)
		clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs)
{
	int i, max = 0;

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

	for (i = 0; i < MAXJOBS; i++)
	{
		if (jobs[i].pid == 0)
		{
			jobs[i].pid = pid;
			jobs[i].state = state;
			jobs[i].jid = nextjid++;
			if (nextjid > MAXJOBS)
				nextjid = 1;
			strcpy(jobs[i].cmdline, cmdline);
			if (verbose)
			{
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

	for (i = 0; i < MAXJOBS; i++)
	{
		if (jobs[i].pid == pid)
		{
			clearjob(&jobs[i]);
			nextjid = maxjid(jobs) + 1;
			return 1;
		}
	}
	return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs)
{
	int i;

	for (i = 0; i < MAXJOBS; i++)
		if (jobs[i].state == FG)
			return jobs[i].pid;
	return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid)
{
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
		if (jobs[i].pid == pid)
		{
			return jobs[i].jid;
		}
	return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs)
{
	int i;

	for (i = 0; i < MAXJOBS; i++)
	{
		if (jobs[i].pid != 0)
		{
			printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
			switch (jobs[i].state)
			{
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
