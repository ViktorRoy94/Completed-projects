#include "kernel.cuh"

__global__ void kernel(int *image, int *bicub, int N, int bicImgRows, int bicImCols, int imgRows, int imgCols)
{
	int j = 1 + threadIdx.x + blockIdx.x * blockDim.x;
	int i = 1 + threadIdx.y + blockIdx.y * blockDim.y;
	/*int i = threadIdx.x;
	int j = blockIdx.y;*/
	//printf("In kernel");
	//for (int j = 1; j < bicImgRows-1; j++) 
	while (j < bicImgRows - 2)
	{
		float tmp = (float) (j) / (float) (bicImgRows - 1) * (imgRows - 1);
		int h = (int) floor(tmp);
		if (h < 1) 
		{
			h = 1;
		} 
		else 
		{
			if (h >= imgRows - 2) 
			{
				h = imgRows - 3;
			}
		}
		float u = tmp - h;
		//for (int i = 1; i < bicImCols-1; i++) 
		while ( i < bicImCols - 2)
		{
			tmp = (float) (i) / (float) (bicImCols - 1) * (imgCols - 1);
			int w = (int) floor(tmp);
			if (w < 1) 
			{
				w = 1;
			} 
			else 
			{
				if (w >= imgCols - 2) 
				{
					w = imgCols - 3;
				}
			}
			float t = tmp - w;

		    // Коэффициенты  

			float b1 =  (1.0f/4)*(u - 1)*(u - 2)*(u + 1)*(t - 1)*(t - 2)*(t + 1);
			float b2 = -  (1.0f/4) *(u)*  (u + 1)*(u - 2)*(t - 1)*(t - 2)*(t + 1)  ;
			float b3 = -  (1.0f/4)*(t)*(u - 1)*(u - 2)*(u + 1)*(t + 1)*(t - 2)  ;
			float b4 =   (1.0f/4)*(u)*(t)*(u + 1)*(u - 2)*(t + 1)*(t - 2)  ;
			float b5 = -  (1.0f/12)*(u)*(u - 1)*(u - 2)*(t - 1)*(t - 2)*(t + 1)  ;
			float b6 = -  (1.0f/12)*(t)*(u - 1)*(u - 2)*(u + 1)*(t - 1)*(t - 2)  ;
			float b7 =   (1.0f/12)*(u)*(t)*(u - 1)*(u - 2)*(t + 1)*(t - 2)  ;
			float b8 =   (1.0f/12)*(u)*(t)*(u + 1)*(u - 2)*(t - 1)*(t - 2)  ;
			float b9 =   (1.0f/12)*(u)*(u - 1)*(u + 1)*(t - 1)*(t - 2)*(t + 1)  ;
			float b10 =   (1.0f/12)*(t)*(u - 1)*(u - 2)*(u + 1)*(t - 1)*(t + 1)  ;
			float b11 =   (1.0f/36)*(u)*(t)*(u - 1)*(u - 2)*(t - 1)*(t - 2)  ;
			float b12 = -  (1.0f/12)*(u)*(t)*(u - 1)*(u + 1)*(t + 1)*(t - 2)  ;
			float b13 = -  (1.0f/12)*(u)*(t)*(u + 1)*(u - 2)*(t - 1)*(t + 1)  ;
			float b14 =  -  (1.0f/36)*(u)*(t)*(u - 1)*(u + 1)*(t - 1)*(t - 2)  ;
			float b15 =  -  (1.0f/36)*(u)*(t)*(u - 1)*(u - 2)*(t - 1)*(t + 1)  ;
			float b16 =   (1.0f/36)*(u)*(t)*(u - 1)*(u + 1)*(t - 1)*(t + 1) ;


			// Окрестные пиксели: a[i][j] 
			float p1 = image[h * imgCols + w];
			float p2 = image[(h+1) * imgCols + w];
			float p3 = image[h * imgCols + w+1];
			float p4 = image[(h+1) * imgCols + w+1];
			float p5 = image[(h-1) * imgCols + w];
			float p6 = image[h * imgCols + w-1];
			float p7 = image[(h-1) * imgCols + w+1];
			float p8 = image[(h+1) * imgCols + w-1];
			float p9 = image[(h+2) * imgCols + w];
			float p10 = image[(h) * imgCols + w+2];
			float p11= image[(h-1) * imgCols + w-1];
			float p12= image[(h+2) * imgCols + w+1];
			float p13= image[(h+1) * imgCols + w+2];
			float p14= image[(h+2) * imgCols + w-1];
			float p15= image[(h-1) * imgCols + w+2];
			float p16= image[(h+2) * imgCols + w+2];


			// Компоненты 
			float newPixel = p1 *b1 + p2 *b2 + p3 *b3 +  p4 *b4 +  p5 *b5+  p6 *b6+  p7 *b7+  p8 *b8+  p9 *b9+  p10 *b10+  p11 *b11+  p12 *b12+  p13 *b13+  p14 *b14+  p15 *b15+  p16 *b16;

			// Новый пиксел 
			bicub[j *bicImCols + i] = abs(newPixel);
			i += blockDim.y * gridDim.y;
		}
		j += blockDim.x * gridDim.x;
	}
}

void StartCuda(int *image, int *bicub, int N, int bicImgRows, int bicImCols, int imgRows, int imgCols)
{
	dim3 block( (bicImgRows + 511) / 512, (bicImCols + 511) / 512);
	dim3 grid(512, 512);
	//dim3 grid(2560, 2560);
	//dim3 block(16, 16);
	printf("\nstart kernel");
	kernel<<<grid, block>>>(image, bicub, N, bicImgRows, bicImCols, imgRows, imgCols);
	//kernel<<<1024, 1024>>>(image, bicub, N, bicImgRows, bicImCols, imgRows, imgCols);
}
