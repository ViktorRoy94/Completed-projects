#include <iostream>
#include <cuda_runtime_api.h>

using namespace std;

__global__ void kernel(int* a, int n)
{
	int tid = blockIdx.x * blockDim.x + threadIdx.x;
	if ( tid < n) {
		a[tid] = a[tid] + tid;
		printf("a[i] = %d\n", a[tid]);
	}
}

int main()
{
	const int n = 100;
	int a[n];
	int *dev_a;
	
	// started values
	for (int i = 0; i < n; i++)
	{
		a[i] = i;
	}

	// memory on GPU
	cudaMalloc((void**)&dev_a, n * sizeof(int));

	// copy to GPU
	cudaMemcpy(dev_a, a, n * sizeof(int), cudaMemcpyHostToDevice);

    kernel<<<10, 10>>>(dev_a, n);
    
	cudaMemcpy(a, dev_a, n * sizeof(int), cudaMemcpyDeviceToHost);
	
	// output
	for( int i = 0; i < n; i++) {
		cout << "a[i] = " << a[i] << endl;
	}
	
	// release memory
	cudaFree(dev_a);

	system("Pause");
    return 0;
}

