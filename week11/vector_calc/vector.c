#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

// ここで構造体の定義をかく。このメンバーに直接触れるのはこのファイルの中の関数だけ
struct vector
{
    double v[2];
};

Vector *axpy(double alpha, const Vector *a, const Vector *b)
{
    Vector *v = malloc(sizeof(Vector));

    // このvector.c 内に限り、アロー演算子が有効（構造体の定義を知っているため）
    *v = (Vector){.v = {alpha * a->v[0] + b->v[0], alpha * a->v[1] + b->v[1]}};

    // 以下は上記の置き換えパターンのうち、かなり裏技的な書き方。Vector型は配列のみのメンバなので、doubleのポインタにキャストして
    // バナナをつけると、doubleとして値を取り出せる。
    // memcpy(v,(double []){alpha *  *(double*)a + *(double*)b, alpha * *((double*)a + 1) + *((double*)b + 1)}, sizeof(double)*2);

    return v;
}

void print_vector(const Vector *a)
{
    printf("[%f %f]\n", a->v[0], a->v[1]);
}

Vector *init_vector(const double v[])
{
    Vector *ret = malloc(sizeof(Vector));
    memcpy(ret->v, v, sizeof(double) * 2);
    return ret;
}

void free_vector(Vector **v)
{
    free(*v);
    *v = NULL;
}

double get_vec_element(const Vector *v, size_t i)
{
    // 不正なインデックスアクセスを抑制
    if (i != 0 && i != 1)
    {
        fprintf(stderr, "%s: index should be 0 or 1.", __func__);
        exit(1);
    }
    return v->v[i];
}
