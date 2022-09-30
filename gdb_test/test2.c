#include <stdio.h>

void print(){
        for(int i = 0; i < 5; ++i){
                printf("i = %d \n", i);
        }
        return;
}

int main(){
        for(int j = 0; j < 10; ++j)
                printf("j = %d \n", j);
        printf("-----call function----- \n");
        print();
        return 0;
}
