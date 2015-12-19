#pragma once
#include <cstring>
#include <sstream>
using namespace std;
class stack1
{
private:
	int max; // к-во эл-тов стека
	double* mas; // массив знаков
	int index; // индекс стека
public:
	stack1(void)
	{
		max=100;
		index=0;
		mas=new double[max];
	}
	void Push (double n) 
	{
		if (index!=max)
		{
			mas[index]=n;
			index++;
		}
	}
	double Watch()
	{
		if(index>0) return mas[index];
		else return 0;
	}

	double Pop ()
	{
		if(!Is_empty())
		{
			double a=mas[--index];
			mas[index+1]=NULL;
			return a;
		}
	}

	
	bool Is_empty()
	{
		if(index<1) 
			return true;
		else return false;
	}
	
	bool Is_full()
	{
		if(index==max-1)
			return true;
		else return false;
	}
	~stack1()
	{
	}
};

