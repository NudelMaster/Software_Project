#define PY_SSIZE_T_CLEAN
#include "Python.h"
/* TODO change this! */
#include "symnmf.h"
#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define symnmf_err { if (shape_h != NULL) free(shape_h); if (shape_w != NULL) free(shape_w); free_matrix(w, n); free_matrix(h, n); return NULL; }

double **decode_matrix(PyObject *p_matrix, int *shape) {
    int i, j, n, k;
    double **matrix;
    PyObject *p_row = NULL;

    n = PyObject_Length(p_matrix);
    if (n <= 0) return NULL;

    p_row = PyList_GetItem(p_matrix, 1);
    if (p_row == NULL) return NULL;
    k = PyObject_Length(p_row);
    if (k <= 0) return NULL;

    matrix = gen_matrix(n, k);

    for (i = 0; i < n; i++) {
        p_row = PyList_GetItem(p_matrix, i);
        if (p_row == NULL) return NULL;
        for (j = 0; j < k; j++) {
            PyObject *p_item = PyList_GetItem(p_row, j);
            matrix[i][j] = PyFloat_AsDouble(p_item);
        }
    }

    shape[0] = n;
    shape[1] = k;

    return matrix;
}

PyObject *encode_matrix(double **matrix, int n, int k) {
    int i, j;
    PyObject *p_matrix = NULL;

    p_matrix = PyList_New(n);
    for (i = 0; i < n; i++) {
        PyObject *p_row = PyList_New(k);
        for (j = 0; j < k; j++) {
            PyObject *p_value = Py_BuildValue("d", matrix[i][j]);
            PyList_SetItem(p_row, j, p_value);
        }
        PyList_SetItem(p_matrix, i, p_row);
    }

    free_matrix(matrix, n);

    return p_matrix;
}

PyObject *call_func_with_x(double **(*func)(double **, int, int), PyObject *args) {
    PyObject *p_points = NULL, *p_res = NULL;
    int n, k, *shape = NULL;
    double **points = NULL, **res = NULL;

    if (!PyArg_ParseTuple(args, "O", &p_points)) return NULL;

    shape = calloc(2, sizeof(int));
    if (shape == NULL) return NULL;

    points = decode_matrix(p_points, shape);
    n = shape[0], k = shape[1];
    free(shape);
    if (points == NULL) return NULL;

    res = func(points, n, k);
    free_matrix(points, n);
    if (res == NULL) return NULL;

    p_res = encode_matrix(res, n, n);

    return p_res;
}

static PyObject *SymNMFLib_SymNMF(PyObject *self, PyObject *args) {
    PyObject *p_h = NULL, *p_w = NULL, *p_res = NULL;
    int n = 0, k, *shape_h = NULL, *shape_w = NULL;
    double **h = NULL, **w = NULL, **res = NULL;

    if (!PyArg_ParseTuple(args, "OO", &p_h, &p_w)) symnmf_err

    shape_h = calloc(2, sizeof(int));
    shape_w = calloc(2, sizeof(int));
    if (shape_h == NULL || shape_w == NULL) symnmf_err

    h = decode_matrix(p_h, shape_h);
    n = shape_h[0], k = shape_h[1];
    w = decode_matrix(p_w, shape_w);
    if (w == NULL || h == NULL) symnmf_err

    res = symnmf(h, w, n, k);
    if (res == NULL) symnmf_err

    p_res = encode_matrix(res, n, k);

    free(shape_h);
    free(shape_w);
    free_matrix(h, n);
    free_matrix(w, n);

    return p_res;
}

static PyObject *SymNMFLib_Sym(PyObject *self, PyObject *args) {
    return call_func_with_x(&sym, args);
}

static PyObject *SymNMFLib_DDG(PyObject *self, PyObject *args) {
    return call_func_with_x(&ddg, args);
}

static PyObject *SymNMFLib_Norm(PyObject *self, PyObject *args) {
    return call_func_with_x(&norm, args);
}


static PyMethodDef SymNMFLib_FunctionsTable[] = {
    {
        "symnmf",
        SymNMFLib_SymNMF,
        METH_VARARGS,
        "Runs the full symNMF algorithm until the convergence is up to epsilon"
    },
    {
        "sym",
        SymNMFLib_Sym,
        METH_VARARGS,
        "Calculates and returns the similarity matrix"
    },
    {
        "ddg",
        SymNMFLib_DDG,
        METH_VARARGS,
        "Calculates and returns the diagonal degree matrix"
    },
    {
        "norm",
        SymNMFLib_Norm,
        METH_VARARGS,
        "Calculates and returns the normalized similarity matrix"
    },
    {
        NULL, NULL, 0, NULL
    }
};

static struct PyModuleDef SymNMF_Module = {
    PyModuleDef_HEAD_INIT,
    "symnmf",
    "Integrate SymNMF between Python and C",
    -1,
    SymNMFLib_FunctionsTable
};

PyMODINIT_FUNC PyInit_symnmf(void) {
    return PyModule_Create(&SymNMF_Module);
}
