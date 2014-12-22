#include <opencv2/opencv.hpp> 
#include "Tobogganing.h"
using namespace cv;

int main(int argc, char* argv[])
{
	Mat image;
	image = imread("image1.jpg", 0);
	Mat result(image.rows, image.cols, image.type());
	tobboganing(image, result);
	imshow("Tbboganing", result);
	waitKey(0);

	image.release();
	result.release();
}