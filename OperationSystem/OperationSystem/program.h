#pragma once
#include <ctime>
#include <stdlib.h>
class program
{
private:
	int numCluster;
	int Time;
	int Status; // 0 - в очереди 
				// 1 - в обработке
				// 2 - выполнена	
				// 3 - невозможно выполнить

public:
	program(void)
	{
		numCluster=0;
		Time=0;
		Status=0;
	}

	program(int N)  // N - кол-во кластеров всего. Вводит пользователь.
	{
		srand(time(NULL));
		numCluster=rand() % ((int)(1.5*N))+1;
		Time=rand() % 8+2;
		Status=0;
	}
	
	int GetNumCluster()
	{
		return numCluster;
	}
	
	int GetTime()
	{
		return Time;
	}
	
	void SetTime(int i)
	{
		Time=i;
	}

	void SetStatus (int i)
	{
		Status=i;
	}
	int GetStatus ()
	{
		return Status;
	}


};

