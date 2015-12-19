#include <vector>
using std::vector;

void getSwap_table(vector <int> &swap_table, const int nRows, int &count)
{
	FILE* interindex;
	fopen_s(&interindex, "interindex.bin", "r+b");

	vector <int> indeces;
	fread(&count, sizeof(int), 1, interindex);
	//count = 2;
	indeces.resize(count);
	fread(indeces.data(), sizeof(int), count, interindex); 
	//indeces = { 0, 3 };
	swap_table.resize(nRows);

	int index = 0;
	int i = 0;
	int flag = 0;
	for (int j = 0; j < nRows; j++) {
		flag = 0;
		i = 0;
		while (i < count) {
			if (indeces[i] == j) {
				flag = 1;
				break;
			}
			i++;
		}
		if (flag == 1) continue;
		swap_table[j] = index++;
	}

	for (int i = 0; i < count; i++) {
		swap_table[indeces[i]] = index++;
	}

	//
	//for (int j = 0; j < indeces[0]; j++){
	//	swap_table[index++] = j;
	//}

	//for (int i = 0; i < count-1; i++) {
	//	for (int j = indeces[i] + 1; j < indeces[i + 1]; j++) {
	//		swap_table[index++] = j;
	//	}
	//}

	//for (int j = indeces[count-1] + 1; j < nRows; j++){
	//	swap_table[index++] = j;
	//}

	//for (int i = 0; i < count; i++) {
	//	swap_table[index++] = indeces[i];
	//}
	//////////////////////////////
	//swap_table = {3, 0, 1, 4, 2};
}