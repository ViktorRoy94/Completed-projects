// Polish_method_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "polish.h"
#include "iostream"

int main()
{
	string input="a*(a+c+b+d*e)/q";
	polish a;
	string output = a.Polish_Method(input);
	cout <<output << endl;
	system("pause");
	return 0;
}

