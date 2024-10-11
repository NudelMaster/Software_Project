#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define err_read { free_matrix(points, size); free_matrix(res, size); return 1; }

void free_matrix(double **mat, int n) {
    int i;
    if (mat == NULL) return;
    for (i = 0; i < n; ++i) free(mat[i]);
    free(mat);
}

int read_matrix(char *file_name, double ***mat_ptr, int *shape) {
    char *line = NULL;
    FILE *fptr;
    size_t len = 0;
    ssize_t read;
    int i, j, size = 256, dimension = 1;

    fptr = fopen(file_name, "r");
    if (fptr == NULL) return 1;

    read = getline(&line, &len, fptr);
    if (read == -1) return 1;

    /* Checks how many "," are in a line. This will be the dimension of each point! */
    for (i = 0; line[i]; i++) if (line[i] == ',') dimension++;

    i = 0;
    do {
        char *line_ptr;

        if (i >= size) {
            double **tmp;
            size *= 2;
            tmp = realloc(mat, size * sizeof(double *));
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
