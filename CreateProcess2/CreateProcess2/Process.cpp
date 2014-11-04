#include <Windows.h>
#include <iostream>
using namespace std;
int volatile var;

void ThreadProc()
{
	for(int i=0;i<100000;i++)
	{
		var++;
	}
} 
void ThreadProc1()
{
	for(int i=0;i<100000;i++)
	{
		var--;
	}
} 

int main ()
{

	DWORD a;
	HANDLE b[20]; 
	for (int i=0;i<10;i++)
	{
		b[i] = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)(&ThreadProc),(LPVOID)&var,0,&a);	
		if(b!=NULL)
		{
			//cout << "it works!";
		}
		else cout << "ERROR!";
	}
	for (int i=10;i<20;i++)
	{
		b[i] = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)(&ThreadProc1),(LPVOID)&var,0,&a);	
		if (b!=NULL)
		{
			//WaitForSingleObject( b, INFINITE );
			//cout << "it works!";
		}
		else cout << "ERROR!";
	}

	WaitForMultipleObjects( 20,b, true, INFINITE );
	
	cout << "variable = " << var << endl;
	
	for (int i=0; i<20;i++)
	{
		CloseHandle(b[i]);
	}
	return 0;
}

