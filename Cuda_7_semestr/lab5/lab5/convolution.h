#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "cufft.h"

/* ������� convolve ��������� ����������� ������� ���� ����������� ��������
   a � b ����� n, ��������� ������������ � result, ������ ��� ���� ��������;
   cufftComplex �������� ������ float2, �.�. ���������� � ������ x, y.
*/
void convolve(const cufftComplex * a, const cufftComplex * b, int n, cufftComplex * result);
void convolve_gpu(cufftComplex * a, cufftComplex * b, int n, cufftComplex * result);

#endif
