#define BLOCK_SIZE 16

void multiplication(const int * a, const int * b, int * c, int m, int n, int k);
void multiplication_gpu(const int * a, const int * b, int * c, int m, int n, int k);
void multiplication_gpu_opt(const int * a, const int * b, int * c, int m, int n, int k);
