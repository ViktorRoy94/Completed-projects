#include <string>
using std::string;

struct node // структура для представления узлов дерева
{
    string S;
    unsigned char height;
    node* left;
    node* right;

    node(string s) 
	{ 
		S = s; 
		left = right = 0; 
		height = 1; 
	}
};