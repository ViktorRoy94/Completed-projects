#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "cufft.h"

/* Функция convolve выполняет циклическую свертку двух комплексных сигналов
   a и b длины n, результат записывается в result, память под него выделена;
   cufftComplex является просто float2, т.е. структурой с полями x, y.
*/
void convolve(const cufftComplex * a, const cufftComplex * b, int n, cufftComplex * result);
void convolve_gpu(cufftComplex * a, cufftComplex * b, int n, cufftComplex * result);

#endif
