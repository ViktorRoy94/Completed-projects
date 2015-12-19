#include <opencv2/opencv.hpp> 
using namespace cv;

void diving(Mat &img, int x, int y, int &xx, int &yy){
    xx=x; 
    yy=y; 
    int flag=1;

    while(flag){
        flag=0;
        if(x-1>=0){
            if (img.at<uchar>(y,x-1)<img.at<uchar>(yy,xx)){
                xx=x-1;
                yy=y;
                flag=1;
            }
        }
        if(y-1>=0){
            if (img.at<uchar>(y-1,x)<img.at<uchar>(yy,xx)){
                xx=x;
                yy=y-1;
                flag=1;
            }
        }
        if(x+1<img.cols){
            if (img.at<uchar>(y,x+1)<img.at<uchar>(yy,xx)){
                xx=x+1;
                yy=y;
                flag=1;
            }
        }
        if(y+1<img.rows){
            if (img.at<uchar>(y+1,x)<img.at<uchar>(yy,xx)){
                xx=x;
                yy=y+1;
                flag=1;
            }
        }
        x=xx; y=yy;
    }
}

void tobboganing(Mat img, Mat &res){
    int x,y;

    for(int j=0;j<img.rows;j++){
        for(int i=0;i<img.cols;i++){
            diving(img,i,j,x,y);
            res.at<uchar>(j,i)=(img.at<uchar>(y,x))%256;
        }
    }
}