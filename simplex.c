#include <stdio.h>  // standard input output
#include <stdlib.h> // standard bibliotek typ Null, mallo, calloc
#include <math.h> // behövs för INFINITY

double epsilon = 1e-6;

typedef struct simplex_t simplex_t;
struct simplex_t
{
    int m;      /* Constraints. */
    int n;      /* Decision variables. */
    int *var;   /* 0..n 1 are nonbasic. */
    double **a; /* A. */
    double *b;  /* b. */
    double *x;  /* x. */
    double *c;  /* c. */
    double y;   /* y. */
};

int init(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{
    int i, k;
    s->m = m; // förkortning av (*s).m = m;
    s->n = n;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;
    s->var = var;
    if (s->var == NULL)
    {
        s->var = calloc(m + n + 1, sizeof(int));
        for (i = 0; i < m + n; i++)
        {
            s->var[i] = i;
        }
    }
    for (k = 0, i = 0; i < m; i++)
    {
        if (b[i] < b[k])
        {
            k = i;
        }
    }
    return k;
}

int select_nonbasic(simplex_t *s)
{
    int i;
    for (i = 0; i < s->n; i++)
    {
        if (s->c[i] > epsilon)
        {
            return i;
        }
    }
    return -1;
}

int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{
    int i, j, k;
    double w;
    k = init(s, m, n, a, b, c, x, y, var);
    return 1;
}

void pivot(simplex_t *s, int row, int col)
{
    double **a = s->a;
    double *b = s->b;
    double *c = s->c;
    int m = s->m;
    int n = s->n;
    int i, j, t;
    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];
    for (i = 0; i < n; i++)
    {
        if (i != col)
        {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = -c[col] / a[row][col];
    for (i = 0; i < m; i++)
    {
        if (i != row)
        {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }
    for (i = 0; i < m; i++)
    {
        if (i != row)
        {
            for (j = 0; j < n; j++)
            {
                if (j != col)
                {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }
    for (i = 0; i < m; i++)
    {
        if (i != row)
        {
            a[i][col] = -a[i][col] / a[row][col];
        }
    }
    for (i = 0; i < n; i++)
    {
        if (i != col)
        {
            a[row][i] = a[row][i] / a[row][col];
        }
    }
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
}

double xsimplex(int m, int n, double **a, double *b, double *c, double *x, double y, int *var, int h)
{
    simplex_t s;
    int i, row, col;
    if (!initial(&s, m, n, a, b, c, x, y, var))
    {
        free(s.var);
        return NAN;
    }
    while ((col = select_nonbasic(&s)) >= 0)
    {
        row = -1;

        for (i = 0; i < m; i++)
        {
            if (a[i][col] > epsilon && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col]))
            {
                row = i;
            }
        }
        if (row < 0)
        {
            free(s.var);
            return INFINITY;
        }
        pivot(&s, row, col);
    }

    if (h == 0)
    {
        for (i = 0; i < n; i++)
        {
            if (s.var[i] < n)
            { // varför s.var och inte s->var svar: vi håller ju i själva objketet i s och inte adressen
                x[s.var[i]] = 0;
            }
        }
        for (i = 0; i < m; i++)
        {
            if (s.var[n + 1] < n)
            {
                x[s.var[n + 1]] = s.b[i];
            }
        }
        free(s.var);
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            x[i] = 0;
        }
        for (i = n; i < n + m; i++)
        {
            x[i] = s.b[i - n];
        }
    }
    return s.y;
}

double simplex(int m, int n, double **a, double *b, double *c, double *x, double y)
{
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

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
    double *x = calloc(m, sizeof(double)); // vad är x för nått?
    double res = simplex(m, n, a, b, c, x, 0);
    printf("%lf", res);

    free(x);
    for (int i = 0; i < m; i++)
    {
        free(a[i]);
    }
    free(a);
    free(c);
    free(b);
}