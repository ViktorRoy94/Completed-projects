#include <string>
using std::string;

struct node // ��������� ��� ������������� ����� ������
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