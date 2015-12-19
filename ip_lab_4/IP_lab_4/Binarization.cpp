# include "Binarization.h"

using namespace cv;

Binarization::Binarization(Mat _img)
{
	img = _img;
}
Mat Binarization::ToGrey()
{
	grey = Mat(img.rows, img.cols, CV_8UC1);
	for(int i = 0; i < img.rows; i++)
	{
		for(int j = 0; j < img.cols; j++)
		{
			grey.at<unsigned char>(i, j) = (unsigned char)(img.at<Vec3b>(i, j)[0]*0.2 + img.at<Vec3b>(i, j)[1]*0.7 + img.at<Vec3b>(i, j)[2]*0.1);
		}
	}
	printf("Stop ToGrey\n");
	return grey;
}
int Binarization::Calc_treshold(int i, int j, int r, int type)
{
	int pi, pj;
	int sum = 0;
	int min = grey.at<unsigned char>(i, j);
	int max = min;
	int minmax;
	for(int k = (i - r); k < (i + r); k++)
	{
		for(int l = (j - r); l < (j + r); l++)
		{
			pi = k;
			pj = l;
			if(pi < 0)
			{
				pi = 0;
			}
			if(pj < 0)
			{
				pj = 0;
			}
			if(pi >= grey.rows)
			{
				pi = grey.rows - 1;
			}
			if(pj >= grey.cols)
			{
				pj = grey.cols - 1;
			}
		sum = sum + grey.at<unsigned char>(pi, pj);
		if(grey.at<unsigned char>(pi, pj) < min) {min = grey.at<unsigned char>(pi, pj);}
		if(grey.at<unsigned char>(pi, pj) > max) {max = grey.at<unsigned char>(pi, pj);}
		}
	}
	sum = sum/(r*r*4);
	minmax = (max + min)/2;
	if(type == 0)
	{
		return sum;
	}
	if(type == 1)
	{
		return minmax;
	}
	if(type == 2)
	{
		return min;
	}
	if(type == 3)
	{
		return 90;
	}
}
int Binarization::Binarizeted (int type, int radius)
{
	int r = radius;
	int pi, pj;
	int t;
	ToGrey();
	bin = Mat(grey.rows, grey.cols, CV_8UC1);
	for(int i = 0; i < grey.rows; i++)
	{
		for(int j = 0; j < grey.cols; j++)
		{
			t = Calc_treshold(i, j, r, type);
			if(grey.at<unsigned char>(i, j) > t)
			{
				bin.at<unsigned char>(i, j) = 255;
			}
			else
			{
				bin.at<unsigned char>(i, j) = 0;
			}
		}
	}
	return 0;
}
Mat Binarization::GetMatrix()
{
	return bin;
}
