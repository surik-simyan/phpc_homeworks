#include <stdio.h>
#include <stdlib.h>

int main()
{
    char **arr;
    arr = (char **)malloc(5 * sizeof(char *));

    if (arr == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        *(arr + i) = "This is template message";
    }

    printf("Before modification\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%s\n", *(arr + i));
    }

    arr[2] = "This is not template message";

    printf("\nAfter modification\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%s\n", *(arr + i));
    }

    free(arr);
    arr = NULL;
    return 0;
}
