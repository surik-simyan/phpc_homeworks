#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100000000
#define FILTER_SIZE 3

void scalar_convolution(float *input, float *output, float *filter, int input_size)
{
    for (int i = 1; i < input_size - 1; i++)
    {
        output[i] = input[i - 1] * filter[0] + input[i] * filter[1] + input[i + 1] * filter[2];
    }

    output[0] = input[0] * filter[1] + input[1] * filter[2];
    output[input_size - 1] = input[input_size - 2] * filter[0] + input[input_size - 1] * filter[1];
}

void simd_convolution(float *input, float *output, float *filter, int input_size)
{
    __m256 w1 = _mm256_set1_ps(filter[0]);
    __m256 w2 = _mm256_set1_ps(filter[1]);
    __m256 w3 = _mm256_set1_ps(filter[2]);

    for (int i = 1; i < input_size - 8; i += 8)
    {
        __m256 prev = _mm256_loadu_ps(&input[i - 1]);
        __m256 curr = _mm256_loadu_ps(&input[i]);
        __m256 next = _mm256_loadu_ps(&input[i + 1]);

        __m256 result = _mm256_mul_ps(prev, w1);
        result = _mm256_add_ps(result, _mm256_mul_ps(curr, w2));
        result = _mm256_add_ps(result, _mm256_mul_ps(next, w3));

        _mm256_storeu_ps(&output[i], result);
    }

    output[0] = input[0] * filter[1] + input[1] * filter[2];

    for (int i = input_size - 8; i < input_size - 1; i++)
    {
        output[i] = input[i - 1] * filter[0] + input[i] * filter[1] + input[i + 1] * filter[2];
    }

    output[input_size - 1] = input[input_size - 2] * filter[0] + input[input_size - 1] * filter[1];
}

int main()
{
    srand(time(0));
    float *input = (float *)malloc(SIZE * sizeof(float));
    float *output_scalar = (float *)malloc(SIZE * sizeof(float));
    float *output_simd = (float *)malloc(SIZE * sizeof(float));

    if (input == NULL || output_scalar == NULL || output_simd == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    float filter[FILTER_SIZE] = {0.25f, 0.5f, 0.25f};
    clock_t start, end;

    for (int i = 0; i < SIZE; i++)
    {
        input[i] = (float)rand() / (float)rand();
    }

    start = clock();
    scalar_convolution(input, output_scalar, filter, SIZE);
    end = clock();
    printf("Scalar convolution time: %f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    simd_convolution(input, output_simd, filter, SIZE);
    end = clock();
    printf("SIMD convolution time: %f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    free(input);
    free(output_scalar);
    free(output_simd);
    input = NULL;
    output_scalar = NULL;
    output_simd = NULL;

    return 0;
}
