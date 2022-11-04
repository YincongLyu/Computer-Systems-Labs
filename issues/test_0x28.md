## <center>mov %fs:0x28,%rax 到底做了什么?</center>

我们试图通过用最简单的例子复现大家都有疑问的一条指令  `mov %fs:0x28,%rax`. 实际
上它是为了判断栈溢出。我们将下述 c 代码保存为 `test_0x28.c`.

```c
#include <stdio.h>

int main(){
    int test_0x28[2];
    test_0x28[0] = 123;
    test_0x28[1] = 345;
    return 0;
}
```

通过以下指令得到汇编

```shell
gcc test_0x28.c
objdump -MATT -d a.out > test_0x28.asm  # 指定生成 AT&T 格式汇编 -MIntel 将生成
                                        # intel 格式汇编，二者有区别，我们的实验
                                        # 全部使用 AT&T 格式汇编
```

然后我们打开 testi\_0x28.asm 找到下面一段代码 (main函数 代码)

```asm
0000000000001149 <main>:
    1149:	f3 0f 1e fa          	endbr64
    114d:	55                   	push   %rbp
    114e:	48 89 e5             	mov    %rsp,%rbp
    1151:	48 83 ec 10          	sub    $0x10,%rsp
    1155:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    115c:	00 00
    115e:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    1162:	31 c0                	xor    %eax,%eax
    1164:	c7 45 f0 7b 00 00 00 	movl   $0x7b,-0x10(%rbp)
    116b:	c7 45 f4 59 01 00 00 	movl   $0x159,-0xc(%rbp)
    1172:	b8 00 00 00 00       	mov    $0x0,%eax
    1177:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    117b:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx
    1182:	00 00
    1184:	74 05                	je     118b <main+0x42>
    1186:	e8 c5 fe ff ff       	callq  1050 <__stack_chk_fail@plt>
    118b:	c9                   	leaveq
    118c:	c3                   	retq
    118d:	0f 1f 00             	nopl   (%rax)
```

这是其 main 函数汇编代码。我们来讲解这一段代码.

1. 开辟栈空间

```shell
push %rbp
mov  %rsp, %rbp
sub $0x10, %rsp 相当于开辟了一个大小为 16 的栈空间
```

这三条指令实际上每次函数调用都会有，我们说每个函数的栈空间都是被 rbp -> rsp 唯一
标识，也就是:

```shell
----------  rbp = rsp
    /|\
     |
     |
上述 c 程序的栈空间
     |
     |
    \|/
----------  rsp - 16
```

2. 设置栈溢出保护

```shell
mov    %fs:0x28,%rax
mov    %rax,-0x8(%rbp)
```

3. 填充栈空间

在这个操作之后，现在的栈实际上如下,右边是栈的地址，左边是栈保存的值:

```shell
Content                              Address
------------
  %rbp                            |   %rbp
------------                      |
  %rax                            |   %rbp - 0x8
------------                      |
  test_0x28[1] = 123              |   %rbp - 0xc
------------                      |
  test_0x28[0] = 456              |   %rbp - 0x10
------------
```

我们注意到程序到这里并没有结束，而是执行了下面的指令:

```asm
mov    -0x8(%rbp),%rdx # 将上面 %rbp - 0x8 的值取出来
xor    %fs:0x28,%rdx   # 检查这个值是不是发生了改变 %fs:0x28 受操作系统保护，
                       # 我们不能读取，只能操作系统拿来做判断
je     118b <main+0x42> # 如果上述两个值相等，也就意味着栈没有被冲掉，那么跳转到
                        # 118b 也就是 leaveq 指令后结束
callq  1050 <__stack_chk_fail@plt> # 如果栈被冲掉将调用该函数做栈溢出处理，程序将报错退出
```

说完上面理论之后，我们通过调试下面代码，看看到底是不是和上述理论一致？

```c
#include <stdio.h>

int main(){
    int test_0x28[2];
    test_0x28[0] = 123;
    test_0x28[1] = 345;
    test_0x28[2] = 567;  # 这里实际上访问了非法的栈空间
    return 0;
}
```

编译上述代码:

```shell
gcc -g test_0x28.c
gdb a.out
```
进入到 gdb 之后，输入下面命令

```shell
b main
run
layout asm
ni  # 一直按 ni
```

我们会看到 `callq  <__stack_chk_fail@plt>` 被调用。

我们在上述汇编代码中看到了下面的一条指令：

```shell
movl  $0x237,-0x8(%rbp)
```

0x237 就是十进制的 567, 我们发现它重写了 -0x8(%rbp) 位置的值. 翻看前面的指令，我们发现：

```ASM
mov    %fs:0x28,%rax
mov    %rax,-0x8(%rbp)
```
-0x8(%rbp) 原先存储 fs:0x28 的保护值，由于非法栈访问，导致它的值被冲掉了.
这是判断栈溢出的办法，相信通过这篇文章，大家应该了解了 fs:0x28 的作用。
