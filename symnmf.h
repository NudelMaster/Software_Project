#ifndef HW_FINAL_SYMNMF_H
#define HW_FINAL_SYMNMF_H

/*
Helper function that calculates and returns the squared Euclidean distance between point1 and point2.

Input:
    double *point1: an array representing a point in dimension dim
    double *point2: an array representing a point in dimension dim
    int dim: the dimension of the points
*/
double euc_dist_sq(double *point1, double *point2, int dim);

/*
Calculates and returns the similarity matrix.

Input:
    double **x: the data matrix X, n*k matrix
    int n: the number of data points, rows in X
    int dim: the dimension of the data points, columns in X
*/
double **sym(double **x, int n, int dim);

/*
Calculates and returns the diagonal degree matrix.

Input:
    double **x: the data matrix X, n*k matrix
    int n: the number of data points, rows in X
    int dim: the dimension of the data points, columns in X
*/
double **ddg(double **x, int n, int dim);

/*
Calculates and returns the normalized similarity matrix.

Input:
    double **x: the data matrix X, n*k matrix
    int n: the number of data points, rows in X
    int dim: the dimension of the data points, columns in X
*/
double **norm(double **x, int n, int dim);

/*
Runs the full symNMF algorithm until the convergence is up to epsilon.

Input:
    double **h: the initial randomized matrix H, n*k matrix
    double **w: the graph Laplacian matrix W, n*n matrix
    int n: the number of data points, rows in H
    int k: the dimension of the data points, columns in H
*/
double **symnmf(double **h, double **w, int n, int k);

#endif
