//
// Created by arrouan on 28/09/16.
//

//#include <stdint-gcc.h>
#include <stdint.h>
#include <random>
//#include <QTZlib/zlib.h>
#include <zlib.h>
#include "Common.h"
#include <omp.h>
#include <iostream>
/* Cuda stuff */
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <curand_kernel.h>

float Common::matrix_binding_[BINDING_MATRIX_SIZE*BINDING_MATRIX_SIZE];

__global__ 
void setup_gen(curandState *state, uint32_t seed, int size)
{
    int y = blockIdx.x*blockDim.x + threadIdx.x;
    int x = blockIdx.y*blockDim.y + threadIdx.y;
    int id = x*size+y;
    /* Each thread gets same seed, a different sequence 
       number, no offset */
    curand_init(seed, id, 0, &state[id]);
}

__global__ 
void proceed (float* matrix_binding, int size, int zperc,
                                curandState *state)
{
    float r;
    int y = blockIdx.x*blockDim.x + threadIdx.x;
    int x = blockIdx.y*blockDim.y + threadIdx.y;
    int id = x*size+y;
    /* Copy state to local memory for efficiency */
    curandState localState = state[id];
    /* Generate pseudo-random uniforms */
    if (curand_uniform(&localState) * 100.0f > zperc) {
        r = curand_uniform(&localState) * 2.0f - 1.0f;
    } else {
        r = 0;
    }
    /* Copy state back to global memory */
    state[id] = localState;
    /* Store results */
    matrix_binding[id] = r;
}

void Common::init_binding_matrix_gpu ( uint32_t seed)
{
    curandState *float_gen_;
    
    int floatGenSizeInBytes = BINDING_MATRIX_SIZE *BINDING_MATRIX_SIZE* sizeof(curandState);
    int ok = cudaMalloc((void**) &float_gen_, floatGenSizeInBytes );
    if(ok!=cudaSuccess){
            std::cout << "error cudaMalloc 0:" << ok << std::endl ;
            return;
    }

    float* gpuMatrixIn;
    int matrixSizeInBytes = BINDING_MATRIX_SIZE *BINDING_MATRIX_SIZE* sizeof(float);
    ok=cudaMalloc((void**) &gpuMatrixIn, matrixSizeInBytes );
    if(ok!=cudaSuccess){
            std::cout << "error cudaMalloc 1:" << ok << std::endl ;
            return;
    }

    dim3 dimBlock(32,32);
    dim3 dimGrid(BINDING_MATRIX_SIZE/dimBlock.x, BINDING_MATRIX_SIZE/dimBlock.y);

    setup_gen<<<dimGrid,dimBlock>>> (float_gen_, seed, BINDING_MATRIX_SIZE);
    proceed<<<dimGrid,dimBlock>>> ( gpuMatrixIn,BINDING_MATRIX_SIZE,
                  BINDING_MATRIX_ZERO_PERCENT,
                  float_gen_);

    ok = cudaMemcpy(matrix_binding_, gpuMatrixIn, matrixSizeInBytes,cudaMemcpyDeviceToHost);
    if(ok!=cudaSuccess){
            std::cout << "error cudaMemcpy 2:" << ok << std::endl ;
            return;
    }

    ok = cudaFree(float_gen_);
    if(ok!=cudaSuccess){
            std::cout << "error cudaFree 0:" << ok << std::endl ;
            return;
    }
    ok = cudaFree(gpuMatrixIn);
    if(ok!=cudaSuccess){
            std::cout << "error cudaFree 1:" << ok << std::endl ;
            return;
    }

}

void Common::init_binding_matrix(uint32_t seed) {

  std::mt19937 float_gen_;
  float_gen_.seed(seed);
  std::uniform_real_distribution<float> dis_number(-1, 1);
  std::uniform_int_distribution<int8_t> dis_percent(0,100);

#pragma omp parallel for schedule(static)
  for (int i = 0; i < BINDING_MATRIX_SIZE; i++) {
    #pragma omp parallel for schedule(static)
    for (int j = 0; j < BINDING_MATRIX_SIZE; j++) {
      if (dis_percent(float_gen_) > BINDING_MATRIX_ZERO_PERCENT)
        matrix_binding_[i*BINDING_MATRIX_SIZE+j]=dis_number(float_gen_);
      else
        matrix_binding_[i*BINDING_MATRIX_SIZE+j]=0;
    }
  }

}

void Common::save_binding_matrix() {
  char* lookup_table_file_name = new char[100];

  sprintf( lookup_table_file_name, "binding_matrix.zip" );

  gzFile lookup_table_file = gzopen( lookup_table_file_name, "r" );

  if ( lookup_table_file == Z_NULL )
  {
    printf( "ERROR : Could not read lookup table file %s\n", lookup_table_file_name );
    exit( EXIT_FAILURE );
  }

  float value;
  for (int i = 0; i < BINDING_MATRIX_SIZE; i++) {
    for (int j = 0; j < BINDING_MATRIX_SIZE; j++) {
      gzread(lookup_table_file, &value, sizeof(float));
      matrix_binding_[i*BINDING_MATRIX_SIZE+j] = value;
    }
  }

  gzclose( lookup_table_file );

  delete[] lookup_table_file_name;
}

void Common::load_binding_matrix() {
  char* lookup_table_file_name = new char[100];

  sprintf( lookup_table_file_name, "binding_matrix.zip" );

  gzFile lookup_table_file = gzopen( lookup_table_file_name, "w" );

  if ( lookup_table_file == Z_NULL )
  {
    printf( "ERROR : Could not write lookup table file %s\n", lookup_table_file_name );
    exit( EXIT_FAILURE );
  }

  double value;
  for (int i = 0; i < BINDING_MATRIX_SIZE; i++) {
    for (int j = 0; j < BINDING_MATRIX_SIZE; j++) {
      value = matrix_binding_[i*BINDING_MATRIX_SIZE+j];

      gzwrite(lookup_table_file, &value, sizeof(float));
    }
  }

  gzclose( lookup_table_file );

  delete[] lookup_table_file_name;
}
