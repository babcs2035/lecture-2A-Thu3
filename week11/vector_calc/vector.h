#pragma once
#include <stddef.h> // size_t
// 宣言のみ行う
typedef struct vector Vector;

// ax + y の形（BLASレベル1と同じ名前のつけかた）
// ポインタでのやり取りに変更
Vector *axpy(double alpha, const Vector *a, const Vector *b);
void print_vector(const Vector *a);

// Vector型を動的に確保して初期化する関数
Vector *init_vector(const double v[]);
// Vector 用の free / Vector型ポインタへのポインタとすることで、nullリセット可能にする
void free_vector(Vector **v);
// 要素の取り出し
double get_vec_element(const Vector *v, size_t i);
