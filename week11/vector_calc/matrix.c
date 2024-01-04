#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "vector.h"

struct matrix
{
    double v[4];
};

Vector *gemv(double alpha, const Matrix *A, const Vector *x, double beta, const Vector *y)
{
    double xx, xy, yx, yy; // x = [xx, xy], y = [yx, yy];
    xx = get_vec_element(x, 0);
    xy = get_vec_element(x, 1);
    yx = get_vec_element(y, 0);
    yy = get_vec_element(y, 1);

    double axx, axy, ayx, ayy;
    axx = get_mat_element(A, 0, 0);
    axy = get_mat_element(A, 0, 1);
    ayx = get_mat_element(A, 1, 0);
    ayy = get_mat_element(A, 1, 1);

    double a = alpha * (axx * xx + axy * xy) + beta * yx;
    double b = alpha * (ayx * xx + ayy * xy) + beta * yy;

    return init_vector((double[]){a, b});
}

void print_matrix(const Matrix *m)
{
    printf("[%f %f ;%f %f]\n", m->v[0], m->v[1], m->v[2], m->v[3]);
}

Matrix *init_matrix(const double m[])
{
    Matrix *ret = (Matrix *)malloc(sizeof(Matrix));
    memcpy(ret->v, m, sizeof(double) * 4);
    return ret;
}

void free_matrix(Matrix **m)
{
    free(*m);
    *m = NULL;
}

double get_mat_element(const Matrix *m, size_t i, size_t j)
{
    if (i != 0 && i != 1)
    {
        fprintf(stderr, "%s: index out of range %zu. It should be [0 1].\n", __func__, i);
        exit(1);
    }
    if (j != 0 && j != 1)
    {
        fprintf(stderr, "%s: index out of range %zu. It should be [0 1].\n", __func__, j);
        exit(1);
    }
    return m->v[2 * i + j];
}
