#include "Data3D.h"
#include "stdio.h"
#include <windows.h>

int main (int argc, char* argv[])
{
	int idx = 1;
	if (argc>1) idx=atoi(argv[1]);
	Data3D arrayData;
	arrayData.ReadData("head");
	
	arrayData.SaveLayer(idx,0,1000);

}
