#define PY_SSIZE_T_CLEAN
/* TODO change this! */
#include "/Library/Developer/CommandLineTools/Library/Frameworks/Python3.framework/Versions/3.9/Headers/Python.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static PyObject *SymNMFLib_SymNMF(PyObject *self, PyObject *args) {
    return NULL;
}

static PyObject *SymNMFLib_Sym(PyObject *self, PyObject *args) {
    PyObject *p_points;

    int iter;
    double eps;
    int size, dimension = 0;
    double **points = NULL;
    double **centroids = NULL;
    int i, j;
    int res;

    if (!PyArg_ParseTuple(args, "O", &p_points)) return NULL;

    size = PyObject_Length(p_points);
    if (size < 0) return NULL;

    points = calloc(size, sizeof(double *));
    if (points == NULL) err_top
        for (i = 0; i < size; i++) {
            p_point = PyList_GetItem(p_points, i);
            if (dimension == 0) dimension = PyObject_Length(p_point);

            points[i] = calloc(dimension, sizeof(double));

            for (j = 0; j < dimension; j++) {
                p_coord = PyList_GetItem(p_point, j);
                points[i][j] = PyFloat_AsDouble(p_coord);
            }
        }

    centroids = calloc(k, sizeof(double *));
    if (centroids == NULL) err_top
        for (i = 0; i < k; i++) {
            centroids[i] = calloc(dimension, sizeof(double));
            p_point = PyList_GetItem(p_centroids, i);
            for (j = 0; j < dimension; j++) {
                p_coord = PyList_GetItem(p_point, j);
                centroids[i][j] = PyFloat_AsDouble(p_coord);
            }
        }

    res = kmeans(points, size, &centroids, k, iter, eps, dimension);
    if (res != 0) err_top

                p_centroids = PyList_New(k);
    for (i = 0; i < k; i++) {
        p_point = PyList_New(dimension);
        for (j = 0; j < dimension; j++) {
            p_coord = Py_BuildValue("d", centroids[i][j]);
            PyList_SetItem(p_point, j, p_coord);
        }
        PyList_SetItem(p_centroids, i, p_point);
    }


    free_memory(points, size, centroids, k);

    return p_centroids;
}

static PyObject *SymNMFLib_DDG(PyObject *self, PyObject *args) {
    return NULL;
}

static PyObject *SymNMFLib_Norm(PyObject *self, PyObject *args) {
    return NULL;
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
