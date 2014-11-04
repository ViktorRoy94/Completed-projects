#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <math.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>



using namespace cv;
using namespace std;

class Data3D
{       
	short n;        // X-size of Dataogram 
	short m;        // Y-size of Dataogram
	short layers_count;     // Data. layer's count
	short* data;                    // data of Dataogram

	public:
			Data3D();
			Data3D(const Data3D &t);
			Data3D(short n, short m, short h);
			Data3D(short n, short m, short h, short *data);
			void ReadData(std::string filename);
			void SaveData(std::string filename) const;
			short N() const;
			short M()const ;
			short Layers_count() const;
			void SetAll(short val);

			short GetMin() const;
			short GetMax() const;

			short GetLayerMin(int ind);
			short GetLayerMax(int ind);

			unsigned int Length() const;
			void Rescale(short new_min, short new_max);
			Data3D& operator =(const Data3D &t);
			Data3D& operator +=(const Data3D &t);
			Data3D& operator -=(const Data3D &t);
			Data3D operator +(const Data3D &t) const;
			Data3D operator -(const Data3D &t )const;

			short* GetLayer(int k);
			short* GetLine(int idx, int num_layer);

			/*Fing Min value in (i,j,k)-point locality.
			Locality has size*size*size elements*/
			short NeighMin(int size,int i, int j, int k) const;

			/*Fing Max value in (i,j,k)-point locality.
			Locality has size*size*size elements*/
			short NeighMax(int size, int i, int j, int k) const;

			void ShowLayer(int idx, short min, short max);
			void SaveLayer(int idx, short min, short max);

			short NeighMin(int size,int idx) const;
			short NeighMax(int size, int idx) const;
			void Pruning();
			void Clear();
			float* NormilizeData();
                
			short* Data();
                
			short& operator[](int idx);
			virtual ~Data3D();

};