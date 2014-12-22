#include <opencv2/opencv.hpp>
#include <time.h>
#include <iostream>
//#include "opencv2/core.hpp"
//#include "opencv2/core/utility.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"

#include <stdio.h>

using namespace cv;
using namespace std;

static void help() {
    printf("\n"
           "Usage:\n"
            "./texture [image_name -- default ../data/lena.png]\n");
}

const char* keys = {
    "{image|../data/lena.png|input image file}"
    "{count|count of clusters}"
};


Mat L5 = (Mat_<float>(1,5) << 1, 4, 6, 4, 1);
Mat E5 = (Mat_<float>(1,5) << -1, -2, 0, 2, 1);
Mat S5 = (Mat_<float>(1,5) << -1, 0, 2, 0, -1);
Mat R5 = (Mat_<float>(1,5) << 1, -4, 6, -4, 1);


int main(int argc, char** argv) {
    Mat img = imread(argv[1]);
    if( img.empty() ) {
        help();
        printf("Cannot read image file: %s\n", argv[1]);
        return -1;
    }

    Mat planes[9];
    Mat E5E5 = E5.t()*E5;
    Mat S5S5 = S5.t()*S5;
    Mat R5R5 = R5.t()*R5;
    Mat E5L5 = E5.t()*L5;
    Mat S5L5 = S5.t()*L5;
    Mat R5L5 = R5.t()*L5;
    Mat S5E5 = S5.t()*E5;
    Mat R5E5 = R5.t()*E5;
    Mat R5S5 = R5.t()*S5;
    filter2D(img, planes[0], CV_32F, E5E5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[1], CV_32F, S5S5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[2], CV_32F, R5R5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[3], CV_32F, E5L5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[4], CV_32F, S5L5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[5], CV_32F, R5L5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[6], CV_32F, S5E5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[7], CV_32F, R5E5, Point(-1,-1), 0.0, BORDER_REPLICATE);
    filter2D(img, planes[8], CV_32F, R5S5, Point(-1,-1), 0.0, BORDER_REPLICATE);

    Mat E15(15, 15, CV_32F);
    for (int i = 0; i < E15.rows; i++) {
        for (int j = 0; j < E15.cols; j++) {
            E15.at<float>(i, j) = 1.0f;
        }
    }
    for (int m = 0; m < 9; m++) {
        filter2D(planes[m], planes[m], -1, E15,
            Point(-1,-1), 0.0, BORDER_REPLICATE);
    }

    Mat dst;
    merge(planes, 9, dst);
    dst = dst.reshape(0, dst.cols*dst.rows);
    imshow("Source", img);
    printf("%d %d\n", dst.cols, dst.rows);

    Mat labels;
    Mat centers;
    int K = atoi(argv[2]);
    double error = kmeans(dst, K, labels,
                          TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 100, 1.0), 3,
                          KMEANS_PP_CENTERS, centers);

    printf("Error = %lf\n", error);

    srand(time(0));
    Mat clust(img.size(), CV_8UC3);
    Mat colors(Size(1, K), CV_8UC3);
    for (int i = 0; i < K; i++) {
        colors.at<Vec3b>(i)[0] = rand() % 256;
        colors.at<Vec3b>(i)[1] = rand() % 256;
        colors.at<Vec3b>(i)[2] = rand() % 256;
    }

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            clust.at<Vec3b>(i, j) = colors.at<Vec3b>(labels.at<int>(j + i*img.cols,0));
        }
    }

    printf("%d %d\n", labels.cols, labels.rows);
    printf("%d %d\n", clust.cols, clust.rows);

    imshow("Segmentation", clust);

    waitKey();
}
