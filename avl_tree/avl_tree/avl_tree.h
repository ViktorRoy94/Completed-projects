#pragma once
#include "node.h"

class avl_tree
{
public:
	avl_tree(){}

private:
	unsigned char height(node* p)
	{
		unsigned char tmp = 0;
		if (p)
			tmp = p->height;
		else 
			tmp = 0 ;
		return tmp;
	}
	
	int bfactor(node* p)
	{
		int tmp = height(p->right)-height(p->left);
		return tmp;
	}

	void fixheight(node* p)
	{
		unsigned char hleft = height(p->left);
		unsigned char hright = height(p->right);
		if (hleft > hright)
			p->height = hleft + 1;
		else
			p->height = hright + 1;
	}
	
	node* rotateright(node* p) // правый поворот вокруг p
	{
		node* q = p->left;
		p->left = q->right;
		q->right = p;
		fixheight(p);
		fixheight(q);
		return q;
	}

	node* rotateleft(node* q) // левый поворот вокруг q
	{
		node* p = q->right;
		q->right = p->left;
		p->left = q;
		fixheight(q);
		fixheight(p);
		return p;
	}

	node* balance(node* p) // балансировка узла p
	{
		fixheight(p);
		if( bfactor(p)==2 )
		{
			if( bfactor(p->right) < 0 )
				p->right = rotateright(p->right);
			return rotateleft(p);
		}
		if( bfactor(p)==-2 )
		{
			if( bfactor(p->left) > 0  )
				p->left = rotateleft(p->left);
			return rotateright(p);
		}
		return p; // балансировка не нужна
	}
	
public:
	node* insert(node* p, string s) // вставка ключа k в дерево с корнем p
	{	
		if( !p ) return new node(s);
		if( s < p->S )
			p->left = insert(p->left, s);
		else
			p->right = insert(p->right, s);
		return balance(p);
	}

	node* search(node* p, string s)
	{
		node* res = p;
		while (true) {
			if (s < res->S)
			{
				if (res->left == nullptr)
					return nullptr;
				res = res->left;
			}
			else if (s > res->S)
			{
				if (res->right == nullptr)
					return nullptr;
				res = res->right;
			}
			else if (s == res->S)
				return res;
		}
		//if( !p ) return 0;
		//node *x = p;
		//while ( x != nullptr && x->S != s)
		//{
		//	if(x->S > s) {
		//		x = search (x->left, s);
		//	}
		//	else 
		//		x = search (x->right, s);
		//}
		//return x;
	}

	~avl_tree()
	{
	}
};

