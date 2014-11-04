#pragma once
#include "stack.h"
#include "string"
#include "sstream"

using namespace std;
class polish
{
public:
	polish(void)
	{
	}
	int Prior (char a)
	{
		switch (a)
		{
		case '*':
		case '/': 
			return 3;
		case '+':
		case '-':
			return 2;
		case'(':
			return 1;
		default:
			return 0;
		}
	}

	string Polish_Method(string input)
	{
		string output="";
		stack stack;
		for(int i=0;i<input.length();i++)
		{
			if (input[i]>48) 
				output+=input[i];
			else
			{
				if ((input[i]=='(') || (Prior(input[i])>Prior(stack.Watch()))) stack.Push(input[i]);
				else{
					if (input[i]==')')
					{
						while((stack.Watch()!='(') && (!stack.Is_empty()))
						{
							output+=stack.Pop();
						}
						if (stack.Watch()=='(') 
						{
								stack.Pop();
								continue;
						}
					}

					while((Prior(input[i])<=Prior(stack.Watch())) && (!stack.Is_empty()))
					{
						output+=stack.Pop();
						if ((Prior(input[i])>=Prior(stack.Watch())) || (!stack.Is_empty())) 
						{
							stack.Push(input[i]);					
							break;;
						}
					}
					
				}		
			}
		}
		if (!stack.Is_empty()) output+=stack.Pop();
		return output;
	}
		
};

