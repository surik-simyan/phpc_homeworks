#include <omp.h>
#include <stdio.h>

long long fib(int n)
{
    if (n <= 1)
    {
        return n;
    }

    long long x, y;

    if (n <= 10)
    {
        x = fib(n - 1);
        y = fib(n - 2);
    }
    else
    {
        #pragma omp task shared(x)
        x = fib(n - 1);

        #pragma omp task shared(y)
        y = fib(n - 2);

        #pragma omp taskwait
    }

    return x + y;
}

int main()
{
    int n;

    printf("Enter number: ");
    scanf("%d", &n);

    if (n < 0)
    {
        printf("Please enter valid number\n");
        return 1;
    }

    long long result;

    #pragma omp parallel
    {
        #pragma omp single
        {
            result = fib(n);
        }
    }

    printf("Fibonacci number at position %d is equal to %lld\n", n, result);

    return 0;
}
