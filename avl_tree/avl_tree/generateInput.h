#include <vector>
#include <string>
using std::vector;
using std::string;

int comp_string(const void*pa, const void*pb);

void generateS(vector <string> &S, vector <char> &A, int n, int m, int s)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < s; j++) {
			S[i] += A[rand() % m];
		}
	}
}

void generateP( vector <string> &P, vector <char> &A, int k, int m, int s) {
	for (int i = 0; i < k; i++) {
		for (int j = 0; j < s; j++) {
			P[i] += A[rand() % m];
		}
	}
}

void generateAvl(avl_tree &dictionaryAvl, node* &head, vector <string> &S, int n)
{
	for (int i = 0; i < n; i++) {
		 head = dictionaryAvl.insert(head, S[i]);
	}
}

void  generateList(vector <string> &dictionaryList, vector <string> &S, int n) {
	for (int i = 0; i < n; i++) {
		dictionaryList[i] = S[i];
	}
}

void generateSortList(vector <string> &dictionaryList, vector <string> &dictionarySortList, int n) {
	qsort(dictionaryList.data(), n, sizeof(string), comp_string);
	dictionarySortList = dictionaryList;
}

int comp_string(const void*pa, const void*pb) {
	string a = *((const string*)pa);
	string b = *((const string*)pb);
	if (a<b)
		return -1;
	else
		return (a == b) ? (0) : (1);
}