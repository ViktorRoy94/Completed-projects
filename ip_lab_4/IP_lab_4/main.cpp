#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "WT_Haar.h"


using namespace cv;
using namespace std;

int main()
{
	IplImage* tmp = cvLoadImage("c:\image2.jpg");
	Mat img(tmp);
	Mat input;
	input = imread("c:\image2.jpg", 0);
	if(img.rows == 0)
	{
		std::cout<<"Input file Error\n";
		return -1;
	}
	int n;
    WT_Haar haar = WT_Haar(img);
    IplImage tmp_res = haar.GetImg();
    haar.Encode("out.txt");
	haar.Decode();

	IplImage encode = haar.GetImg();
	cvNamedWindow("Encode", 0);
	cvShowImage("Encode", &encode);
        
	haar.Decode();
		
	IplImage decode = input;
	cvNamedWindow("Decode", 0);
	cvShowImage("Decode", &decode);
		
	cvWaitKey(0);
	cvDestroyWindow("Encode");
	cvDestroyWindow("Decode");
	return 0;
}