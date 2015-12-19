// Polish_Method_Local.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "polish.h"
#include "iostream"


int main()
{
	string input="t";
	polish a;
	string output = a.polishMethod(input);
	cout <<output << endl;
	cout << a.reversePolishMethod(output,10)<<endl;

	system("pause");
	return 0;
}

