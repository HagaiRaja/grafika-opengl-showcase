#include <iostream>
#include <fstream>
#include <numeric>
#include "cuda_runtime.h"
#include "cuda.h"
#include "curand_kernel.h"
#include "device_launch_parameters.h"
#include <time.h>
#define MAX_BLOCK_SIZE 1024
#define RNG_SEED 13516013
using namespace std;
void print(unsigned int * arr, int n) {
    for (int i = 0; i < n; i++)
        cout << i <<" :"<<arr[i] << " "<<endl;
    cout << endl;
}
__global__ void getMax(unsigned int * input, int n)
{
    const int tid = blockDim.x * blockIdx.x + threadIdx.x;
    auto step_size = 1;
    int number_of_threads = n / 2;
    while (number_of_threads > 0)
    {
        if (tid < number_of_threads) // still alive?
        {
            const auto fst = tid * step_size * 2;
            const auto snd = fst + step_size;
            if(input[fst] < input[snd]){
                input[fst] = input[snd];
            }
        }
        step_size <<= 1;
        number_of_threads >>= 1;
    }
    __syncthreads();
}
__global__ void storeCount(unsigned int * arr, int n, int * count, unsigned long long exp){
    const int tid = blockDim.x * blockIdx.x + threadIdx.x;
    int digit;
    if(tid < n){
        digit = (arr[tid] / exp) % 10;
        atomicAdd(&count[digit], 1);
    }
}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    curandGenerator_t curand_gen;
    cudaEvent_t start, stop;
    float time;
    int N;
    if (argc == 2) {
        N = strtol(argv[1], NULL, 10);
    } else {
        printf("ERROR: ./radix_sort <array_length>\n");
        return 1;
    }
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    curandCreateGenerator(&curand_gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(curand_gen, RNG_SEED);
    const long count = N;
    unsigned int h_arr[count];
    unsigned int * d_arr;
    int grid, block;
    cudaMalloc(&d_arr, count * sizeof(unsigned int));
//rng
    curandGenerate(curand_gen, d_arr, count);
    cudaDeviceSynchronize();
    cudaMemcpy(h_arr, d_arr, count * sizeof(int), cudaMemcpyDeviceToHost);
//find max
    if(count > MAX_BLOCK_SIZE){
        grid = count / (MAX_BLOCK_SIZE) + 1;
    }
    else{
        grid = 1;
    }
    block = MAX_BLOCK_SIZE / 2;
    getMax<<<grid, block >>>(d_arr, count);
    unsigned int global_max;
    cudaMemcpy(&global_max, d_arr, sizeof(int), cudaMemcpyDeviceToHost);
//sort
    cudaEventRecord(start, 0);
    cudaMemcpy(d_arr, h_arr, count * sizeof(unsigned int), cudaMemcpyHostToDevice);
    block *= 2;
    for (unsigned long long exp = 1; global_max / exp > 0; exp *= 10) {
        unsigned int output[count];
        int h_count[10] = {0};
        int * d_count;
        cudaMalloc(&d_count, sizeof(int) * 10);
        cudaMemset(d_count, 0, sizeof(int) * 10);
        storeCount<<<grid, block>>>(d_arr, count, d_count, exp);
        cudaMemcpy(h_count, d_count, 10 * sizeof(int), cudaMemcpyDeviceToHost);
        for (int i = 1; i < 10; i++) {
            h_count[i] += h_count[i - 1];
        }
        for (int i = count - 1; i >= 0; i--) {
            output[h_count[ (h_arr[i] / exp) % 10 ] - 1] = h_arr[i];
            h_count[ (h_arr[i] / exp) % 10 ]--;
        }
        cudaMemcpy(d_arr, output, sizeof(unsigned int) * count, cudaMemcpyHostToDevice);
        memcpy(h_arr, output, sizeof(unsigned int) * count);
        cudaFree(d_count);
        cudaDeviceSynchronize();
    }
    cudaMemcpy(h_arr, d_arr, count * sizeof(unsigned int), cudaMemcpyDeviceToHost);
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time, start, stop);
    print(h_arr,count);
    ofstream output("test/output.txt", std::ios::out | std::ios::trunc);
    for(int k = 0; k < count; k ++){
        output << k << ":"<<h_arr[k] << " " ;
    }
    output.close();
    cout << "Executed in " << time * 1000 << " ms" << endl;
    cudaFree(d_arr);
    curandDestroyGenerator(curand_gen);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    return 0;
}