#include <stdio.h> 
#include <opencv2/opencv.hpp> 
#include "portable_time.h"
#include "cuda_runtime.h"
#include "kernel.cuh"
using namespace cv; 

void bil_interpolation1(const Mat &image, Mat &newImage, int n);

int main(int argc, char* argv[]) 
{ 
  Mat image;
  image = imread("image2.jpg",0);
  int n = 30;
  Mat bilInterpolation1((int)(n*image.rows), (int)(n*image.cols), image.type());
  
  ////////////////////////////////////////////////////
  // START CUDA HERE
  
  double time = PortableGetTime();

  const int bilinSize = n*n*image.rows*image.cols;        // результат изображения на cpu
  int *cpu_bilin = new int[bilinSize];  
  int *dev_bilin;								          // результат изображения на gpu	
  cudaMalloc( (void**)&dev_bilin, bilinSize*sizeof(int));

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
  
  StartCuda(dev_image, dev_bilin, n, n*image.rows, n*image.cols, image.rows, image.cols);
  
  cudaMemcpy(cpu_bilin, dev_bilin, bilinSize*sizeof(int),cudaMemcpyDeviceToHost);
  
  for (int i = 0; i < n*image.rows; i++)
	  for (int j = 0; j < n*image.cols; j++)
	  {
		  bilInterpolation1.at<uchar>(i,j) = (uchar)cpu_bilin[i * n * image.cols + j];
	  }

  time = PortableGetTime() - time;
  
  // FINISH CUDA HERE
  ////////////////////////////////////////////////////

  //bil_interpolation1(image, bilInterpolation1, n);

  namedWindow("Bilinear interpolation", CV_WINDOW_AUTOSIZE );
  imshow("Bilinear interpolation", bilInterpolation1);
  
  
  printf("time = %lf", time);

  waitKey(0);

  image.release();
  bilInterpolation1.release();
  cudaFree(dev_bilin);
  cudaFree(dev_image);
  delete [] cpu_bilin;
  delete [] cpu_image;
  return 0; 
}   

void bil_interpolation1(const Mat &image, Mat &newImage, int n)
{
    int h, w;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    int p1, p2, p3, p4;	/* Окрестные пикселы */
	for ( int j = 0; j < newImage.rows; j++)
	{
		tmp = (float) (j) / (float) (newImage.rows-1) * (image.rows-1);
		h = (int) floor(tmp);
		if (h < 0) { h = 0; }
		else
		{
			if (h >= image.rows - 1) {
				h = image.rows - 2;
			}
		}
		u = tmp - h;
		for ( int i = 0; i < newImage.cols; i++)
		{
			tmp = (float) (i) / (float) (newImage.cols-1) * (image.cols-1);
			w = (int) floor(tmp);
			if (w < 0) { w = 0; }
			else
			{
				if (w >= image.cols - 1) {
					w = image.cols - 2;
				}
			}
			t = tmp - w;
	
			d1 = (1 - t) * (1 - u);
			d2 = t * (1 - u);
			d3 = t * u;
			d4 = (1 - t) * u;

			/* Окрестные пиксели: image.at<uchar>(i,j) */
			p1 = image.at<uchar>(h,w);
			p2 = image.at<uchar>(h,w+1);
			p3 = image.at<uchar>(h+1,w+1);
			p4 = image.at<uchar>(h+1,w);

			float pixel = p1*d1 + p2*d2 + p3*d3 + p4*d4;
			newImage.at<uchar>(j,i) = pixel;
		}
	}
}

