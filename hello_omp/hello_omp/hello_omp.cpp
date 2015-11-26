#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

volatile DWORD dwStart;
volatile int global = 0;


double test(int num_steps) {
	int i;
	global++;
	double x, pi, sum = 0.0, step;

	step = 1.0 / (double)num_steps;

	for (i = 1; i <= num_steps; i++) {
		x = (i - 0.5) * step;
		sum = sum + 4.0 / (1.0 + x*x);
	}

	pi = step * sum;
	return pi;
}

double test2(int num_steps) {
	int i;
	global++;
	double x, pi, sum = 0.0, step;

	step = 1.0 / (double)num_steps;

#pragma omp parallel for reduction(+:sum) private(x)
	for (i = 1; i <= num_steps; i++) {
		x = (i - 0.5) * step;
		sum = sum + 4.0 / (1.0 + x*x);
	}

	pi = step * sum;
	return pi;
}

int main(int argc, char* argv[]) {
	double   d;
	int n = 10000000;

	if (argc > 1)
		n = atoi(argv[1]);

	dwStart = GetTickCount();
	d = test(n);
	printf_s("For %d steps, pi = %.15f, %d milliseconds\n", n, d, GetTickCount() - dwStart);

	dwStart = GetTickCount();
	d = test2(n);
	printf_s("For %d steps, pi = %.15f, %d milliseconds\n", n, d, GetTickCount() - dwStart);
	
	//omp_set_num_threads(10);
#pragma omp parallel for
	for (int i = 0; i < 30; i++)
	{
		int t_id = omp_get_thread_num();
		printf("hello from thread %d \n", t_id);
	}

	system("PAUSE");
}