#pragma once
#include <cstring>
#include <sstream>
using namespace std;
class stack
{
private:
	int max; // к-во эл-тов стека
	string mas; // массив знаков
	int index; // индекс стека
public:
	stack(void)
	{
		max=100;
		index=0;
		mas=" ";
	}
	void Push (char n) 
	{
		if (index!=max)
		{
			mas+=n;
			index++;
		}
	}
	char Watch()
	{
		if(index>0) return mas[index];
		else return 0;
	}

	char Pop ()
	{
		if(!Is_empty())
		{
			char a=mas[index--];
			mas.erase(index+1,1);
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
	~stack()
	{
	}
};
