#include <vector>
#include <string>
using std::vector;
using std::string;

int binarySearch(vector <string> &A, string key, int N)
{
	int left = 0;
	int right = N - 1;
	int mid;
	do{
		mid = left + (right - left) / 2;
		if (key < A[mid])
			right = mid - 1;
		else 
		if (key > A[mid])
			left = mid + 1;
		else
			break;
	} while (key != A[mid] && left <= right);
	return 0;
}