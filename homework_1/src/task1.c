#include <stdio.h>

int main()
{
    int x = 5;
    int *ptr = &x;
    printf("Value of x = %d and Address of x = %p\n", x, ptr);
    *ptr = 10;
    printf("Value of x = %d and Address of x = %p\n", x, ptr);
}
