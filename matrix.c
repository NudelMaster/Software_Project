#define _GNU_SOURCE
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void free_matrix(double **mat, int n) {
    int i;
    if (mat == NULL) return;
    for (i = 0; i < n; ++i) if (mat[i] != NULL) free(mat[i]);
    free(mat);
}

void realloc_matrix(double ***mat_ptr, int *size) {
    double **tmp;
    *size *= 2;
    tmp = realloc(&mat_ptr, (*size) * sizeof(double *));
    if (tmp == NULL) free_matrix(*mat_ptr, (*size) / 2);
    *mat_ptr = tmp;
}

int read_line(char *line, double **mat_line_ptr, int dimension) {
    char *line_ptr;
    int i;

    *mat_line_ptr = calloc(dimension, sizeof(double));
    if (*mat_line_ptr == NULL) return -1;
    line_ptr = line;
    for (i = 0; i < dimension; i++) {
        (*mat_line_ptr)[i] = strtod(line_ptr, &line_ptr);
        if (*line_ptr == ',') line_ptr++;
    }
    return 0;
}

int read_matrix(const char *file_name, double ***mat_ptr, int *shape) {
    char *line = NULL;
    FILE *fptr;
    size_t len = 0;
    int i, size = 256, dimension = 1;

    fptr = fopen(file_name, "r");
    if (fptr == NULL) return -1;
    if (getline(&line, &len, fptr) == -1) {
        fclose(fptr);
        free(line);
        return -1;
    }

    /* Checks how many "," are in a line. This will be the dimension of each point! */
    for (i = 0; line[i]; i++) if (line[i] == ',') dimension++;
    *mat_ptr = calloc(size, sizeof(double *));
    if (*mat_ptr == NULL) return -1;

    i = 0;
    do {
        if (i >= size) {
            realloc_matrix(mat_ptr, &size);
            if (*mat_ptr == NULL) return -1;
        }
        if (read_line(line, &((*mat_ptr)[i]), dimension) == -1) {
            free_matrix(*mat_ptr, size);
            return -1;
        }
        i++;
    } while (getline(&line, &len, fptr) != -1);
    free(line);
    fclose(fptr);

    shape[0] = i;
    shape[1] = dimension;
    return 0;
}

double **gen_matrix(int n, int k) {
    int i;
    double **mat;

    mat = calloc(n, sizeof(double *));
    if (mat == NULL) return NULL;
    for (i = 0; i < n; ++i) {
        mat[i] = calloc(k, sizeof(double));
        if (mat[i] == NULL) {
            free_matrix(mat, n);
            return NULL;
        }
    }

    return mat;
}

double **mat_mult(double **mat1, double **mat2, int n, int k, int m) {
    double **res;
    int i, j, t;

    res = gen_matrix(n, m);
    if (res == NULL) return NULL;

    for (i = 0; i < n; ++i) for (j = 0; j < m; ++j) {
            double sum = 0;
            for (t = 0; t < k; ++t) sum += mat1[i][t] * mat2[t][j];
            res[i][j] = sum;
        }

    return res;
}

double **mat_sub(double **mat1, double **mat2, int n, int k) {
    int i, j;
    double **res;

    res = gen_matrix(n, k);
    if (res == NULL) return NULL;

    for (i = 0; i < n; ++i) for (j = 0; j < k; ++j)
            res[i][j] = mat1[i][j] - mat2[i][j];

    return res;
}

double **transpose(double **mat, int n, int k) {
    int i, j;
    double **mat_tr;

    mat_tr = calloc(k, sizeof(double *));
    if (mat_tr == NULL) return NULL;
    for (i = 0; i < k; ++i) {
        mat_tr[i] = calloc(n, sizeof(double));
        if (mat_tr[i] == NULL) {
            free_matrix(mat_tr, k);
            return NULL;
        }
    }

    for (i = 0; i < n; ++i) for (j = 0; j < k; ++j)
            mat_tr[j][i] = mat[i][j];

    return mat_tr;
}

double frobenius_dist_sq(double **mat1, double **mat2, int n, int k) {
    int i, j;
    double **sub = NULL, sum = 0;

    sub = mat_sub(mat1, mat2, n, k);
    if (sub == NULL) return -1;

    for (i = 0; i < n; ++i) for (j = 0; j < k; ++j)
            sum += pow(sub[i][j], 2);
    free_matrix(sub, n);

    return sum;
}
