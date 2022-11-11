#include <stdio.h>

// C 语言中的可变长度数组，这种可变长度数组在 C++ 原则上是不允许定义的
// C++ 只要你使用可变长度数组，那么一定都能用 vector 代替
// 这个特性是 C 和 C++ 最重要的一个区别
// 本质上，所有的类型转换实际上都是在告诉 CPU 如何取解析内存地址

int vec_ele(int n, int *A, int i, int j){
    return A[i * n + j];
}

// 如果你是一个多维数组，那么除了第一维，其他维度必须要给定
// 取 A 数组的第 i 行 j 列的元素
int var_ele(int n, int A[][n], int i, int j){
    return A[i][j];
}

int vec_ele_2(int n, int **A, int i, int j){
    return *((int*)(A) + i * n + j);
}

int main(){
    int n = 100;
    int a[n][n];
    for(int i =0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            a[i][j] = i+j;
    int res1 = vec_ele(n, (int*)a, 3, 5);
    int res2 = var_ele(n, a, 3, 5);
    int res3 = vec_ele_2(n, (int**)a, 3, 5);
    printf("res1 = %d\n", res1);
    printf("res2 = %d\n", res2);
    printf("res3 = %d\n", res3);
    return 0;
}
