#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

void convert_to_grayscale_scalar(FILE *input, FILE *output)
{
    fseek(input, 54, SEEK_SET);
    fseek(output, 54, SEEK_SET);

    uint8_t pixelData[3];
    while (fread(pixelData, sizeof(uint8_t), 3, input) == 3)
    {
        uint8_t gray = (uint8_t)(0.299 * pixelData[2] + 0.587 * pixelData[1] + 0.114 * pixelData[0]);
        pixelData[0] = pixelData[1] = pixelData[2] = gray;
        fwrite(pixelData, sizeof(uint8_t), 3, output);
    }
}

void convert_to_grayscale_simd(FILE *input, FILE *output)
{
    fseek(input, 54, SEEK_SET);
    fseek(output, 54, SEEK_SET);

    __m256 red_weight = _mm256_set1_ps(0.299f);
    __m256 green_weight = _mm256_set1_ps(0.587f);
    __m256 blue_weight = _mm256_set1_ps(0.114f);

    uint8_t pixelData[24];
    while (fread(pixelData, sizeof(uint8_t), 24, input) == 24)
    {
        float r[8], g[8], b[8];
        for (int k = 0; k < 8; k++)
        {
            b[k] = pixelData[k * 3];
            g[k] = pixelData[k * 3 + 1];
            r[k] = pixelData[k * 3 + 2];
        }

        __m256 r_float = _mm256_loadu_ps(r);
        __m256 g_float = _mm256_loadu_ps(g);
        __m256 b_float = _mm256_loadu_ps(b);

        __m256 gray_float =
            _mm256_add_ps(_mm256_add_ps(
                _mm256_mul_ps(r_float, red_weight),
                _mm256_mul_ps(g_float, green_weight)),
                _mm256_mul_ps(b_float, blue_weight)
            );

        float gray_vals[8];
        _mm256_storeu_ps(gray_vals, gray_float);

        for (int k = 0; k < 8; k++)
        {
            uint8_t gray = (uint8_t)gray_vals[k];
            pixelData[k * 3] = pixelData[k * 3 + 1] = pixelData[k * 3 + 2] = gray;
        }
        fwrite(pixelData, sizeof(uint8_t), 24, output);
    }
}

int main()
{
    FILE *input = fopen("../src/Lenna_(test_image).bmp", "rb");
    FILE *output_scalar = fopen("Lenna_(test_image)_grayscale_scalar.bmp", "wb");
    FILE *output_simd = fopen("Lenna_(test_image)_grayscale_simd.bmp", "wb");

    if (!input || !output_scalar || !output_simd)
    {
        printf("Error: Could not open files.\n");
        return 1;
    }

    uint8_t header[54];
    fread(header, sizeof(uint8_t), 54, input);
    fwrite(header, sizeof(uint8_t), 54, output_scalar);
    fwrite(header, sizeof(uint8_t), 54, output_simd);

    rewind(input);

    clock_t start = clock();
    convert_to_grayscale_scalar(input, output_scalar);
    clock_t end = clock();
    printf("Scalar conversion time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    rewind(input);

    start = clock();
    convert_to_grayscale_simd(input, output_simd);
    end = clock();
    printf("SIMD conversion time: %f seconds\n",
           ((double)(end - start)) / CLOCKS_PER_SEC);

    fclose(input);
    fclose(output_scalar);
    fclose(output_simd);

    return 0;
}
