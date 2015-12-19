#pragma once
#include "stack.h"
#include "string"
#include "sstream"
#include "stack_numeral.h"
#include <iostream>

using namespace std;
class polish
{
public:
	polish(void)
	{
	}
	string polishMethod(string input)
	{
		string output="";
		stack stack;
		for(int i=0;i<input.length();i++)
		{
			if ((input[i]=='s') && (input[i+1]=='i') && (input[i+2]=='n') && (input[i+3]=='('))
			{
				int b=i+4; // записываем значение i, когда происходит появление sin()
				while(input[i]!=')')
				{
					i++;
				}
				if (input[i]==')')
				{
					string arg_sin;
					arg_sin=input.substr(b,i-b);
					output+=polishMethod(arg_sin);
					output+="sin";
				}
			}
			if ((input[i]=='c') && (input[i+1]=='o') && (input[i+2]=='s') && (input[i+3]=='('))
			{
				int b=i+4; // записываем значение i, когда происходит появление sin()
				while(input[i]!=')')
				{
					i++;
				}
				if (input[i]==')')
				{
					string arg_cos;
					arg_cos=input.substr(b,i-b);
					output+=polishMethod(arg_cos);
					output+="cos";
				}
			}
			
			if (((input[i]>47) && (input[i]<58)) || (input[i]==116))
			{
				output+=input[i];
				if (!((input[i+1]>47) && (input[i+1]<58)))
				{
					output+=' ';
					continue;
				}
				if (input[i]==116)
				{
					output+=input[i];
					output+=' ';
				}
			}
			else
			{
				if ((input[i]=='(') || (Prior(input[i])>Prior(stack.Watch()))) stack.Push(input[i]);
				else{
					if (input[i]==')')
					{
						while((stack.Watch()!='(') && (!stack.Is_empty()))
						{
							output+=stack.Pop();
							output+=' ';
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
						output+=' ';
						if ((Prior(input[i])>=Prior(stack.Watch())) || (!stack.Is_empty())) 
						{
							stack.Push(input[i]);					
							break;
						}
					}
					
				}		
			}
		}
		while (!stack.Is_empty()) 
		{
			output+=stack.Pop();
			output+=' ';
		}
		return output;
	}
private:
	int Prior (char a)
	{
		switch (a)
		{
		case'sin':
		case'cos':
			return  5;
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
	string Insert ( const string str, const string t) // str - изначальная строка, где ищем t
	{ 
		string tmp = str.c_str();
		for( int i = tmp.find("t"); i < tmp.find_last_of("t")+1; i++)
		{
			if(tmp[i] == 't')
			tmp.replace(tmp.find('t'),1,t);
		}
		return tmp;
	}
public:
	double reversePolishMethod(string input,int t)
	{
		stack1 stack;
		double n1,n2,res;

		string str; //промежуточный рез-т	
		string tmp1=input; // сохраняю исходную строку в tmp1, чтобы можно было менять input

		std::ostringstream ost; //
		ost << t; // магическое превращение int в std::string
		str = ost.str();// в a лежит t
		
		input=Insert(tmp1,str); // ф-ция, которая ищет t в input и вставляет вместо него a
		string a;
		const int pi=180; 

		for(int i=0; i<input.length(); i++)
		{
			if ((input[i]=='c') && (input[i+1]=='o') && (input[i+2]=='s'))
			{
				i+=2;
				stack.Push(cos(stack.Pop()*pi/180));
				continue;
			}

			if ((input[i]=='s') && (input[i+1]=='i') && (input[i+2]=='n'))
			{
				i+=2;
				stack.Push(sin(stack.Pop()*pi/180));
				continue;
			}
			if ((input[i]>47) && (input[i]<58) || (input[i]==' ')) 
			{
				if (input[i]==' ') 
				{
					stack.Push(atof(a.c_str()));
					a.erase(0,i);
					continue;
				}	
				a+=input[i];
				
			}
			else
			{   
				n2=stack.Pop();  
				n1=stack.Pop();	
				switch(input[i])
				{
					case '+': res=n1+n2; break;
					case '-': res=n1-n2; break;
					case '*': res=n1*n2; break;
					case '/': res=n1/n2; break;
					default: return NULL;
				}
				stack.Push(res);
				i++;  // пропускаем проблел после знака * a b * c
				continue;
			}
		}
		return stack.Pop();	

	}
};

