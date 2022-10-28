## <center>Attack Lab 作业提交说明</center>

### 作业说明

Attack Lab 共有 5 个 phase，其中前 4 个 phase 必选，第 5 个 phase 选做 （暂不计入成绩）。

所有同学按照[序号](https://docs.qq.com/sheet/DRWpBcHpscWFHb2NG?tab=7j3vho)选择题目，题目已经在 [github](https://github.com/zjpedu/Computer-Systems-Labs/tree/main/lab4/attack-handout) 上开放，作业测试的 [job](https://github.com/zjpedu/Computer-Systems-Labs/blob/main/.github/workflows/attacklab.yml) 也开放，大家可阅读其中的 `shell` 代码，这是我们检查作业的依据。

4 个 phase 的**答案命名且只能命名** 为 **answer1.txt、answer2.txt、answer3.txt、answer4.txt**，通过下述方法测试自己的答案是否正确:

```shell
cat answer1.txt | ./hex2raw | ./ctarget -q
```

答案返回应该包含如下内容:

```shell
Cookie: 0x59b997fa
Type string:Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
        user id 100
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:1:11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 C0 17 40
```

> 返回中包含 PASS 字样，说明该 phase 通过检查，其它内容每个人不同，请大家注意。

其他 phase 测试代码如下:

```shell
cat answer2.txt | ./hex2raw | ./ctarget -q
cat answer3.txt | ./hex2raw | ./ctarget -q
cat answer4.txt | ./hex2raw | ./rtarget -q
```

### 作业提交内容

1. 请大家各自 `sync fork` [我的仓库](https://github.com/zjpedu/Computer-Systems-Labs)，作业提交到各自序号对应的代码仓库中，并且**只提交 answer1.txt、answer2.txt、answer3.txt、answer4.txt**。

2. 每个人都需要和助教预约检查作业的时间，每人 15 分钟。

3. 提交一个文档到水杉对应的入口中。

### 答疑

我们固定答疑和检查作业时间及地点。

时间: **每周五15.00-17.00，**

地点: **腾讯会议号：646-8393-4548，**

同学们有问题就直接进来pair

> 大家尽量通过 [Discussions](https://github.com/zjpedu/Computer-Systems-Labs/discussions) 或 csapp 群内问问题，尽量不要通过私人微信问问题，避免大量重复的问题及回答。

### 截止日期

**2022-11-20 23:59** 截止日期过后，作业将不会被检查。
