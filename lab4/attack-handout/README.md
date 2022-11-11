## <center>Attack Lab 作业提交说明</center>

### 作业说明

Attack Lab 共有 5 个 phase，其中前 4 个 phase 必选，第 5 个 phase 选做 （暂不计入成绩）。

所有同学按照[序号](https://docs.qq.com/sheet/DRWpBcHpscWFHb2NG?tab=7j3vho)选择题目，题目已经在 [github](https://github.com/zjpedu/Computer-Systems-Labs/tree/main/lab4/attack-handout) 上开放，作业测试的 [job](https://github.com/zjpedu/Computer-Systems-Labs/blob/main/.github/workflows/attacklab.yml) 也开放，大家可阅读其中的 `shell` 代码，这是我们检查作业的依据。

> 请大家通过 gdb 调试，并且看 `run` 之后的汇编代码。举例如下:

```shell
gdb ctarget
run
disas getbuf
disas touch1
disas touch2
disas touch3
```

当你通过上述代码得到答案后，可以使用下述方法测试答案，以 phase_1 为例:

```shell
cat answer1.txt | ./hex2raw > answer1_raw1.txt
gdb ctarget -ex 'run -q < answer1_raw1.txt'
```

你将会在 gdb 的输出中看到 `PASS` 字样，此时说明结果正确，恭喜你可以继续探索其它的题目。如果不正确，请自行修改。


4 个 phase 的**答案命名且只能命名** 为 **answer1.txt、answer2.txt、answer3.txt、answer4.txt**，测试请大家好好看自己的作业测试 [job](https://github.com/zjpedu/Computer-Systems-Labs/blob/main/.github/workflows/attacklab.yml)

### 作业提交内容

1. 请大家各自 `sync fork` [我的仓库](https://github.com/zjpedu/Computer-Systems-Labs)，作业提交到各自序号对应的代码仓库中，并且**只提交 answer1.txt、answer2.txt、answer3.txt、answer4.txt**。

2. 每个人都需要和助教预约检查作业的时间，每人 15 分钟。

3. 提交一个文档到水杉对应的入口中。

### 答疑

我们固定答疑和检查作业时间及地点。

时间: **每周五15.00-17.00，**

地点: **腾讯会议号 (偶数)：646-8393-4548，** **(奇数)：844-6381-7375**

同学们有问题就直接进来pair

> 大家尽量通过 [Discussions](https://github.com/zjpedu/Computer-Systems-Labs/discussions) 或 csapp 群内问问题，尽量不要通过私人微信问问题，避免大量重复的问题及回答。

### 截止日期

**2022-11-20 23:59** 截止日期过后，作业将不会被检查。
