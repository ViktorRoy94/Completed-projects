#include <math.h>
#include <stdio.h>

double trapeze(double a, double b, int n);
double simpson(double a, double b, int n);
double f(double x);

int main()
{
	double a = -0.5;
	double b =  0.5;
	int    n = 100;

	printf("Method trapeze = %lf \n", trapeze(a, b, n));
	printf("Simpsons method = %lf \n", simpson(a, b, n));

	return 0;
}


double trapeze(double a, double b, int n)
{
	double h = (double)(b - a) / n;
	double S = 0;
	double x = a;
	while (x < b) {
		S += 0.5*(f(x + h) + f(x))*h;
		x += h;
	}
	return S;
}

double simpson(double a, double b, int n)
{
	double h = (double)(b - a) / (n);
	double S = 0;
	S += f(a) + f(b);
	for (int i = 1; i < n; i++) {
		if (i % 2 == 0) 
			S += 2 * f(a + i*h);
		else 
			S += 4 * f(a + i*h);
	}
	
	return S * (h / 3);
}

double f(double x)
{
	return (x*x*x) / sqrt(1 - x*x);
}
