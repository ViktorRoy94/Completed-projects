#pragma once
#include "program.h"
ref class queue
{
protected:
	program* mas;
	int indexEnd; // индекс конца
	int indexBegin; // индекс начала

public:
	queue(void)
	{
		mas=new program[1000];
		indexEnd=0;
		indexBegin=0;
	}

	void Enqueue (program a) // a - промежуточная переменная || кладем элемент в очередь
	{
		End();
		mas[indexEnd++]=a;
	}
	program Dequeue () // достает элемент из очереди
	{
		Begin();
		program a=mas[indexBegin];
		program b;
		mas[indexBegin++]=b;
		return a;
	}


	void End () // закольцовывание очереди
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

