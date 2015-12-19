#pragma once
#include "program.h"
ref class queue
{
protected:
	program* mas;
	int indexEnd; // ������ �����
	int indexBegin; // ������ ������

public:
	queue(void)
	{
		mas=new program[1000];
		indexEnd=0;
		indexBegin=0;
	}

	void Enqueue (program a) // a - ������������� ���������� || ������ ������� � �������
	{
		End();
		mas[indexEnd++]=a;
	}
	program Dequeue () // ������� ������� �� �������
	{
		Begin();
		program a=mas[indexBegin];
		program b;
		mas[indexBegin++]=b;
		return a;
	}


	void End () // ��������������� �������
	{
		if (indexEnd==1000)
		{
			indexEnd=0;
		}

	}
	void Begin ()
	{
		if (indexBegin==1000)
		{
			indexBegin=0;
		}
	}


	int GetIndexBegin ()
	{
		return indexBegin;
	}

	int GetIndexEnd ()
	{
		return indexEnd;
	}
};

