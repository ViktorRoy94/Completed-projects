#include <stdio.h> 
#include <opencv2/opencv.hpp> 
#include "portable_time.h"
using namespace cv; 

void hist_create(Mat &histImage, Mat &image);
void smoothed_hist_create(Mat &smoothedHist, Mat &image, Mat &histImage);
void bil_interpolation1(const Mat &image, Mat &newImage, int n);
void bic_interpolation(const Mat &img, Mat &BicImg, int N);
int hist_w = 512; 
int hist_h = 400;

int histSize = 256;
float range[] = { 0, 256 } ;
const float* histRange = { range };
bool uniform = true; 
bool accumulat = false;

int main(int argc, char* argv[]) 
{ 
  Mat image;
  image = imread("image2.jpg",0);
  Mat histImage( hist_h, hist_w, CV_8UC3);
  Mat smoothed_hist;
  Mat smoothed_histImage( hist_h, hist_w, CV_8UC3);
  
  int n = 30;
  Mat bilInterpolation (2*image.rows-1, 2*image.cols-1, image.type());
  Mat bilInterpolation1((int)(n*image.rows), (int)(n*image.cols), image.type());
  Mat bicubInterpolation((int)(n*image.rows), (int)(n*image.cols), image.type());

  hist_create(histImage, image);
  
  /*namedWindow("Image", CV_WINDOW_AUTOSIZE );
  imshow("Image", image);*/

  //namedWindow("Histogram", CV_WINDOW_AUTOSIZE );
  //imshow("Histogram", histImage );
  //
  //namedWindow("SmoothedHistogram", CV_WINDOW_AUTOSIZE );
  //smoothed_hist_create(smoothed_hist, image, smoothed_histImage);
  //imshow("SmoothedHistogram", smoothed_histImage );
  
  double time = PortableGetTime();
  bil_interpolation1(image, bilInterpolation1, n);
  time = PortableGetTime() - time;
  printf("time = %lf", time);

  namedWindow("Bilinear interpolation", CV_WINDOW_AUTOSIZE );
  imshow("Bilinear interpolation", bilInterpolation1);
  
  
  /*bic_interpolation(image, bicubInterpolation, n);
  namedWindow("Bicubinear interpolation", CV_WINDOW_AUTOSIZE );
  imshow("Bicubinear interpolation", bicubInterpolation);*/

  waitKey(0);

  image.release();
  histImage.release();
  smoothed_hist.release();
  bilInterpolation.release();
  bilInterpolation1.release();
  return 0; 
}   

void hist_create(Mat &histImage, Mat &image)
{
  Mat hist;  

  calcHist( &image, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulat);
  normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  int bin_w = cvRound( (double) hist_w/histSize );
  
  for( int i = 1; i < histSize; i++ )
  {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );
  }
}
void smoothed_hist_create(Mat &smoothedHist, Mat &image, Mat &histImage)
{
	Mat hist;
	calcHist( &image, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulat);
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	smoothedHist = hist.clone();

	for (int j = 0; j < 3; j++)
		for (int i = 1; i < histSize - 1; i++)
			smoothedHist.at<float>(i) = (smoothedHist.at<float>(i-1) + smoothedHist.at<float>(i) + smoothedHist.at<float>(i+1))/3;

	int bin_w = cvRound( (double) hist_w/histSize );

	for( int i = 1; i < histSize; i++ )
	{
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(smoothedHist.at<float>(i-1)) ) ,
					   Point( bin_w*(i), hist_h - cvRound(smoothedHist.at<float>(i)) ),
					   Scalar( 255, 255, 255), 2, 8, 0  );
	}
}

void bic_interpolation(const Mat &img, Mat &BicImg, int N)
{
    for (int j = 1; j < BicImg.rows-1; j++) 
	{
		float tmp = (float) (j) / (float) (BicImg.rows - 1) * (img.rows - 1);
		int h = (int) floor(tmp);
		if (h < 1) 
		{
			h = 1;
		} 
		else 
		{
			if (h >= img.rows - 2) 
			{
				h = img.rows - 3;
			}
		}
		float u = tmp - h;
		for (int i = 1; i < BicImg.cols-1; i++) 
		{
			tmp = (float) (i) / (float) (BicImg.cols - 1) * (img.cols - 1);
			int w = (int) floor(tmp);
			if (w < 1) 
			{
				w = 1;
			} 
			else 
			{
				if (w >= img.cols - 2) 
				{
					w = img.cols - 3;
				}
			}
			float t = tmp - w;

		    // Коэффициенты  

			float b1 =  (1.0f/4)*(u - 1)*(u - 2)*(u + 1)*(t - 1)*(t - 2)*(t + 1);
			float b2 = -  (1.0f/4) *(u)*  (u + 1)*(u - 2)*(t - 1)*(t - 2)*(t + 1)  ;
			float b3 = -  (1.0f/4)*(t)*(u - 1)*(u - 2)*(u + 1)*(t + 1)*(t - 2)  ;
			float b4 =   (1.0f/4)*(u)*(t)*(u + 1)*(u - 2)*(t + 1)*(t - 2)  ;
			float b5 = -  (1.0f/12)*(u)*(u - 1)*(u - 2)*(t - 1)*(t - 2)*(t + 1)  ;
			float b6 = -  (1.0f/12)*(t)*(u - 1)*(u - 2)*(u + 1)*(t - 1)*(t - 2)  ;
			float b7 =   (1.0f/12)*(u)*(t)*(u - 1)*(u - 2)*(t + 1)*(t - 2)  ;
			float b8 =   (1.0f/12)*(u)*(t)*(u + 1)*(u - 2)*(t - 1)*(t - 2)  ;
			float b9 =   (1.0f/12)*(u)*(u - 1)*(u + 1)*(t - 1)*(t - 2)*(t + 1)  ;
			float b10 =   (1.0f/12)*(t)*(u - 1)*(u - 2)*(u + 1)*(t - 1)*(t + 1)  ;
			float b11 =   (1.0f/36)*(u)*(t)*(u - 1)*(u - 2)*(t - 1)*(t - 2)  ;
			float b12 = -  (1.0f/12)*(u)*(t)*(u - 1)*(u + 1)*(t + 1)*(t - 2)  ;
			float b13 = -  (1.0f/12)*(u)*(t)*(u + 1)*(u - 2)*(t - 1)*(t + 1)  ;
			float b14 =  -  (1.0f/36)*(u)*(t)*(u - 1)*(u + 1)*(t - 1)*(t - 2)  ;
			float b15 =  -  (1.0f/36)*(u)*(t)*(u - 1)*(u - 2)*(t - 1)*(t + 1)  ;
			float b16 =   (1.0f/36)*(u)*(t)*(u - 1)*(u + 1)*(t - 1)*(t + 1) ;


			// Окрестные пиксели: a[i][j] 
			float p1 = img.at<uchar>(h,w);
			float p2 = img.at<uchar>(h+1,w);
			float p3 = img.at<uchar>(h,w+1);
			float p4 = img.at<uchar>(h+1,w+1);
			float p5 = img.at<uchar>(h-1,w);
			float p6 = img.at<uchar>(h,w-1);
			float p7 = img.at<uchar>(h-1,w+1);
			float p8 = img.at<uchar>(h+1,w-1);
			float p9 = img.at<uchar>(h+2,w);
			float p10 = img.at<uchar>(h, w+2);
			float p11= img.at<uchar>(h-1,w-1);
			float p12= img.at<uchar>(h+2,w+1);
			float p13= img.at<uchar>(h+1,w+2);
			float p14= img.at<uchar>(h+2,w-1);
			float p15= img.at<uchar>(h-1,w+2);
			float p16= img.at<uchar>(h+2,w+2);


			// Компоненты 
			float newPixel = p1 *b1 + p2 *b2 + p3 *b3 +  p4 *b4 +  p5 *b5+  p6 *b6+  p7 *b7+  p8 *b8+  p9 *b9+  p10 *b10+  p11 *b11+  p12 *b12+  p13 *b13+  p14 *b14+  p15 *b15+  p16 *b16;

			// Новый пиксел 
			BicImg.at<uchar>(j,i) = abs(newPixel);
	    }
    }
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

