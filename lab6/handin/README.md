# note

1. 需在`tsh.c`完成 `eval`、`builtin_cmd`、`do_bgfg`、`waitfg`、`sigchld_handler`、`sigint_handler`、`sigtstp_handler`七个函数的功能。
2. 自测
   - 每完成`tsh.c`的一个功能，重新make后，进入`./tsh`，测试功能是否正常。具体用来测试的语句可参考`tracexx.txt`文件。
   - 执行`make test01` ，比对与给定的参考程序`tshref`执行`make rtest01`的输出是否一致，依次检查完16个trace文件。

# submit

1. 源码：只需提交你的`tsh.c`文件（需重新命名为：**学号**+tsh.c）到`github`的`lab6/handin`文件夹下。
2. 报告：提交到邮箱`csapp2022ecnu@163.com`。报告内容需至少包含以下六个函数`eval`、`do_bgfg`、`sigchld_handler`、`sigchld_handler`、`sigint_handler`、`sigtstp_handler`中的三个，如设计过程、遇到的bug和收获到的知识点。以及运行`trace15.txt`的结果
3. 截至时间：2023-02-01 23.59。
