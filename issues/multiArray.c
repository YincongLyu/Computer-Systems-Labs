#include <stdio.h>

int vec_ele(int n, int *A, int i, int j){
    return A[i * n + j];
}

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
