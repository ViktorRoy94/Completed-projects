#include <vector>
#include <complex>

using std::vector;
using std::complex;

void getShurTest(vector <complex<double>> &shurTest, int count) {
	FILE* shur;
	fopen_s(&shur, "schur.bin", "rb");
	fread(shurTest.data(), sizeof(complex<double>), count * count, shur);
}