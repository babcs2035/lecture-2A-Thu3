#pragma once

#include <stddef.h> // size_t
#include "vector.h"
typedef struct matrix Matrix;

// プロトタイプ宣言ではVector型があることをを知っていればよい
// alpha * Ax + beta * y
Vector *gemv(double alpha, const Matrix *A, const Vector *x, double beta, const Vector *y);
void print_matrix(const Matrix *m);

// Matrix型を動的確保し、初期化する関数
Matrix *init_matrix(const double m[]);
// Matrix用の free / 引数をMatrix型ポインタへのポインタとすることで、nullリセット可能にする
void free_matrix(Matrix **m);
double get_mat_element(const Matrix *m, size_t i, size_t j);
