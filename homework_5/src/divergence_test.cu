#include <stdio.h>
#include <cuda_runtime.h>

__global__ void warmupKernel(int* data)
{
	int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (true) {
        data[idx] *= 2;
    }
}

__global__ void no_divergence(int* data)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (true) {
        data[idx] *= 2;
    }
}

__global__ void divergence(int* data)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx % 2 == 0)
    {
      data[idx] *= 2;
    }
    else
    {
      data[idx] *= 3;
    }
}

__global__ void small_divergence(int* data)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if ((idx / warpSize) % 2 == 0)
    {
      data[idx] *= 2;
    }
    else
    {
      data[idx] *= 3;
    }
}

int main()
{

    int N = 1000000;
    size_t size = N * sizeof(int);

    int *h_data = (int *)malloc(size);

    for (int i = 0; i < N; i++) {
        h_data[i] = i + 1;
    }

    int *d_data;
    cudaMalloc(&d_data, size);

    int blockSize = 256;
    int numBlocks = (N + blockSize - 1) / blockSize;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float milliseconds = 0;

    cudaMemcpy(d_data, h_data, size, cudaMemcpyHostToDevice);
    warmupKernel<<<numBlocks, blockSize>>>(d_data);
    cudaDeviceSynchronize();

    cudaMemcpy(d_data, h_data, size, cudaMemcpyHostToDevice);
    cudaEventRecord(start);
    no_divergence<<<numBlocks, blockSize>>>(d_data);
    cudaDeviceSynchronize();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    printf("No divergence kernel execution time: %.3f ms\n", milliseconds);


    cudaMemcpy(d_data, h_data, size, cudaMemcpyHostToDevice);
    cudaEventRecord(start);
    divergence<<<numBlocks, blockSize>>>(d_data);
    cudaDeviceSynchronize();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    printf("Divergence kernel execution time: %.3f ms\n", milliseconds);


    cudaMemcpy(d_data, h_data, size, cudaMemcpyHostToDevice);
    cudaEventRecord(start);
    small_divergence<<<numBlocks, blockSize>>>(d_data);
    cudaDeviceSynchronize();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&milliseconds, start, stop);
    printf("Small divergence kernel execution time: %.3f ms\n", milliseconds);

    free(h_data);
    cudaFree(d_data);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}
