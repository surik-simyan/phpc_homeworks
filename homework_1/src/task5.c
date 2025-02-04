#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *arr, *num;

    num = (int *)malloc(sizeof(int));
    if (num == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    *num = 5;
    printf("Number is: %d\n", *num);

    arr = (int *)malloc(5 * sizeof(int));
    if (arr == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        *(arr + i) = i * i;
    }

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", *(arr + i));
    }
    printf("\n");

    free(num);
    free(arr);
    num = NULL;
    arr = NULL;
    return 0;
}
