#include <stdio.h>

int main()
{
    int arr[5] = {1, 2, 3, 4, 5};
    int *ptr = arr;
    for (int i = 0; i < 5; i++)
    {
        printf("Value of %dth element = %d\n", i, *(ptr + i));
    }
    for (int i = 0; i < 5; i++)
    {
        *(ptr + i) = -*(ptr + i);
    }
    printf("Print using pointer\n");
    for (int i = 0; i < 5; i++)
    {
        printf("Value of %dth element = %d\n", i, *(ptr + i));
    }
    printf("Print using array name\n");
    for (int i = 0; i < 5; i++)
    {
        printf("Value of %dth element = %d\n", i, arr[i]);
    }
}
