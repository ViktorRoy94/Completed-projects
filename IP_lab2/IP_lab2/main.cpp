#include <stdio.h> 
#include <opencv2/opencv.hpp>
#include <math.h>
 
using namespace cv; 
void smoothed_hist_create(Mat image, Mat &smoothedHist, Mat &histImage);
int	 triangle_method( Mat &smtHist);

int hist_w = 512; 
int hist_h = 400;
int histSize = 256;
float range[] = { 0, 256 } ;
const float* histRange = { range };
bool uniform = true; 
bool accumulat = true;

int main(int argc, char* argv[]) 
{ 
  Mat image;
  image = imread("image2.jpg", 0);

  Mat smoothed_hist;
  Mat smoothed_histImage(hist_h, hist_w, CV_8U);

  smoothed_hist_create(image, smoothed_hist, smoothed_histImage);
  
  imshow("SmoothedHistogram", smoothed_histImage );

  int T = triangle_method(smoothed_hist);
  
  imshow("src", image);
  threshold(image, image, T, 255, image.type());
  imshow("triangle_image", image );

  waitKey(0);
  return 0; 
}   

void smoothed_hist_create(Mat image, Mat &smoothedHist, Mat &histImage)
{
	calcHist( &image, 1, 0, Mat(), smoothedHist, 1, &histSize, &histRange);
    normalize(smoothedHist, smoothedHist, 0, 1000, NORM_MINMAX, -1, Mat());
	
	boxFilter(smoothedHist, smoothedHist, smoothedHist.depth(), Size(3, 3));

	int bin_w = cvRound((double) hist_w / histSize );

	for( int i = 1; i < histSize-5; i++ )
	{
	  line( histImage, Point( bin_w*(i-1), hist_h - cvRound(smoothedHist.at<float>(i-1)) ) ,
					   Point( bin_w*(i), hist_h - cvRound(smoothedHist.at<float>(i)) ),
					   Scalar( 0, 255, 255), 2, 8, 0  );
	}
}

int	 triangle_method( Mat &smtHist)
{
	int hmax, imax;
	int hmin, imin;
	int hd, id;
	float dmax = 0;
	hmax = -1;
	for( int i = 0; i < histSize-5; i++ )
	{
	  if (smtHist.at<float>(i) > hmax)
	  {
		  hmax = smtHist.at<float>(i);
		  imax = i;
	  }
	}
	hmin = 256;
	for( int i = 1; i < imax; i++ )
	{
	  if ((smtHist.at<float>(i) < hmin) && (smtHist.at<float>(i) != 0))
	  {
		  hmin = smtHist.at<float>(i);
		  imin = i;
	  }
	}

	int A = imax - imin;
	int B = hmax - hmin;
	int C = hmin*imin - hmax*imax;
	for (int i = imin; i < imax; i++)
	{
		if (((A * smtHist.at<float>(i) + i * B  + C) / sqrt((float)A*A + B*B))  > dmax)
		{
			dmax = (A * i + smtHist.at<float>(i) + C) / sqrt(A*A + 1.0);
			id = i;
		}
	}
	return id;
}

