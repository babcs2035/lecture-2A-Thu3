// 四則演算のプログラム
#include <stdio.h>
#include "calc.h"

int main()
{
    double a = 1.2;
    double b = 1.4;

    printf("a+b: %f\n", add(a, b));
    printf("a-b: %f\n", sub(a, b));
    printf("a*b: %f\n", mul(a, b));
    printf("a/b: %f\n", div(a, b));
    return 0;
}
