#include "avl_tree.h"
#include "generateInput.h"
#include "portable_time.h"
#include "binarySearch.h"
#include <mpi.h>
#include <vector>
#include <string>
using std::vector;
using std::string;

int main (int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int m = 26;    // к-во букв в алфавите
	int k = 100;    // к-во слов в последовательности
	int n = 80000; // к-во слов в словаре
	int s = 7;     // к-во букв в словах
	vector <string> S(n); // слова в словаре
	vector <char> A;
	A = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};   // буквы в алфавите
	generateS(S, A, n, m, s);

	FILE* output;
	fopen_s(&output, "output.txt", "w+");

	while (k < n) {
		vector <string> P(k); // последовательность слов
		generateP(P, A, k, m, s);

		node* head = nullptr;
		avl_tree dictionaryAvl;
		vector <string> dictionaryList(n);
		vector <string> dictionarySortList(n);

		generateAvl(dictionaryAvl, head, S, n);
		//generateList(dictionaryList, S, n);
		generateSortList(dictionaryList, dictionarySortList, n);

		double timeAvl = MPI_Wtime();
		for (int i = 0; i < k; i++) {
			node *x = dictionaryAvl.search(head, P[i]);
		}
		timeAvl = MPI_Wtime() - timeAvl;

		double timeList = MPI_Wtime();
		//for (int i = 0; i < k; i++) {
		//	int j = 0;
		//	while ((j < n) && (dictionaryList[j] != P[i])) {
		//		j++;
		//	}
		//}
		//timeList = MPI_Wtime() - timeList;

		double timeSortList = MPI_Wtime();
		for (int i = 0; i < k; i++) {
			binarySearch(dictionarySortList, P[i], n);
		}
		timeSortList = MPI_Wtime() - timeSortList;

		fprintf(output, "k = %d timeAvl = %lf timeList = %lf timeSortList = %lf\n", k, timeAvl, timeList, timeSortList);
		printf(" k = %d\n", k);
		k += 500;
	}
	fclose(output);
	MPI_Finalize();
	return 0;	
}
