#include <cstdlib>
#include <iostream>
#include <string>

#include <cublas.h>
#include <curand.h>

#define NONE_TRANS 'N'
#define BLOCK_SIZE 32

__global__ void makePositiveOrientire( const int n, float* matrix )
{
    const int i = blockIdx.x * blockDim.x + threadIdx.x;
    if ( i < n )
    {
        matrix[ i * n + i ] += n;
    }
}

int main( int argc, char** argv )
{
    const int    n       = 100;
    const int    nIters  = 1000;
    const float  epsilon = 0.0001;

    cublasInit();
    curandGenerator_t generator;
    curandCreateGenerator( &generator, curandRngType::CURAND_RNG_PSEUDO_DEFAULT );
    curandSetPseudoRandomGeneratorSeed( generator, 2671ULL );

    float* devA = nullptr;
    float* devX = nullptr;
    float* devB = nullptr;

    cublasAlloc( n*n, sizeof( float ), (void**) &devA );
    cublasAlloc( n*1, sizeof( float ), (void**) &devX );
    cublasAlloc( n*1, sizeof( float ), (void**) &devB );

    curandGenerateUniform( generator, devA, n*n );
    curandGenerateUniform( generator, devB, n*1   );

    makePositiveOrientire<<< ( n + BLOCK_SIZE - 1 ) / BLOCK_SIZE, BLOCK_SIZE >>>( n, devA );
    cudaDeviceSynchronize();

    cudaMemcpy((void*) devX, (const void*) devB, sizeof( float )*n, cudaMemcpyDeviceToDevice);

    int    iter  = 0;
    float  error = 0.0f;
    float* devR  = nullptr;
    float* devE  = nullptr;

    cublasAlloc( n*1, sizeof( float ), (void**) &devR );
    cublasAlloc( n*1, sizeof( float ), (void**) &devE );

    do
    {
        /* r = A*x */
        cublasSgemv(
            NONE_TRANS, n, n,
            1.0f, (const float*) devA, n, (const float*) devX, 1, 0.0f, devR, 1
        );

        /* r = r - b */
        cublasSaxpy( n, -1.0f, (const float*) devB, 1, devR, 1 );

        /* e = A*r */
        cublasSgemv(
            NONE_TRANS, n, n,
            1.0f, (const float*) devA, n, (const float*) devR, 1, 0.0f, devE, 1
        );

        float t  = cublasSdot( n, (const float*) devE, 1, (const float*) devR, 1 );
              t /= cublasSdot( n, (const float*) devE, 1, (const float*) devE, 1 );
        
        /* x = x - t * r */
        cublasSaxpy( n, -t, (const float*) devR, 1, devX, 1 );

        /* e = A*x */
        cublasSgemv(
            NONE_TRANS, n, n,
            1.0f, (const float*) devA, n, (const float*) devX, 1, 0.0f, devE, 1
        );

        /* e = e - b */
        cublasSaxpy( n, -1, (const float*) devB, 1, devE, 1 );

        /* error = ||e|| */
        error = cublasSnrm2( n, (const float*) devE, 1 );
    } while ( nIters > iter && error > epsilon);

    std::cout << "diff = " << error << std::endl;
	system("pause");
    cublasFree( devE );
    cublasFree( devR );

    cublasFree( devA );
    cublasFree( devX );
    cublasFree( devB );

    cublasShutdown();
    curandDestroyGenerator( generator );

	
    return 0;
}
