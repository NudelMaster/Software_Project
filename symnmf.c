#include "symnmf.h"
#include "matrix.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define err_ddg { free_matrix(a, n); free_matrix(d, n); return NULL; }
#define err_norm { free_matrix(a, n); free_matrix(d, n); free_matrix(tmp_w, n); free_matrix(w, n); return NULL; }
#define err_symnmf { free_matrix(w_h, n); free_matrix(h_tr, k); free_matrix(h_htr, n); free_matrix(h_htr_h, n); free_matrix(next_h, n); return NULL; }
#define err_main { free(shape); free_matrix(points, shape[0]); printf("%s", error_msg); return 1; }

const double beta = 0.5;
const double eps = 0.0001;
const char *error_msg = "An Error Has Occurred\n";

double euc_dist_sq(double *point1, double *point2, int dim) {
    double sum = 0;
    int i;
    for (i = 0; i < dim; ++i) sum += pow(point1[i] - point2[i], 2);
    return sum;
}

double **sym(double **x, int n, int dim) {
    double **a;
    int i, j;

    a = gen_matrix(n, n);
    if (a == NULL) return NULL;

    for (i = 0; i < n; ++i) for (j = 0; j < n; ++j)
        if (i != j) a[i][j] = exp(-(euc_dist_sq(x[i], x[j], dim) / 2));

    return a;
}

double **ddg(double **x, int n, int dim) {
    double **a = NULL, **d = NULL;
    int i, j;

    a = sym(x, n, dim);
    if (a == NULL) err_ddg

    d = gen_matrix(n, n);
    if (d == NULL) err_ddg

    for (i = 0; i < n; ++i) {
        double sum = 0;
        for (j = 0; j < n; ++j) sum += a[i][j];
        d[i][i] = sum;
    }

    free_matrix(a, n);

    return d;
}

double **norm(double **x, int n, int dim) {
    double **a = NULL, **d = NULL, **tmp_w = NULL, **w = NULL;
    int i;

    a = sym(x, n, dim);
    if (a == NULL) err_norm

    d = ddg(x, n, dim);
    if (d == NULL) err_norm

    for (i = 0; i < n; ++i) if (d[i][i] > eps) d[i][i] = sqrt(1.0 / d[i][i]);
    tmp_w = mat_mult(d, a, n, n, n);
    if (tmp_w == NULL) err_norm
    w = mat_mult(tmp_w, d, n, n, n);
    if (w == NULL) err_norm

    free_matrix(a, n);
    free_matrix(d, n);
    free_matrix(tmp_w, n);

    return w;
}

double **symnmf(double **h, double **w, int n, int k) {
    double **w_h = NULL, **h_tr = NULL, **h_htr = NULL, **h_htr_h = NULL, **next_h = NULL, **res = NULL;
    int i, j;

    w_h = mat_mult(w, h, n, n, k);
    if (w_h == NULL) err_symnmf

    h_tr = transpose(h, n, k);
    if (h_tr == NULL) err_symnmf

    h_htr = mat_mult(h, h_tr, n, k, n);
    if (h_htr == NULL) err_symnmf
    free_matrix(h_tr, k);

    h_htr_h = mat_mult(h_htr, h, n, n, k);
    if (h_htr_h == NULL) err_symnmf
    free_matrix(h_htr, n);

    next_h = gen_matrix(n, k);
    if (next_h == NULL) err_symnmf

    for (i = 0; i < n; ++i) for (j = 0; j < k; ++j)
        next_h[i][j] = h[i][j] * (1 - beta + beta * (w_h[i][j] / h_htr_h[i][j]));

    if (frobenius_dist_sq(next_h, h, n, k) < eps) res = next_h;
    else {
        res = symnmf(next_h, w, n, k);
        free_matrix(next_h, n);
    }

    free_matrix(w_h, n);
    free_matrix(h_htr_h, n);

    return res;
}

int main(int argc, char *argv[]) {
    const char *goal, *file_name;
    int i, j, read_res, *shape = NULL;
    double **points = NULL, **res = NULL;

    if (argc < 2) return 1;

    goal = argv[1];
    file_name = argv[2];

    shape = calloc(2, sizeof(int));
    read_res = read_matrix(file_name, &points, shape);
    if (read_res == -1) err_main

    if (strcmp(goal, "sym") == 0) {
        res = sym(points, shape[0], shape[1]);
    } else if (strcmp(goal, "ddg") == 0) {
        res = ddg(points, shape[0], shape[1]);
    } else if (strcmp(goal, "norm") == 0) {
        res = norm(points, shape[0], shape[1]);
    }

    if (res == NULL) err_main

    for (i = 0; i < shape[0]; i++) {
        for (j = 0; j < shape[0]; j++) {
            printf("%.4f", res[i][j]);
            if (j != shape[0] - 1) printf(",");
        }
        printf("\n");
    }

    free_matrix(points, shape[0]);
    free_matrix(res, shape[0]);
    free(shape);

    return 0;
}
