#include <stdio.h>

void swap(int *a, int *b)
{
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

int main()
{
    int a = 5, b = 10;
    printf("Value of a = %d and value of b = %d\n", a, b);
    swap(&a, &b);
    printf("Value of a = %d and value of b = %d\n", a, b);
}
