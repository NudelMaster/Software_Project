#include "symnmf.h"
#include "matrix.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define err_ddg { free_matrix(a, n); free_matrix(d, n); return NULL; }
#define err_norm { free_matrix(a, n); free_matrix(d, n); free_matrix(tmp_w, n); free_matrix(w, n); return NULL; }
#define err_symnmf { free_matrix(w_h, n); free_matrix(h_tr, k); free_matrix(h_htr, n); free_matrix(h_htr_h, n); free_matrix(next_h, n); return NULL; }
#define err_main { free_matrix(points, size); free_matrix(res, size); printf("%s", error_msg); return 1; }

/*
Helper functions that calculates and returns the squared Euclidean distance between point1 and point2.

Input:
    double *point1: an array representing a point in dimension dim
    double *point2: an array representing a point in dimension dim
    int dim: the dimension of the points
*/
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
        for (j = 0; j < dim; ++j) sum += a[i][j];
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

    w = gen_matrix(n, n);
    if (w == NULL) err_norm

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
    char *line = NULL;
    FILE *fptr;
    size_t len = 0;
    ssize_t read;
    int i, j, dimension = 1, size = 256;
    double **points = NULL, **res = NULL;

    if (argc < 2) err_main

    goal = argv[1];
    file_name = argv[2];

    fptr = fopen(file_name, "r");
    if (fptr == NULL) err_main

    points = calloc(size, sizeof(double *));
    if (points == NULL) err_main

    read = getline(&line, &len, fptr);
    if (read == -1) err_main

    /* Checks how many "," are in a line. This will be the dimension of each point! */
    for (i = 0; line[i]; i++) if (line[i] == ',') dimension++;

    i = 0;
    do {
        char *line_ptr;

        if (i >= size) {
            double **tmp;
            size *= 2;
            tmp = realloc(points, size * sizeof(double *));
            if (tmp == NULL) err_main
            points = tmp;
        }

        points[i] = calloc(dimension, sizeof(double));
        if (points[i] == NULL) err_main
        line_ptr = line;
        for (j = 0; j < dimension; j++) {
            points[i][j] = strtod(line_ptr, &line_ptr);
            /* Moves past the comma */
            if (*line_ptr == ',') line_ptr++;
        }
        i++;
    } while (getline(&line, &len, fptr) != -1);
    size = i;
    free(line);
    fclose(fptr);

    if (strcmp(goal, "sym") == 0) {
        res = sym(points, size, dimension);
    } else if (strcmp(goal, "ddg") == 0) {
        res = ddg(points, size, dimension);
    } else if (strcmp(goal, "norm") == 0) {
        res = norm(points, size, dimension);
    }

    if (res == NULL) err_main

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            printf("%.4f", res[i][j]);
            if (j != size - 1) printf(",");
        }
        printf("\n");
    }

    free_matrix(points, size);
    free_matrix(res, size);

    return 0;
}
