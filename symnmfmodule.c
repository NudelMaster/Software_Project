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
    return NULL;
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
