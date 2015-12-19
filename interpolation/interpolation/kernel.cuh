#include <cuda_runtime.h>
#include <opencv2/opencv.hpp> 

__global__ void kernel(int *image, int *bicub, int N, int bicImgRows, int bicImCols, int imgRows, int imgCols);
void StartCuda(int *image, int *bicub, int N, int bicImgRows, int bicImgCols, int imgRows, int imgCols);