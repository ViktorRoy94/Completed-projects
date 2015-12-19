#include "Claster.h"
#include "Math.h"
#include "Binarization.h"
#include "Regions.h"

using namespace cv;

Claster::Claster(Mat _img)
{
	img = _img;
}


int Claster::Adapt_calc()
{
	Binarization b(img);
	b.Binarizeted(3, 1);
	bin = b.GetMatrix();
	int count = 0;
	int i, j;
	int A, B, C;
	for(i = 0; i < bin.rows; i++)
	{
		for(j = 0; j < bin.cols; j++)
		{
			A = bin.at<unsigned char>(i, j);
			if((j - 1) >= 0)
			{
				B = bin.at<unsigned char>(i, (j - 1));
			}
			else
			{
				B = 0;
			}
			if((i - 1) >= 0)
			{
				C = bin.at<unsigned char>((i - 1), j);
			}
			else
			{
				C = 0;
			}
			/////
			if((B == 0)&&(C == 0))
			{
				if(A == 0)
				{
				}
				else
				{
					count++;
					areas.push_back(1);
					perim.push_back(1);
				}
			}
			else
			{
				if(B != 0)
				{
					bin.at<unsigned char>(i, j) = B;
					areas[count - 1]++;
					if(C != 0)
					{
						if((i + 1) >= bin.rows)
						{
							perim[count - 1]++;
						}
						else
						{
							if(bin.at<unsigned char>((i + 1), j) == 0)
							{
								perim[count - 1]++;
							}
						}
					}
					else
					{
						perim[count - 1]++;
					}
				}
				else
				{
					bin.at<unsigned char>(i, j) = C;
					areas[count - 1]++;
				}
			}
			/////

		}
	}
	return count;
}

int Claster::Reg_calc()
{
	int i,j;
	int n;
	int A, B, C, A_r, B_r, C_r;
	int delta = 50;
	reg = Mat(img.rows, img.cols, CV_8UC1);
	std::vector<Regions> vReg;
	std::vector<int> regs;
	int regions = 1;
	for(i = 0; i < reg.rows; i++)
	{
		for(j = 0; j < reg.cols; j++)
		{
			reg.at<unsigned char>(i, j) = img.at<Vec3b>(i,j)[0]*0.8 + img.at<Vec3b>(i,j)[1]*0.0 + img.at<Vec3b>(i,j)[2]*0.2;
		}
	}

    Regions r_tmp(0, 0, reg);
	for(i = 1; i < reg.rows; i++)
	{
		r_tmp.AddPixel(i, 0);
	}
	for(i = 1; i < reg.cols; i++)
	{
		r_tmp.AddPixel(0, i);
	}
	vReg.push_back(r_tmp);
	areas.push_back(reg.rows + reg.cols - 1);
	perim.push_back(reg.rows + reg.cols - 1);
	for(i = 1; i < reg.rows; i++)
	{
		for(j = 1; j < reg.cols; j++)
		{
			if((j - 1) >= 0)
			{
				B = reg.at<unsigned char>(i, (j - 1));
			}
			else
			{
				B = reg.at<unsigned char>(i, j);
			}
			if((i - 1) >= 0)
			{
				C = reg.at<unsigned char>((i - 1), j);
			}
			else
			{
				C = reg.at<unsigned char>(i, j);
			}
			A = reg.at<unsigned char>(i, j);
			B_r = 0;
			C_r = 0;
			if((abs(A - B) > delta)&&(abs(A - C) > delta))
			{
				regions++;
				r_tmp = Regions(i, j, reg);
				vReg.push_back(r_tmp);
				areas.push_back(1);
				perim.push_back(1);
			}
			else
			{
				if(abs(A - B) <= delta)
				{
					if(abs(A - C) <= delta)
					{
						if(((i - 1) >= 0)&&((j - 1) >= 0))
						{
							//merge regions
							n = vReg.size();
							for(int i_r = 0; i_r < n; i_r++)
							{
								if(vReg[i_r].ExistPixel(i, (j - 1)))
								{
									B_r = i_r;
									//printf("%d: %d\n",i_r, vReg[i_r].points.size());
								}
								if(vReg[i_r].ExistPixel((i - 1), j))
								{
									C_r = i_r;
								}
							}
							if(B_r != C_r)
							{
								vReg[B_r].AddRegion(vReg[C_r]);
								areas[B_r] = areas[B_r] + areas[C_r];
								perim[B_r] = perim[B_r]/2 + perim[C_r]/2;
								reg.at<unsigned char>(i, j) = vReg[B_r].avg;
								vReg.erase(vReg.begin() + C_r);
								areas.erase(areas.begin() + C_r);
								perim.erase(perim.begin() + C_r);
								regions--;
							}
							else
							{
								vReg[B_r].AddPixel(i, j);
								areas[B_r]++;
								if((vReg[B_r].ExistPixel((i - 1), j))&&(vReg[B_r].ExistPixel(i, (j - 1))))
								{
								}
								else
								{
									perim[B_r]++;
								}
								reg.at<unsigned char>(i, j) = vReg[B_r].avg;
							}
						}
					}
					else
					{
						//A add to B region B
						n = vReg.size();
						for(int i_r = 0; i_r < n; i_r++)
						{
							if(vReg[i_r].ExistPixel(i, (j - 1)))
							{
								B_r = i_r;
							}
						}
						vReg[B_r].AddPixel(i, j);
						areas[B_r]++;
						if((vReg[B_r].ExistPixel((i - 1), j))&&(vReg[B_r].ExistPixel(i, (j - 1))))
						{
						}
						else
						{
							perim[B_r]++;
						}
						reg.at<unsigned char>(i, j) = vReg[B_r].avg;
					}
				}
				else
				{
					if(abs(A - C) <= delta)
					{
						//A add to region C
						n = vReg.size();
						for(int i_r = 0; i_r < n; i_r++)
						{
							if(vReg[i_r].ExistPixel((i - 1), j))
							{
								C_r = i_r;
							}
						}
						vReg[C_r].AddPixel(i, j);
						areas[C_r]++;
						if((vReg[C_r].ExistPixel((i - 1), j))&&(vReg[C_r].ExistPixel(i, (j - 1))))
						{
						}
						else
						{
							perim[C_r]++;
						}
						reg.at<unsigned char>(i, j) = vReg[C_r].avg;
					}
				}
			}
		}
	}
	return regions;
}

Mat Claster::GetRegMatrix()
{
	return reg;
}
Mat Claster::GetBinMatrix()
{
	return bin;
}
