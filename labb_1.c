#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	int m, n;  // m = constraints, n = decision variables
		  scanf("%d %d/n", &m, &n);

	        int *shiftregs;
	            shiftregs = calloc(1, sizeof(int));

	    double* c = (double*)calloc(n, sizeof(double)); // a vector with n c-coefficients

		double** a; //  a matrix with m rows and n columns,
		int i;
		a = calloc(m, sizeof(double*));
		for (i = 0; i < m; i += 1)
		a[i] = calloc(n, sizeof(double));

		double* b = (double*)calloc(m, sizeof(double)); // a vector with m b-values

		scanf("%lf %lf/n", &c[0], &c[1]);
		scanf("%lf %lf/n", &a[0][0], &a[1][0]);
		scanf("%lf %lf/n", &a[1][0], &a[1][1]);
		scanf("%lf %lf/n", &b[0], &b[1]);



	   /* m =2, n=2;
	    c[0] = 1, c[1] = 2;
		a[0][0] = -0.5, a[0][1] = 1;
		a[1][0] = 3, a[1][1] =1;
		b[0] = 4, b[1] = 18;  */


}
