#include "stdio.h"
int main(int argc,char *argv[]){// 参数名是argc和argv（可以取其他名字），但必须gcc编译后，在执行.o文件
    int a = 10; 
    int banana = 20;
    int * p = &banana;
    int apple = sizeof(int) * * p;//说明了p指针就是指针类型
    int orange = sizeof(int) * a;//不是强制类型，不是乘（除非没说明指针）
    printf("apple = %d\n",apple);
    printf("orange = %d\n",orange);
    printf("%s\n",argv[0]);//第0为是程序名本身
    printf("%s\n",argv[1]);
    printf("%s\n",argv[2]);
    //printf("%s\n",agv[1]);
    printf("%d\n",argc);
    return 0;
}
