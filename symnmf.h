#ifndef HW_FINAL_SYMNMF_H
#define HW_FINAL_SYMNMF_H

double **sym(double **x, int n, int dim);

double **ddg(double **x, int n, int dim);

double **norm(double **x, int n, int dim);

double **symnmf(double **h, double **w, int n, int k);

#endif //HW_FINAL_SYMNMF_H
