#include <stdio.h>
#include <stdlib.h>

void print_Equations(int m, int n, double *c, double **a, double *b)
{
	// första raden dvs max z =....
	printf("     max z        =%10.3lfx_%d", c[0], (int)0);
	for (int j = 1; j < n; j++)
	{ // n st kolumner
		printf("     +  %10.3lfx_%d", c[j], j);
	}
	printf("\n");

	for (int i = 0; i < m; i++)
	{ // bör bli m+1 rader
		// printf("%10.3lfx_0", a[0][0]);
		for (int j = 0; j < n; j++)
		{ // n st kolumner
			printf("%10.3lfx_%d", a[i][j], j);
			if (j < n - 1)
			{
				printf("     +");
			}
			else
			{
				printf("     \u2264  %10.3lf", b[i]);
			}
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int m, n; // m = constraints, n = decision variables
	double *c;
	double **a;
	double *b;

	scanf("%d %d/n", &m, &n);

	c = (double *)calloc(n, sizeof(double)); // a vector with n c-coefficients

	a = (double **)calloc(m, sizeof(double *)); //  a matrix with m rows and n columns,
	for (int i = 0; i < m; i += 1)
		a[i] = (double *)calloc(n, sizeof(double));

	b = (double *)calloc(m, sizeof(double)); // a vector with m b-values

	for (int i = 0; i < n; i++) // scan values to c-vector
		scanf("%lf", &c[i]);

	for (int i = 0; i < m; i++) // gives values to a-matrix
		for (int j = 0; j < n; j++)
			scanf("%lf", &a[i][j]);

	for (int i = 0; i < m; i++) // gives value to b-vector
		scanf("%lf", &b[i]);

	print_Equations(m, n, c, a, b);

	for (int i = 0; i < m; i++)
	{
		free(a[i]);
	}
	free(a);
	free(c);
	free(b);

	/* m =2, n=2;
	 c[0] = 1, c[1] = 2;
	 a[0][0] = -0.5, a[0][1] = 1;
	 a[1][0] = 3, a[1][1] =1;
	 b[0] = 4, b[1] = 18;  */
}
