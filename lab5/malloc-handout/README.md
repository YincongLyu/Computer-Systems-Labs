## note

### Envoronments

1. 解压缩后，确保有正确的运行环境：

   1. 修改Makefile文件里的gcc编译选项`CFLAGS = -Wall -g -O2 -m32`为`CFLAGS = -Wall -O3 -g -DDRIVER -std=gnu99 -Wno-unused-function -Wno-unused-parameter`很多情况下，你的代码需要 debug。

   2. `make`

   3. 执行`./mdriver -f short1-bal.rep`看是否有结果输出, 例如:

      `Perf index = 30 (util) + 40 (thru) = 70/100` 【第一项util是空间利用率，第二项thru是吞吐量】

2. 本地自测过程【提供两种性能分析工具`valgrind`和`gprof`，会有优化提示】

   - `./mdriver`的参数在pdf里有详细说明

   - `valgrind ./mdriver -f short1-bal.rep `

   - 使用`gprof`工具如下：
     1. 需要修改`Makefile`里`gcc`选项，改`-g`为`-pg` 【体验过之后，可以改回来】
     2. 运行` ./mdriver -f short1-bal.rep `    会在目录下生成一个`gmon.out`文件
     3. 运行`gprof ./mdriver`查看结果

3. 测试方法

```shell
./mdriver -v -t ./traces
```

同学们需要根据课程上讲过的内容实现: mm_init, mm_malloc, mm_free, mm_realloc 函数，并且通过上面的测试，最后的 perf 分数越高，实验的得分也会越高。good luck :)

### submit

1. 源码：只需提交你修改的`mm.c`文件（需重新命名为：**学号+mm.c**）到`github`的`lab5/malloc-handout`文件夹下。

1. 报告：提交到邮箱`csapp2022ecnu@163.com`

3. **截至时间**：2022-12-31 23:59【本次实验不需要pair，TA会详细看报告的good luck:-)】有特殊情况的同学可以和我们说明
