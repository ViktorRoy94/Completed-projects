#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

#define PARSE_ERROR 1
#define DATA_ERROR 2

Mat haar_wavelet_transform(Mat src, int iterations)
{
	Mat result = src.clone();
	Mat temp;
	int rows = result.rows;
	int cols = result.cols;

	for (int k = 0; k < iterations; k++)
	{
		temp = result.clone();
		for (int i = 0; i < rows; i++)
		{
			int h = cols >> 1;
			for (int j = 0; j < h; j++)
			{
				int k = (j << 1);
				result.at<Vec3f>(i, j) = temp.at<Vec3f>(i, k) * 0.5 + temp.at<Vec3f>(i, k+1) * 0.5;
				result.at<Vec3f>(i, j + h) = temp.at<Vec3f>(i, k) * 0.5 - temp.at<Vec3f>(i, k + 1) * 0.5;
			}
		}
		temp = result.clone();
		for (int i = 0; i < cols; i++)
		{
			int h = rows >> 1;
			for (int j = 0; j < h; j++)
			{
				int k = (j << 1);
				result.at<Vec3f>(j, i) = temp.at<Vec3f>(k, i) * 0.5 + temp.at<Vec3f>(k + 1, i) * 0.5;
				result.at<Vec3f>(j + h, i) = temp.at<Vec3f>(k, i) * 0.5 - temp.at<Vec3f>(k + 1, i) * 0.5;
			}
		}
		cols = cols / 2;
		rows = rows / 2;
	}
	return result;
}

Mat inverse_haar_wavelet_transform(Mat src, int iterations)
{
	Mat result = src.clone();
	Mat temp;
	int rows = result.rows;
	int cols = result.cols;

	for (int i = 0; i < iterations; i++)
	{
		cols = cols / 2;
		rows = rows / 2;
	}
	for (int l = 0; l < iterations; l++)
	{	
		cols = cols * 2;
		rows = rows * 2;
		temp = result.clone();
		for (int i = 0; i < cols; i++)
		{
			int h = rows >> 1;
			for (int j = 0; j < h; j++)
			{
				int k = (j << 1);
				result.at<Vec3f>(k, i) = temp.at<Vec3f>(j, i) + temp.at<Vec3f>(j + h, i);
				result.at<Vec3f>(k + 1, i) = temp.at<Vec3f>(j, i) - temp.at<Vec3f>(j + h, i);
			}
		}
		temp = result.clone();
		for (int i = 0; i < rows; i++)
		{
			int h = cols >> 1;
			for (int j = 0; j < h; j++)
			{
				int k = (j << 1);
				result.at<Vec3f>(i, k) = temp.at<Vec3f>(i, j) + temp.at<Vec3f>(i, j + h);
				result.at<Vec3f>(i, k + 1) = temp.at<Vec3f>(i, j) - temp.at<Vec3f>(i, j + h);
			}
		}
	}
	return result;
}

int main(int argc, char* argv[]) 
{
	/*if (argc < 2)
		return PARSE_ERROR;*/
	Mat src = imread("image1.jpg");
	if (src.data == NULL)
		return DATA_ERROR;
	imshow("original", src);
	src.convertTo(src, CV_32FC3);
	normalize(src, src, -1.0, 1.0, NORM_MINMAX, CV_32FC3);

	int iterations = 3;

	Mat result;
	result = haar_wavelet_transform(src, iterations);

	Mat result2;
	result2 = inverse_haar_wavelet_transform(result, iterations);

	normalize(result2, result2, 0, 255, NORM_MINMAX, CV_32FC3);
	result2.convertTo(result2, CV_8UC3);

	normalize(result, result, 0, 255, NORM_MINMAX, CV_32FC3);
	result.convertTo(result, CV_8UC3);
	imshow("result", result);
	imshow("result2", result2);
	waitKey(0);
	return 0;
}