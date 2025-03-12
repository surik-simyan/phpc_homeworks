#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10000

void scalar_multiplication(float *vector, float (*matrix)[SIZE], float *output)
{
    for (int i = 0; i < SIZE; i++)
    {
        float tempMultiplication = 0.0f;
        for (int k = 0; k < SIZE; k++)
        {
            tempMultiplication += vector[k] * matrix[i][k];
        }
        output[i] = tempMultiplication;
    }
}

void simd_multiplication(float *vector, float (*matrix)[SIZE], float *output)
{
    for (int i = 0; i < SIZE; i++)
    {
        __m256 sum_vec = _mm256_setzero_ps();
        for (int k = 0; k < SIZE; k += 8)
        {
            __m256 vec = _mm256_loadu_ps(&vector[k]);
            __m256 mat = _mm256_loadu_ps(&matrix[i][k]);
            __m256 mul = _mm256_mul_ps(vec, mat);
            sum_vec = _mm256_add_ps(sum_vec, mul);
        }

        float sum_array[8];
        _mm256_storeu_ps(sum_array, sum_vec);

        float sum = sum_array[0] + sum_array[1] + sum_array[2] + sum_array[3] + sum_array[4] + sum_array[5] + sum_array[6] + sum_array[7];
        output[i] = sum;
    }
}

int main()
{
    srand(time(0));
    float *vector = (float *)malloc(SIZE * sizeof(float));
    float(*matrix)[SIZE] = malloc(sizeof(float[SIZE][SIZE]));
    float *output_scalar = (float *)malloc(SIZE * sizeof(float));
    float *output_simd = (float *)malloc(SIZE * sizeof(float));
    clock_t start, end;

    if (vector == NULL || matrix == NULL || output_scalar == NULL || output_simd == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        vector[i] = (float)rand() / (float)rand();
        for (int j = 0; j < SIZE; j++)
        {
            matrix[i][j] = (float)rand() / (float)rand();
        }
    }

    start = clock();
    scalar_multiplication(vector, matrix, output_scalar);
    end = clock();
    printf("Scalar matrix multiplication time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    start = clock();
    simd_multiplication(vector, matrix, output_simd);
    end = clock();
    printf("SIMD matrix multiplication time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    free(vector);
    free(matrix);
    free(output_scalar);
    free(output_simd);
    vector = NULL;
    matrix = NULL;
    output_scalar = NULL;
    output_simd = NULL;

    return 0;
}
