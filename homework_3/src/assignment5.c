#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 1000000

void scalar_toupper(char *input, char *output)
{
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
        {
            output[i] = (char)input[i] - 32;
        }
        else
        {
            output[i] = input[i];
        }
    }
}

void simd_toupper(char *input, char *output)
{
    int len = strlen(input);
    int i;
    __m256i lower_a = _mm256_set1_epi8('a' - 1);
    __m256i lower_z = _mm256_set1_epi8('z');
    __m256i case_mask = _mm256_set1_epi8(0x20);

    for (i = 0; i + 32 <= len; i += 32)
    {
        __m256i data = _mm256_loadu_si256((__m256i *)&input[i]);
        __m256i is_lower = _mm256_and_si256(_mm256_cmpgt_epi8(data, lower_a), _mm256_cmpgt_epi8(lower_z, data));
        __m256i upper_data = _mm256_xor_si256(data, _mm256_and_si256(is_lower, case_mask));
        _mm256_storeu_si256((__m256i *)&output[i], upper_data);
    }

    while (i < len)
    {
        if (input[i] >= 'a' && input[i] <= 'z')
        {
            output[i] = (char)input[i] - 32;
        }
        else
        {
            output[i] = input[i];
        }
        i++;
    }
}

int main()
{
    srand(time(0));
    char *input = (char *)malloc(SIZE * sizeof(char));
    char *output_scalar = (char *)malloc(SIZE * sizeof(char));
    char *output_simd = (char *)malloc(SIZE * sizeof(char));
    clock_t start, end;

    if (input == NULL || output_scalar == NULL || output_simd == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < SIZE - 1; i++)
    {
        input[i] = (char)(rand() % ('~' - ' ' + 1) + ' ');
    }

    start = clock();
    scalar_toupper(input, output_scalar);
    end = clock();
    printf("Scalar convertion time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    start = clock();
    simd_toupper(input, output_simd);
    end = clock();
    printf("SIMD convertion time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    free(output_scalar);
    free(output_simd);
    input = NULL;
    output_scalar = NULL;
    output_simd = NULL;

    return 0;
}
