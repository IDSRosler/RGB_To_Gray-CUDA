#include <cuda.h>
#include <cuda_runtime.h>

#include "gray_img_CUDA.cuh"

__global__ void gray_CUDA(unsigned char* orig, unsigned char* gray, int width, int height, int channels){
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;

    if (x < width && y < height) {
        int grayOffset = y * width + x;
        int rgbOffset = grayOffset * channels;
        unsigned char r = orig[rgbOffset]; // red value for pixel
        unsigned char g = orig[rgbOffset + 1]; // green value for pixel
        unsigned char b = orig[rgbOffset + 2]; // blue value for pixel

        gray[rgbOffset] = 0.21f*r + 0.71f*g + 0.07f*b;
        gray[rgbOffset + 1] = 0.21f*r + 0.71f*g + 0.07f*b;
        gray[rgbOffset + 2] = 0.21f*r + 0.71f*g + 0.07f*b;
    }
}

void grayParallel(unsigned char* orig, unsigned char* gray, int height, int width, int channels){
    unsigned char* gpu_data, *gray_data;

    //gpu memory allocate
    cudaMalloc((void **) &gpu_data, height * width * channels * sizeof(unsigned char));
    cudaMalloc((void **) &gray_data, height * width * channels * sizeof(unsigned char));

    //copy data from CPU to GPU
    cudaMemcpy(gpu_data, orig, height * width * channels * sizeof(unsigned char), cudaMemcpyHostToDevice);

    dim3 blockSize( (width-1)/16 + 1, (height-1)/16 + 1, 1);
    dim3 gridSize( 16, 16, 1);

    gray_CUDA<<<blockSize,gridSize>>>(gpu_data, gray_data, width, height, channels);

    //copy data from GPU to CPU
    cudaMemcpy(gray, gray_data, height * width * channels * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    //free GPU memory
    cudaFree(gpu_data);
    cudaFree(gray_data);
}