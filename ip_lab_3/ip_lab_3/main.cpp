#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stack>
#include <set>
#include <math.h>
#include <opencv2/opencv.hpp> 

using namespace cv;
using namespace std;

int rows, cols;
uchar *tmp;
int T = 50;
uchar color = 0;


class vec3{
public:
	int x, y, z;
	vec3(int q, int w, int e){
		x = q; y = w; z = e;
	}
	vec3(){
		x = 0; y = 0; z = 0;
	}
	void fill(int q, int w, int e){
		x = q; y = w; z = e;
	}
};

class MatOfVec3{
public:
	vec3 **mat;
	int dimX, dimY;
	int count;
	MatOfVec3(){
		mat = NULL;
		dimX = 0; dimY = 0;
		count = dimX*dimY;
	}
	MatOfVec3(int q, int w){
		mat = new vec3*[q*w];
		dimX = q; dimY = w;
		count = dimX*dimY;
	}
	MatOfVec3(int q, int w, vec3 **_mat){
		mat = _mat;
		dimX = q; dimY = w;
		count = dimX*dimY;
	}
	MatOfVec3(int q, int w, vec3 **_mat, vec3 **_mat2){
		mat = new vec3*[q + w];
		dimX = -1; dimY = -1;
		count = q + w;
		for (int i = 0; i<q; i++){
			mat[i] = _mat[i];
		}
		for (int i = q; i<q + w; i++){
			mat[i] = _mat2[i - q];
		}
	}
	int middle(){
		int res = 0;
		for (int i = 0; i<count; i++){
			res += mat[i][0].z;
		}
		res /= count;
		return res;
	}
	bool isNeighbor(MatOfVec3 &a){
		
		for (int i = 0; i<(cols + 2)*(2 + rows); i++)
			tmp[i] = 0;
		if (a.count > count){
			for (int i = 0; i < count; i++) {
				tmp[(mat[i][0].x)    *(cols + 2) + (mat[i][0].y + 1)] = 1;
				tmp[(mat[i][0].x + 2)*(cols + 2) + (mat[i][0].y + 1)] = 1;
				tmp[(mat[i][0].x + 1)*(cols + 2) + (mat[i][0].y)] = 1;
				tmp[(mat[i][0].x + 1)*(cols + 2) + (mat[i][0].y + 2)] = 1;
			}
			for (int i = 0; i<a.count; i++){
				if (tmp[(a.mat[i][0].x + 1)*(cols + 2) + a.mat[i][0].y + 1] == 1){
					return 1;
				}
			}
		}
		else{
			for (int i = 0; i<a.count; i++){
				tmp[(a.mat[i][0].x)*(cols + 2) + (a.mat[i][0].y + 1)] = 1;
				tmp[(a.mat[i][0].x + 2)*(cols + 2) + (a.mat[i][0].y + 1)] = 1;
				tmp[(a.mat[i][0].x + 1)*(cols + 2) + (a.mat[i][0].y)] = 1;
				tmp[(a.mat[i][0].x + 1)*(cols + 2) + (a.mat[i][0].y + 2)] = 1;
			}
			for (int i = 0; i<count; i++){
				if (tmp[(mat[i][0].x + 1)*(cols + 2) + mat[i][0].y + 1] == 1){
					return 1;
				}
			}
		}
		return 0;
	}
	bool uniformlyWith(MatOfVec3 img, int t){
		int min = img.mat[0][0].z;
		int max = min;
		for (int i = 0; i<img.count; i++){
			int tmp1 = img.mat[i][0].z;
			if (tmp1<min) min = tmp1;
			if (tmp1>max) max = tmp1;
		}
		for (int i = 0; i<count; i++){
			int tmp2 = mat[i][0].z;
			if (tmp2<min) min = tmp2;
			if (tmp2>max) max = tmp2;
			if (max - min>t) return 0;
		}
		return 1;
	}
};

bool isUniformly(MatOfVec3 img, int t){
	int min = img.mat[0][0].z;
	int max = min;
	for (int i = 0; i<img.count; i++){
		int tmp = img.mat[i][0].z;
		if (tmp<min) min = tmp;
		if (tmp>max) max = tmp;
		if (max - min>t) return 0;
	}
	return 1;
}

void divide(MatOfVec3 mat, vec3 **subMat1, vec3 **subMat2, vec3 **subMat3, vec3 **subMat4){
	int dimX = mat.dimX;
	int dimY = mat.dimY;
	int x1 = (int)(dimX / 2);
	int y1 = (int)(dimY / 2);
	int k1 = 0, k2 = 0, k3 = 0, k4 = 0;
	for (int i = 0; i<dimX; i++){
		for (int j = 0; j<dimY; j++){
			if (i<x1&&j<y1) subMat1[k1++] = mat.mat[i*dimY + j];
			if (i<x1&&j >= y1) subMat2[k2++] = mat.mat[i*dimY + j];
			if (i >= x1&&j<y1)
				subMat3[k3++] = mat.mat[i*dimY + j];
			if (i >= x1&&j >= y1) subMat4[k4++] = mat.mat[i*dimY + j];
		}
	}
}

void divide(MatOfVec3 mat, vec3 **subMat1, vec3 **subMat2){
	int dimX = mat.dimX;
	int dimY = mat.dimY;
	int x1 = (int)(dimX / 2);
	int y1 = (int)(dimY / 2);
	int k1 = 0, k2 = 0, k3 = 0, k4 = 0;
	for (int i = 0; i<dimX; i++){
		for (int j = 0; j<dimY; j++){
			if (0 == x1&&j<y1) subMat1[k1++] = mat.mat[i*dimY + j];
			if (0 == x1&&j >= y1) subMat2[k2++] = mat.mat[i*dimY + j];
			if (0 == y1&&i<x1) subMat1[k1++] = mat.mat[i*dimY + j];
			if (0 == y1&&i >= x1) subMat2[k2++] = mat.mat[i*dimY + j];
		}
	}
}

void split(Mat img, stack<MatOfVec3> &s2){
	vec3 **arr = new vec3*[img.rows*img.cols];
	MatOfVec3 mat(img.rows, img.cols, arr);

	for (int i = 0; i<img.rows; i++){
		for (int j = 0; j<img.cols; j++){
			arr[i*img.cols + j] = new vec3(i, j, img.at<uchar>(i, j));
		}
	}

	stack<MatOfVec3> s1;
	s1.push(mat);

	while (!s1.empty()){

		MatOfVec3 tmp = s1.top();
		s1.pop();
		if (isUniformly(tmp, T)){
			s2.push(tmp);
		}
		else{
			if (tmp.dimX<2 || tmp.dimY<2){
				int dimX = tmp.dimX;
				int dimY = tmp.dimY;
				int x1 = (int)(dimX / 2);
				int y1 = (int)(dimY / 2);
				vec3 **subMat1, **subMat2;
				if (x1 == 0){
					subMat1 = new vec3*[(dimX - x1)*y1];
					subMat2 = new vec3*[(dimX - x1)*(dimY - y1)];
					divide(tmp, subMat1, subMat2);
					s1.push(MatOfVec3((dimX - x1), y1, subMat1));
					s1.push(MatOfVec3((dimX - x1), (dimY - y1), subMat2));
				}
				else{
					subMat1 = new vec3*[x1*(dimY - y1)];
					subMat2 = new vec3*[(dimX - x1)*(dimY - y1)];
					divide(tmp, subMat1, subMat2);
					s1.push(MatOfVec3(x1, (dimY - y1), subMat1));
					s1.push(MatOfVec3((dimX - x1), (dimY - y1), subMat2));
				}

			}
			else{
				int dimX = tmp.dimX;
				int dimY = tmp.dimY;
				int x1 = (int)(dimX / 2);
				int y1 = (int)(dimY / 2);
				vec3 **subMat1 = new vec3*[x1*y1],
					**subMat2 = new vec3*[x1*(dimY - y1)],
					**subMat3 = new vec3*[(dimX - x1)*y1],
					**subMat4 = new vec3*[(dimX - x1)*(dimY - y1)];
				divide(tmp, subMat1, subMat2, subMat3, subMat4);
				s1.push(MatOfVec3(x1, y1, subMat1));
				s1.push(MatOfVec3(x1, (dimY - y1), subMat2));
				s1.push(MatOfVec3((dimX - x1), y1, subMat3));
				s1.push(MatOfVec3((dimX - x1), (dimY - y1), subMat4));
			}
		}
	}
}

void merge(stack<MatOfVec3> &s1, stack<MatOfVec3> &s3){
	stack<MatOfVec3> s2;
	bool flag = 1;
	while (flag){
		flag = 0;
		while (!s1.empty() || !s2.empty()){
			MatOfVec3 tmp = s1.top(); s1.pop();
			while (!s1.empty()){
				MatOfVec3 tmp2 = s1.top(); s1.pop();
				if ((tmp.uniformlyWith(tmp2, T)) && (tmp.isNeighbor(tmp2))){
					tmp = MatOfVec3(tmp.count, tmp2.count, tmp.mat, tmp2.mat);
					flag = 1;
				}
				else{
					s2.push(tmp2);
				}
			}
			while (!s2.empty()){
				s1.push(s2.top()); s2.pop();
			}
			s3.push(tmp);
		}
		while (flag&&!s3.empty()){
			s1.push(s3.top()); s3.pop();
		}
	}
}

void splitAndMerge(Mat img, Mat &out){
	tmp = new uchar[(2 + cols)*(2 + rows)];

	stack<MatOfVec3> s1, s3;

	split(img, s1);
	merge(s1, s3);

	while (!s3.empty()){
		for (int i = 0; i<s3.top().count; i++){
			out.at<uchar>(s3.top().mat[i][0].x, s3.top().mat[i][0].y) = s3.top().middle();
			out.at<uchar>(s3.top().mat[i][0].x, s3.top().mat[i][0].y) = color;
		}
		s3.pop();
		color += 1;
	}
}

int moment(stack<vec3> s, int i, int j){
	int result = 0;
	while (!s.empty()){
		vec3 tmp = s.top(); s.pop();
		result += (pow(tmp.x, i) + pow(tmp.y, j));
	}
	return result;
}

int main(int argc, char* argv[])
{
	Mat img, bin;

	img = imread("image.jpg", 0);

	rows = img.rows; 
	cols = img.cols;

	bin = img.clone();
	for (int i = 0; i<img.rows; i++){
		for (int j = 0; j<img.cols; j++){
			if (bin.at<uchar>(i, j)>0)
				bin.at<uchar>(i, j) = 255;
		}
	}

	splitAndMerge(bin, bin);

	//   отображение исходного изображения и гистограмм 
	//namedWindow("Image", WINDOW_AUTOSIZE);
	//namedWindow("Result", WINDOW_AUTOSIZE);
	/*imshow("Image", img);
	imshow("Result", bin);*/



	//////Подсчет параметров формы 
	int q = 0;
	printf("number of area ");
	printf("%d\n", color);
	//scanf("%d", &q);
	while (q < 205) {
		stack<vec3> s;

		for (int i = 0; i<img.rows; i++){
			for (int j = 0; j<img.cols; j++){
				if (bin.at<uchar>(i, j) == q)
					s.push(vec3(i, j, bin.at<uchar>(i, j)));
			}
		}
		if (s.empty()){
			printf("\n\nToo big number of figure");
			break;
		}
		double orentation = 0;
		double m20 = moment(s, 2, 0);
		double m02 = moment(s, 0, 2);
		double m11 = moment(s, 1, 1);
		if ((m20 - m02) != 0){
			orentation = 0.5*(atan((2 * m11) / (m20 - m02)));
		}
		int perimeter = 0;
		int square = s.size();
		double x1 = 0, y1 = 0;
		while (!s.empty()) {
			vec3 tmp = s.top(); s.pop();
			if (tmp.x == 0 || tmp.x == bin.rows - 1 || tmp.y == 0 || tmp.y == cols - 1){
				perimeter++;
			}
			else if (bin.at<uchar>(tmp.x - 1, tmp.y) != tmp.z ||
				bin.at<uchar>(tmp.x + 1, tmp.y) != tmp.z ||
				bin.at<uchar>(tmp.x, tmp.y - 1) != tmp.z ||
				bin.at<uchar>(tmp.x, tmp.y + 1) != tmp.z){
				perimeter++;
			}
			x1 += tmp.x;
			y1 += tmp.y;
		}
		x1 /= square;
		y1 /= square;
		double compactness = perimeter*perimeter / square;

		printf("\Perimeter = %d", perimeter);
		printf("\tSquare = %d", square);
		printf("\tCompact = %f", compactness);
		printf("\tOrentation = %f", orentation);
		printf("\nCenter of mass: %f, %f", x1, y1);
		printf("number of area: ");
		//scanf("%d\n", &q);
		printf("%d\n", q);

		waitKey();
		q++;
	}
	
	// закрытие окон 
	destroyAllWindows();

	// осовобождение памяти 
	img.release();
	bin.release();
	return 0;
}