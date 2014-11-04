#include "Data3D.h"


Data3D::Data3D(): n(1),m(1),layers_count(1)
{
  data = new short[1];
}
void Data3D::ReadData(std::string filename)
{
        delete []data;
        std::ifstream fs ( filename.c_str(), std::ios::in | std::ios::binary);

        if (!fs.is_open())
        {
                printf ("No such file or directory %s\n", filename.c_str() );
                return;
        }
        fs.read((char*)&n,sizeof(short));
        fs.read((char*)&m,sizeof(short));
        fs.read((char*)&layers_count,sizeof(short));
        
        data = new short[(long)n*(long)m*(long)layers_count];
        fs.read((char*)data,sizeof(short)*n*m*layers_count);
        fs.close();
}
void Data3D::SaveData(std::string filename) const
{
        std::ofstream fs(filename.c_str(), std::ios::out | std::ios::binary );

        fs.write((char*)&n,sizeof(short));
        fs.write((char*)&m,sizeof(short));
        fs.write((char*)&layers_count,sizeof(short));

        fs.write((char*)&data[0],sizeof(short)*n*m*layers_count);

        fs.close();
};

short Data3D::N() const
{
        return n;
}
short Data3D::M() const
{
        return m;
}
short Data3D::Layers_count() const
{
        return layers_count;
}
Data3D::Data3D(const Data3D &t)
{
        n = t.n;
        m = t.m;
        layers_count = t.layers_count;
        unsigned int size = Length();
        data = new short[size];
        for(unsigned int i = 0; i < size; ++i)
                data[i] = t.data[i];
}
void Data3D::SetAll(short val)
{
        for(int i = 0; i < n*m*layers_count; ++i)
                data[i] = val;
}
Data3D::~Data3D()
{
        delete []data;
}
short Data3D::GetMin() const
{
        short min = 0;
        for(int i = 0; i < n; ++i)
                min = data[i]<min?data[i]:min;
        return min;
}
short Data3D::GetMax() const
{
        short max = 0;
        for(int i = 0; i < n; ++i)
                max = data[i]>max?data[i]:max;
        return max;
}
void Data3D::Rescale(short new_min, short new_max)
{
        short d_min = GetMin();
        short d_max = GetMax();
        float d_delta = (float)d_max - d_min;
        float delta = (float)new_max - new_min;

        for( int i = 0 ;i < n; ++i)
                data[i] = ((float)((float)data[i]  - (float)d_min)/(float) d_delta) *(float)delta + new_min;
}
Data3D& Data3D::operator =(const Data3D &t)
{
        if(data!= NULL &&( n!= t.n || m!=t.m || layers_count != t.layers_count)) 
        delete []data;

        n = t.n;
        m = t.m;
        layers_count = t.layers_count;
        data = new short[Length()];
        for(unsigned int i = 0; i < Length(); ++i)
                data[i] = t.data[i];

        return *this;
}
Data3D Data3D::operator+(const Data3D &t) const
{
        Data3D tmp = t;

        tmp+=t;

        return tmp;
}
Data3D& Data3D::operator +=(const Data3D &t)
{
        if(n == t.n && m == t.m && layers_count == t.layers_count)
        {
                for(unsigned int i = 0; i < n*m*layers_count; ++i)
                        data[i]+=t.data[i];
        }
        return *this;
}
Data3D& Data3D::operator -=(const Data3D &t)
{
        if(n == t.n && m == t.m && layers_count == t.layers_count)
        {
                for(unsigned int i = 0; i < n*m*layers_count; ++i)
                        data[i]-=t.data[i];
        }
        return *this;
}
Data3D Data3D::operator-(const Data3D &t) const
{
        Data3D tmp = *this;

        tmp-= t;

        return tmp;
}
short& Data3D::operator[](int idx)
{
        if(idx < n*m*layers_count && idx>=0)
                return data[idx];
}
unsigned int Data3D::Length() const
{
        return n*m*layers_count;
}

Data3D::Data3D(short _n, short _m, short _h): n(_n), m(_m),layers_count(_h)
{
        data = new short[Length()];
        for(unsigned int i = 0; i < Length(); ++i)
                data[i] = 0;
}
Data3D::Data3D(short _n, short _m, short _h, short *_data): n(_n), m(_m),layers_count(_h)
{
        data = new short[Length()];
        for(unsigned int i = 0; i < Length(); ++i)
                data[i] = _data[i];
}

short Data3D::NeighMin(int size,int i, int j, int k) const
{
  short min = 10000;
  short val;
  unsigned int id = 0;
  
  for(int x = -size; x<=size; ++x)
    for(int y = -size; y<=size; ++y)
      for(int z = -size; z<=size; ++z)
        {
          id = (k+z)*n*m + (i+x)*n+ j;
          val = data[id];
          min = val <min?val:min;
        }
  return min;
}

short Data3D::NeighMax(int size,int i, int j, int k) const
{
  short max = -10000;
  short val;
  int id;
  for(int x = -size; x<=size; ++x)
    for(int y = -size; y<=size; ++y)
      for(int z = -size; z<=size; ++z)
        {

          id = (k+z)*n*m + (i+x)*n + j;
          val = data[id];
          max = val <max?val:max;
        }
  return max;
}

short Data3D::NeighMin(int size,int idx) const
{
  int k = idx / (n*m);
  int i = (idx - k*n*m) /n;
  int j = (idx - k*n*m - i*n);
  
  return NeighMin(size,i,j,k);
}

short Data3D::NeighMax(int size,int idx) const
{
  int k = idx / (n*m);
  int i = (idx - k*n*m) /n;
  int j = (idx - k*n*m - i*n);
  
  return NeighMax(size,i,j,k);
}

short* Data3D::Data()
{
        return data;
}

void Data3D::Clear()
{
        delete []data;
        n = 1;
        m = 1;
        layers_count = 1;
        data = new short[1];
}

float* Data3D::NormilizeData()
{
        float *tmp = new float[Length()];
        double m = Length();
        for(unsigned int i = 0; i < Length(); ++i)
                if(data[i] < 0 || data[i] > 400) tmp[i] = 0;
                else tmp[i] = ((double)data[i])/ ((double)(400));

        return tmp;
}

void Data3D::Pruning()
{
        int max_start_x = m-1;
        int max_start_y = n-1;
        int max_start_z = layers_count-1;

        int min_end_x = 0;
        int min_end_y = 0;
        int min_end_z = 0;

        for(int h = 0; h< layers_count; ++h)
                for(int i = 0; i < m; ++i)
                {
                        int j  = 0;
                        while(data[h*n*m + i*n +j]==0 && j < (n-1))
                                ++j;
                        if(j < max_start_y) max_start_y = j;

                        j  = n-1;
                        while(data[h*n*m + i*n +j]==0 && j > 0)
                                --j;

                        if(j > min_end_y) min_end_y = j;
                }

                for(int h = 0; h< layers_count; ++h)
                        for(int j = 0; j < n; ++j)
                {
                        int i  = 0;
                        while(data[h*n*m + i*n +j]==0 && i < (m-1))
                                ++i;
                        if(i < max_start_x) max_start_x = j;

                        i  = m-1;
                        while(data[h*n*m + i*n +j]==0 && i > 0)
                                --i;

                        if(i > min_end_x) min_end_x = i;
                }

                for(int i = 0; i < m; ++i)
                        for(int j = 0; j < n; ++j)
                {
                        int h  = 0;
                        while(data[h*n*m + i*n +j]==0 && h < (layers_count-1))
                                ++h;
                        if(h < max_start_z) max_start_z = h;

                        h  = layers_count-1;
                        while(data[h*n*m + i*n +j]==0 && h > 0)
                                --h;

                        if(h > min_end_z) min_end_z = h;
                }

        /*      for(int i = 0; i < m; ++i)
                        for(int j = 0; j < n; ++j )
                        {
                                int j  =0;
                                while(data[layers_count*n*m + i*n +j]==0 && j < n)
                                        ++j;
                        }
*/
                        min_end_z = (min_end_z<(layers_count-3))?min_end_z+2:layers_count-1;
                        max_start_z = (max_start_z >=2)?max_start_z-2:0;

                        min_end_x = (min_end_x<(m-3))?min_end_x+2:m-1;
                        max_start_x = (max_start_x >=2)?max_start_x-2:0;

                        min_end_y = (min_end_y<(n-3))?min_end_y+2:n-1;
                        max_start_y = (max_start_y >=2)?max_start_y-2:0;





        short *tmp = new short[(min_end_z - max_start_z+1)*(min_end_y - max_start_y+1)*(min_end_x - max_start_x+1)];
        for(int h = max_start_z; h <=min_end_z ; ++h)
                for(int i = max_start_x; i <= min_end_x; ++i)
                        for(int j = max_start_y; j <= min_end_y; ++j)
                        {
                                tmp[(h-max_start_z)*(min_end_y - max_start_y+1)*(min_end_x - max_start_x+1)
                                        + (i - max_start_x)* (min_end_y - max_start_y+1) + (j - max_start_y)]=
                                        data[h*n*m + i*n + j];
                        }
                        delete []data;
                        data = tmp;
                        n = (min_end_y - max_start_y+1);
                        m = (min_end_x - max_start_x+1);
                        layers_count = (min_end_z - max_start_z+1);
}

short* Data3D::GetLayer(int k)
{
	return (data+k*n*m);
}

short* Data3D::GetLine(int idx, int num_layer)
{
	return (data+num_layer*n*m + idx*n);
}

void Data3D::ShowLayer(int idx, short min, short max)
{
	Mat img(cv::Size(m,n),CV_8U);
	
	double size = max-min;
	unsigned int sidx = idx*n*m;

	for(int i = 0; i < img.rows; ++i)
		for(int j = 0; j < img.cols; ++j)
		{
			short t= data[sidx + i*n + j];
			if(t >=min && t<=max)
				img.at<uchar>(i,j) = (uchar)(((double)t - min)/size*255.0);
			else img.at<uchar>(i,j) = (uchar)0;
		}

    namedWindow( "Layer image", CV_WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Layer image", img );  
	waitKey(0);   
}

void Data3D::SaveLayer(int idx, short min, short max)
{
	Mat img(cv::Size(m,n),CV_8U);

	double size = max-min;
	unsigned int sidx = idx*n*m;

	for(int i = 0; i < img.rows; ++i)
		for(int j = 0; j < img.cols; ++j)
		{
			short t= data[sidx + i*n + j];
			if(t >=min && t<=max)
				img.at<uchar>(i,j) = (uchar)(((double)t - min)/size*255.0);
			else img.at<uchar>(i,j) = (uchar)0;
		}
	IplImage ipl_from_mat((IplImage)img);
	char buffer[1000];
	sprintf(buffer,"imageFolder/image[%u].jpg",idx);
	cvSaveImage(buffer,&ipl_from_mat);
}


short Data3D::GetLayerMin(int ind)
{
	short min = 0;
	for(int i = ind*n*m; i < (ind+1)*n*m; ++i)
			min = data[i]<min?data[i]:min;
	return min;
}
short Data3D::GetLayerMax(int ind)
{
	short max = 0;
	for(int i = ind*n*m; i < (ind+1)*n*m; ++i)
			max = data[i]>max?data[i]:max;
	return max;
}