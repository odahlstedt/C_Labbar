#include <stdio.h>  // standard input output
#include <stdlib.h> // standard bibliotek typ Null, mallo, calloc
#include <math.h>   // behövs för INFINITY

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
} ;

void print(simplex_t *s);
int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var);

int	global_array[10];

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

    for (i = 0; i < 11; i += 1)
	global_array[i] = i;


    if (s->var == NULL)
    {
        s->var = calloc(m+n+1, sizeof(int));
        for (int i = 0;i < m+n; i++) ///////////////////////////////har ändrats saknar int i = 0;
        {
            s->var[i] = i;
        }
        
        
    }
    for (k = 0, i = 1; i < m; i++)
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
    for (i = 0; i < n; i++) /// här kan fel göras
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


void prepare(simplex_t *s, int k)
{
    int m = s->m;
    int n = s->n;
    int i;

    // make room for xm+n at s.var[n] by moving s.var[n..n+m-1] one
    // step to the right.
    for (i = m + n; i > n; i --)
    {
        s->var[i] = s->var[i-1];
    }
    s->var[n] = m + n;
    // add xm+n to each constraint
    n ++;

    for (i = 0; i < m; i++)
    {
        s->a[i][n-1] = -1;
    }
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n-1] = -1;
    s->n = n;
    pivot(s, k, n-1);
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
   print(&s); 
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
        print(&s); 
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


int initial(simplex_t *s, int m, int n, double **a, double *b, double *c, double *x, double y, int *var)
{
    // tänk över de parametrar som skickas med, bör väll ändå användas
    int i, j, k;
    double w;
    k = init(s, m, n, a, b, c, x, y, var);
    if (b[k] >= 0)
    {
        return 1;
    }
    prepare(s, k);
    n = s->n;
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1); /// testa bara b istället för s->a

    for (i = 0; i < m + n; i++)
    {
        if (s->var[i] == m + n - 1)
        {
            if (fabs(s->x[i]) > epsilon)
            {
                free(s->x);
                free(s->c);
                return 0; // infeasible
            }
            else
            {
                break; // This i will be used on the next page
            }
        }
    }
    // ________________________________________________
    if (i >= n)
    // xn+m is basic. find good nonbasic.
    {
        for (j = k = 0; k < n; k++)
        {
            if (fabs(s->a[i-n][k]) > fabs(s->a[i-n][j]))
            {
                j = k;
            }
        }
        pivot(s, i-n, j);
        i = j;
    }

    if (i < n-1)
    {
        // xn+m is nonbasic and not last. swap columns i and n-1
        k = s->var[i];
        s->var[i] = s->var[n-1];
        s->var[n-1] = k;
        for (k = 0; k < m; k++)
        {
            w = s->a[k][n-1];
            s->a[k][n-1] = s->a[k][i];
            s->a[k][i] = w;
        }
    }
    else
    {
        // xn+m is nonbasic and last. forget it.
    }
    free(s->c);
    s->c = c;
    s->y = y;
    for (k = n-1; k < n + m - 1; k++)
    {
        s->var[k] = s->var[k+1];
    }

    n = s->n = s->n - 1;
    double *t = calloc(n, sizeof(double));

    for (k = 0; k < n; k++)
    {
        for (j = 0; j < n; j++)
        {
            if (k == s->var[j])
            {
                // xk is nonbasic. add c
                t[j] = t[j] + s->c[k];
                goto next_k;
            }
        }
        // xk is basic.
        for(j = 0; j < m; j++){
            if(s->var[n+j] == k){
                // xk is at row j
                break;
            }
        }
        s->y = s->y + s->c[k] * s->b[j];
        for(i = 0; i < n; i++){
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }
        next_k:;
    }
    for(i = 0; i < n; i++){
        s->c[i] = t[i];
    }
    free(t);
    free(s->x);
    return 1;
}



double simplex(int m, int n, double **a, double *b, double *c, double *x, double y)
{
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

void print_eq(int m, int n, double *c, double **a, double *b) // behövs egentligen inte
{
    printf("--------------------------------------------------------------");
    printf("\n");
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

void print(simplex_t *s){
    int m = s->m;
    int n = s->n;
    double* c = s->c;
    double **a = s->a;
    double *b = s->b;
    print_eq(m, n, c, a, b);
}

int main(int argc, char **argv)
{
    int m, n; // m = constraints, n = decision variables
    double *c;
    double **a;
    double *b;

    scanf("%d %d/n", &m, &n);

    c = calloc(n, sizeof(double)); // a vector with n c-coefficients
    a = calloc(m, sizeof(double *)); //  a matrix with m rows and n columns,
    b = calloc(m, sizeof(double)); // a vector with m b-values

    for (int i = 0; i < n; i++) // scan values to c-vector
        scanf("%lf", &c[i]);

    for (int i = 0; i < m; i++)
        a[i] = (double *)calloc(n + 1, sizeof(double));  // var här mitt fel låg! n+1


    for (int i = 0; i < m; i++) // gives values to a-matrix
        for (int j = 0; j < n; j++)
            scanf("%lf", &a[i][j]);

    for (int i = 0; i < m; i++) // gives value to b-vector
        scanf("%lf", &b[i]);
   
    double* x = calloc(n + m + 1, sizeof(double));

    print_eq(m, n, c, a, b);
    double res = simplex(m, n, a, b, c, x, 0); // är storleken rätt?

    printf(" res = %1.3lf", res);
    printf("\n");
    free(x);
    for (int i = 0; i < m; i++)
    {
        free(a[i]);
    }
    free(a);
    free(c);
    free(b);
}
