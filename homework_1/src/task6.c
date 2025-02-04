#include <stdio.h>
#include <stdlib.h>

int str_length(char *str)
{
    int length = 0;
    for (char *c = str; *c != '\0'; c++)
    {
        length++;
    }
    return length;
}

int main()
{
    char *literal = "Lorem Ipsum";
    for (char *c = literal; *c != '\0'; c++)
    {
        printf("%c", *c);
    }
    printf("\n");
    printf("Length of string is: %d\n", str_length(literal));

    return 0;
}
