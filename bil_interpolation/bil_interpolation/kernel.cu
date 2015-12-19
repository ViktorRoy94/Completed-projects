
#include "kernel.cuh"

__global__ void kernel(int *image, int *bilin, int N, int bilImgRows, int bilImCols, int imgRows, int imgCols)
{
	int j = 1 + threadIdx.x + blockIdx.x * blockDim.x;
	int i = 1 + threadIdx.y + blockIdx.y * blockDim.y;
	int h, w;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    int p1, p2, p3, p4;	/* Окрестные пикселы */
	while (j < bilImgRows - 2)
	{
		tmp = (float) (j) / (float) (bilImgRows-1) * (imgRows-1);
		h = (int) floor(tmp);
		if (h < 0) { h = 0; }
		else
		{
			if (h >= imgRows - 1) {
				h = imgRows - 2;
			}
		}
		u = tmp - h;
		while ( i < bilImCols - 2)
		{
			tmp = (float) (i) / (float) (bilImCols-1) * (imgCols-1);
			w = (int) floor(tmp);
			if (w < 0) { w = 0; }
			else
			{
				if (w >= imgCols - 1) {
					w = imgCols - 2;
				}
			}
			t = tmp - w;
	
			d1 = (1 - t) * (1 - u);
			d2 = t * (1 - u);
			d3 = t * u;
			d4 = (1 - t) * u;
			
			/* Окрестные пиксели: image.at<uchar>(i,j) */
			p1 = image[h * imgCols + w];
			p2 = image[h * imgCols + w+1];
			p3 = image[(h+1) * imgCols + w+1];
			p4 = image[(h+1) * imgCols + w];

			float pixel = p1*d1 + p2*d2 + p3*d3 + p4*d4;
			bilin[j * bilImCols + i] = pixel;
			i += blockDim.y * gridDim.y;
		}
		j += blockDim.x * gridDim.x;
	}
}

void StartCuda(int *image, int *bilin, int N, int bilImgRows, int bilImgCols, int imgRows, int imgCols)
{
	dim3 block( (bilImgRows + 511) / 512, (bilImgCols + 511) / 512);
	dim3 grid(512, 512);
	printf("\nstart kernel");
	kernel<<<grid, block>>>(image, bilin, N, bilImgRows, bilImgCols, imgRows, imgCols);
}
