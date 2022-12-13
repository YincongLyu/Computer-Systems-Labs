# note

1. 修改Makefile文件里的gcc编译选项`CFLAGS = -Wall -O2 -m32`为`CFLAGS = -Wall -Werror -O3 -g -DDRIVER -std=gnu99 -Wno-unused-function -Wno-unused-parameter`

2. 本地自测过程【提供两种性能分析工具`valgrind`和`gprof`，会有优化提示】

   - `./mdriver`的参数在pdf里有详细说明

   - `valgrind ./mdriver -f short1-bal.rep `

   - 使用`gprof`工具如下：
     1. 需要修改`Makefile`里`gcc`选项，改`-g`为`-pg` 【体验过之后，可以改回来】
     2. 运行` ./mdriver -f short1-bal.rep `    会在目录下生成一个`gmon.out`文件
     3. 运行`gprof ./mdriver`查看结果

3. 最后会有两个输出，第一项util是空间利用率，第二项thru是吞吐量

   `Perf index = 30 (util) + 40 (thru) = 70/100`

# submit

1. 源码：只需提交你修改的`mm.c`文件（需重新命名为：**学号+mm.c**）到`github`的`lab5/malloc-handout`文件夹下。

2. 报告：提交到邮箱`csapp2022ecnu@163.com`

3. **截至时间**：2022-12-31 23:59【本次实验不需要pair，TA会详细看报告的good luck:)】
