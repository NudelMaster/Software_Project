#ifndef HW_FINAL_MATRIX_H
#define HW_FINAL_MATRIX_H

/*
Reads from line and inputs into mat_line_ptr. Returns 0 if succeeded, 1 if failed.

Input:
    char *line: the string line to be read through
    double **mat_line_ptr: a pointer to the row in the matrix to be filled
    int dimension: number of coords in line
*/
int read_line(char *line, double **mat_line_ptr, int dimension);

/*
Reads matrix from file, where each cell is seperated by ",", and each line is seperated by "\n", updates it in mat_ptr,
and puts its dimensions in sizes, such that shape[0] = rows, shape[1] = columns. Returns 0 if succeeded, 1 if failed.

Input:
    char *file_name: the path to the file containing the matrix
    double ***mat_ptr: a pointer to the empty matrix
    int *shape: array that will contain shape of read matrix
*/
int read_matrix(const char *file_name, double ***mat_ptr, int *shape);

/*
Frees dynamically allocated matrix from memory.

Input:
    double **mat: the matrix to be freed
    int n: the number of rows in mat
*/
void free_matrix(double **mat, int n);

/*
Returns an n*k matrix filled with zeros.

Input:
    int n: the desired number of rows
    int k: the desired number of columns
*/
double **gen_matrix(int n, int k);

/*
Multiplies n*k matrix mat1 by k*m matrix mat2, and returns the result.

Input:
    double **mat1: an n*k matrix
    double **mat2: a k*m matrix
    int n: the number of rows in mat1
    int k: the number of columns in mat1, equal to the number of rows in mat2
    int m: the number of columns in mat2
*/
double **mat_mult(double **mat1, double **mat2, int n, int k, int m);

/*
Subtracts mat2 from mat1, and returns the result.

Input:
    double **mat1: an n*k matrix
    double **mat2: an n*k matrix
    int n: the number of rows in mat1, mat2
    int k: the number of columns in mat1, mat2
*/
double **mat_sub(double **mat1, double **mat2, int n, int k);

/*
Transposes mat, and returns the transposed matrix.

Input:
    double **mat: an n*k matrix
    int n: the number of rows in mat
    int k: the number of columns in mat
*/
double **transpose(double **mat, int n, int k);

/*
Calculates and returns the squared Frobenius distance between mat1 and mat2.

Input:
    double **mat1: an n*k matrix
    double **mat2: an n*k matrix
    int n: the number of rows in mat1, mat2
    int k: the number of columns in mat1, mat2
*/
double frobenius_dist_sq(double **mat1, double **mat2, int n, int k);

#endif
