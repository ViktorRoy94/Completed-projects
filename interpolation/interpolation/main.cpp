#include <stdio.h> 
#include <opencv2/opencv.hpp> 
#include <cuda_runtime.h>
#include "kernel.cuh"
#include "portable_time.h"
using namespace cv; 

void bic_interpolation(const Mat &img, Mat &BicImg, int N);

int main(int argc, char* argv[]) 
{ 
  Mat image;
  image = imread("image1.jpg",0);

  int n = 20;
  Mat bicubInterpolation((int)(n*image.rows), (int)(n*image.cols), CV_8UC1);
  
  ////////////////////////////////////////////////////
  // START CUDA HERE
  
  double time = PortableGetTime();

  const int bicubSize = n*n*image.rows*image.cols;        // результат изображения на cpu
  int *cpu_bicub = new int[bicubSize];  
  int *dev_bicub;								  // результат изображения на gpu	
  cudaMalloc( (void**)&dev_bicub, bicubSize*sizeof(int));

  const int imageSize = image.rows * image.cols;
  int *cpu_image = new int[imageSize];
  int *dev_image;
  cudaMalloc( (void**)&dev_image, imageSize*sizeof(int));

  for (int i = 0; i < image.rows; i++)
	  for (int j = 0; j < image.cols; j++)
	  {
		  cpu_image[i*image.cols + j] = (int)image.at<uchar>(i, j);
	  }
  
  cudaMemcpy(dev_image, cpu_image, imageSize*sizeof(int),cudaMemcpyHostToDevice);
  
  printf("Start cuda");
  
  
  StartCuda(dev_image, dev_bicub, n, n*image.rows, n*image.cols, image.rows, image.cols);
  
  
  cudaMemcpy(cpu_bicub, dev_bicub, bicubSize*sizeof(int),cudaMemcpyDeviceToHost);
  
  for (int i = 0; i < n*image.rows; i++)
	  for (int j = 0; j < n*image.cols; j++)
	  {
		  bicubInterpolation.at<uchar>(i,j) = (uchar)cpu_bicub[i * n * image.cols + j];
	  }

  time = PortableGetTime() - time;
  // FINISH CUDA HERE
  ////////////////////////////////////////////////////

  printf("time = %lf", time);
  namedWindow("Bicubinear interpolation", CV_WINDOW_AUTOSIZE );
  imshow("Bicubinear interpolation", bicubInterpolation);

  waitKey(0);
  
  cudaFree(dev_bicub);
  cudaFree(dev_image);
  delete [] cpu_bicub;
  delete [] cpu_image;

  bicubInterpolation.release();
  image.release();
  return 0; 
}   

