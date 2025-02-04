#include <stdio.h>

int main()
{
    int x = 5;
    int *ptr = &x;
    int **second_ptr = &ptr;
    printf("Value of x = %d (Using first pointer)\n", *ptr);
    printf("Value of x = %d (Using second pointer)\n", **second_ptr);
}
