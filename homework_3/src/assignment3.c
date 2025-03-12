#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 30000
#define BLOCK_SIZE 8

void scalar_prefix_sum(int *input, int *output)
{
    output[0] = input[0];
    for (int i = 1; i < SIZE; i++)
    {
        output[i] = output[i - 1] + input[i];
    }
}

void simd_prefix_sum(int *input, int *output)
{

    int i = 0;
    __m256i prev_sum = _mm256_setzero_si256();

    for (; i <= SIZE - 8; i += 8)
    {
        __m256i current = _mm256_loadu_si256((__m256i *)&input[i]);

        current = _mm256_add_epi32(current, _mm256_slli_si256(current, 4));
        current = _mm256_add_epi32(current, _mm256_slli_si256(current, 8));
        current = _mm256_add_epi32(current, _mm256_slli_si256(current, 12));

        current = _mm256_add_epi32(current, prev_sum);

        _mm256_storeu_si256((__m256i *)&output[i], current);

        prev_sum = _mm256_set1_epi32(output[i + 7]);
    }

    for (; i < SIZE; i++)
    {
        output[i] = output[i - 1] + input[i];
    }
}

int main()
{
    srand(time(0));
    int *input = (int *)malloc(SIZE * sizeof(int));
    int *output_scalar = (int *)malloc(SIZE * sizeof(int));
    int *output_simd = (int *)malloc(SIZE * sizeof(int));
    clock_t start, end;

    if (input == NULL || output_scalar == NULL || output_simd == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        input[i] = (float)rand() / (float)rand();
    }

    start = clock();
    scalar_prefix_sum(input, output_scalar);
    end = clock();
    printf("Scalar prefix sum time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    start = clock();
    simd_prefix_sum(input, output_simd);
    end = clock();
    printf("SIMD prefix sum time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    free(input);
    free(output_scalar);
    free(output_simd);
    input = NULL;
    output_scalar = NULL;
    output_simd = NULL;

    return 0;
}
