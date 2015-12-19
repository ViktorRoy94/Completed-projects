
void saxpy(int n, float a, float * x, int incx, float * y, int incy);
void daxpy(int n, double a, double * x, int incx, double * y, int incy);

void saxpy_gpu(int n, float a, float * x, int incx, float * y, int incy);
void daxpy_gpu(int n, double a, double * x, int incx, double * y, int incy);

void saxpy_openmp(int n, float a, float * x, int incx, float * y, int incy);
void daxpy_openmp(int n, double a, double * x, int incx, double * y, int incy);
