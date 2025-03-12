#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

float scl_Max(float *arr)
{
    float maxValue = arr[0];
    for (int i = 1; i < SIZE; i++)
    {
        if (arr[i] > maxValue)
        {
            maxValue = arr[i];
        }
    }
    return maxValue;
}

float simd_Max(float *arr)
{
    __m256 maxValues = _mm256_loadu_ps(arr);

    for (int i = 8; i < SIZE; i += 8)
    {
        __m256 currentValues = _mm256_loadu_ps(arr + i);
        maxValues = _mm256_max_ps(maxValues, currentValues);
    }

    float temp[8];
    _mm256_storeu_ps(temp, maxValues);
    float maxValue = temp[0];
    for (int i = 1; i < 8; i++)
    {
        if (temp[i] > maxValue)
        {
            maxValue = temp[i];
        }
    }
    return maxValue;
}

int main()
{
    srand(time(0));
    float arr[SIZE], scalar_max, simd_max;
    clock_t start, end;

    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = (float)rand() / (float)rand();
    }

    start = clock();
    scalar_max = scl_Max(arr);
    end = clock();
    printf("Scalar Max Value: %f | Time: %f seconds\n", scalar_max,
           (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    simd_max = simd_Max(arr);
    end = clock();
    printf("SIMD Max Value: %f | Time: %f seconds\n", simd_max,
           (double)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
