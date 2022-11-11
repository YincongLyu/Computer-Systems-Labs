### VLA 支持

#### C、C++ 标准

关于VLA（栈上变长数组）这个问题，其实挺tricky的，先说结论：

1. VLA在 C++标准（C++11、14、17、20）中很明确不支持，是错误的feature，虽然好几次标准委员会（比如C++14）曾反复讨论，最后的结果都是明确不支持。
2. VLA 在C99得到明确支持（之前的C89标准是不支持的），但是后来因为众多反对声音（包括Linux巨大的社群），到C11又变成了optional（可选支持）。


但针对这个结论，有些同学可能不解，不是C++标准都支持C语言标准吗？ 这句话其实不精确。 或者可以说有很多灰色空间。 谈到支持C语言，一定要谈支持C语言的那个标准。

比如C++之父Bjarne也明确说过，C++ 11支持几乎所有的C99标准，唯独VLA 和 指定初始化这两个feature明确不支持。Bjarne也谈过C++标准委员会 和C语言标准委员会，两者很多理念不一样，经常有分歧。VLA就是最大的一个分歧点:) 

https://en.cppreference.com/w/cpp/language/array

#### 编译器支持

上面说完标准支持问题，再来说下编译器支持。

编译器对于标准的支持，并不是 Yes 所有feature我都支持；或者 No 所有feature 我都不支持。比如某个编译器声称支持C++20标准，并不是所有C++ 20的feature，某个编译器都支持。 

对于VLA来说， GNU官方说明支持， 但是又说明G++不支持VLA（参考这里：https://gcc.gnu.org/gcc-5/changes.html#cxx）

还有微软的 MSVC 也一直很明确 ，坚决不支持 VLA（参考这里：https://devblogs.microsoft.com/cppblog/c11-and-c17-standard-support-arriving-in-msvc/）

简单的结论是，VLA 显然是不能得到所有编译器支持的。

至于VLA为什么得到这么多的抵制（特别是C++社区，虽然C语言社区对这个也挺分裂的）、无非是极不安全、性能也差，有更好的其他替代品。 简单说属于万恶之源类的feature。
