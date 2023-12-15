#include "stdio.h"
int main()
{

int b = 2;
int a = 2;
switch (b)
{
    //int a = 5;//可以声明，没法赋值，不会被执行,但a的值变了，跳出switch之后a的值又变回来（a变成局部变量了）
    //a = 3;//不会被执行，case3可以给a赋值
case 2 :
    //int a = 5;
    b = 5;
case 3 :
    a = 5;
    //break;  
defau1t:
     a = 6;
    //break;
}
printf("a = %d, b = %d\n" ,a,b);
return 0;
}