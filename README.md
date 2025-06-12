# Software Project - Symmetric Non-negative Matrix Factorization (SymNMF)

## Overview

This project implements a clustering algorithm based on **Symmetric Non-negative Matrix Factorization (SymNMF)** and applies it to several datasets. The implementation includes a C extension wrapped in Python, allowing efficient computation combined with Python usability.

SymNMF is compared with K-means clustering using the silhouette score to evaluate clustering quality.

---

## Algorithm

Given a set of points \(X = \{x_1, x_2, ..., x_N\} \subset \mathbb{R}^d\), the SymNMF algorithm follows these steps:

1. **Similarity Matrix \(A\)**  
   Constructed as:  
   \[
   a_{ij} = \begin{cases}
   \exp(-\|x_i - x_j\|_2^2) & i \neq j \\
   0 & i = j
   \end{cases}
   \]

2. **Degree Matrix \(D\)**  
   Diagonal matrix with vertex degrees:  
   \[
   d_i = \sum_{j=1}^n a_{ij}
   \]

3. **Normalized Similarity Matrix \(W\)**  
   Defined as:  
   \[
   W = D^{-1/2} A D^{-1/2}
   \]

4. **Matrix Factorization**  
   Find non-negative matrix \(H \in \mathbb{R}^{n \times k}\) that minimizes:  
   \[
   \min_{H \geq 0} \|W - H H^T\|_F^2
   \]  
   using an iterative update rule until convergence or max iterations.

5. **Clustering**  
   Assign each point to the cluster corresponding to the column of \(H\) with the highest value.

---

## Project Files

| File             | Description                                         |
|------------------|-----------------------------------------------------|
| `symnmf.py`      | Python interface to the C extension. Reads CLI args and outputs matrices. |
| `symnmf.c`       | C implementation of SymNMF computations.           |
| `symnmfmodule.c` | Python C API wrapper exposing C functions to Python.|
| `symnmf.h`       | Header file declaring C function prototypes.       |
| `analysis.py`    | Script comparing SymNMF clustering with K-means via silhouette score. |
| `setup.py`       | Build script to compile the C extension.            |
| `Makefile`       | Build script for the C program executable.          |
| Other `.c`/`.h` files | Additional modules and headers as needed.        |

---

## Usage

### Build C Extension

```bash
python3 setup.py build_ext --inplace
```

# Step 2: Build C Executable

To compile the C program for Symmetric Non-negative Matrix Factorization (SymNMF), run the following command in your project directory:

```bash
make
```

# Step 3: Run the Python Interface

Use the Python interface to run the Symmetric Non-negative Matrix Factorization (SymNMF) algorithm or compute intermediate matrices.

---

## Command Syntax

```bash
python3 symnmf.py <k> <goal> <input_file.txt>
```

# Step 4: Run the C Program

After building the C executable (`symnmf`), you can run it directly to compute matrices for your dataset.

---

## Command Syntax

```bash
./symnmf <goal> <input_file.txt>
```


# Step 5: Run Analysis Script

This step runs a Python script that compares the clustering quality of Symmetric Non-negative Matrix Factorization (SymNMF) against K-means clustering using the silhouette score metric.

---

## Purpose

The script `analysis.py` applies both SymNMF and K-means clustering to the given dataset and calculates the **silhouette score** for each method.

The silhouette score measures how well clusters are separated and defined:

\[
\text{Silhouette coefficient} = \frac{b - a}{\max(a, b)}
\]

where for each data point:

- \(a\) = average distance between the point and all other points in the **same cluster** (intra-cluster distance).  
- \(b\) = minimum average distance between the point and all points in any **other cluster** (nearest-cluster distance).

A higher silhouette score (closer to 1) indicates better cluster separation.

---

## Command Syntax

```bash
python3 analysis.py <k> <input_file.txt>
```

