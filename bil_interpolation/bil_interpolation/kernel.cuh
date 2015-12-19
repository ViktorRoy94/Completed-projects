#include <cuda_runtime.h>
#include <opencv2/opencv.hpp> 

__global__ void kernel(int *image, int *bilin, int N, int bilImgRows, int bilImCols, int imgRows, int imgCols);
void StartCuda(int *image, int *bilin, int N, int bilImgRows, int bilImgCols, int imgRows, int imgCols);