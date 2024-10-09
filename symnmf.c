#include "symnmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void free_rec(double **mat, int n) {
    int i;
    if (mat == NULL) return;
    for (i = 0; i < n; ++i) free(mat[i]);
    free(mat);
}

double euc_dist_sq(double *point1, double *point2, int dim) {
    double sum = 0;
    int i;
    for (i = 0; i < dim; ++i) sum += pow(point1[i] - point2[i], 2);
    return sum;
}

/** multiply n*k matrix mat1 with k*m matrix mat2 */
double **mat_mult(double **mat1, double **mat2, int n, int k, int m) {
    double **res;
    int i, j, t;

    res = calloc(n, sizeof(double *));
    if (res == NULL) return NULL;
    for (i = 0; i < n; ++i) {
        res[i] = calloc(m, sizeof(double));
        if (res[i] == NULL) {
            free_rec(res, n);
            return NULL;
        }
    }

    for (i = 0; i < n; ++i) {
        for (j = 0; j < m; ++j) {
            double sum = 0;
            for (t = 0; t < k; ++t) sum += mat1[i][t] * mat2[t][j];
            res[i][j] = sum;
        }
    }

    return res;
}

double **sym(double **x, int n, int dim) {
    double **a;
    int i, j;

    a = calloc(n, sizeof(double *));
    if (a == NULL) return NULL;
    for (i = 0; i < n; ++i) {
        a[i] = calloc(n, sizeof(double));
        if (a[i] == NULL) {
            free_rec(a, n);
            return NULL;
        }
    }

    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            if (i != j)
                a[i][j] = exp(-(euc_dist_sq(x[i], x[j], dim) / 2));
        }
    }

    return a;
}

double **ddg(double **x, int n, int dim) {
    double **a, **d;
    int i, j;

    a = sym(x, n, dim);
    if (a == NULL) return NULL;

    d = calloc(n, sizeof(double *));
    if (d == NULL) {
        free_rec(a, n);
        return NULL;
    }
    for (i = 0; i < n; ++i) {
        d[i] = calloc(n, sizeof(double));
        if (d[i] == NULL) {
            free_rec(a, n);
            free_rec(d, n);
            return NULL;
        }
    }

    for (i = 0; i < n; ++i) {
        double sum = 0;
        for (j = 0; j < dim; ++j) sum += a[i][j];
        d[i][i] = sum;
    }

    free_rec(a, n);

    return d;
}

double **norm(double **x, int n, int dim) {
    double **a, **d, **tmp_w, **w;
    int i;

    a = sym(x, n, dim);
    if (a == NULL) return NULL;
    d = ddg(x, n, dim);
    if (d == NULL) {
        free_rec(a, n);
        return NULL;
    }

    w = calloc(n, sizeof(double *));
    if (w == NULL) {
        free_rec(a, n);
        free_rec(d, n);
        return NULL;
    }
    for (i = 0; i < n; ++i) {
        w[i] = calloc(n, sizeof(double));
        if (w[i] == NULL) {
            free_rec(a, n);
            free_rec(d, n);
            free_rec(w, n);
            return NULL;
        }
    }

    for (i = 0; i < n; ++i) d[i][i] = sqrt(1 / d[i][i]);
    tmp_w = mat_mult(d, a, n, n, n);
    w = mat_mult(tmp_w, d, n, n, n);

    free_rec(a, n);
    free_rec(d, n);
    free_rec(tmp_w, n);

    return w;
}
