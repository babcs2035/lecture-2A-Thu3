#include "matrix.h"
#include "vector.h"

int main()
{
    // 複合リテラルでdouble型配列を用意して引数とする。
    // 配列が引数の場合は、関数内部ではポインタ扱いで処理される。
    Vector *x = init_vector((double[]){1.0, 2.0});
    Vector *y = init_vector((double[]){3.0, 4.0});
    Matrix *A = init_matrix((double[]){1.0, -1.0, -1.0, 1.0});

    // c = 2.0 x + y
    Vector *c = axpy(2.0, x, y);

    // d = 2.0 Ax + y
    Vector *d = gemv(2.0, A, x, 1.0, y);

    print_vector(c);
    print_vector(d);

    // 以下のfree関数群の結果、ポインタにはNULLが代入されるので、init関数でmallocされたときのアドレス情報は残らない
    free_vector(&x);
    free_vector(&y);

    free_vector(&c);
    free_vector(&d);
    free_matrix(&A);

    return 0;
}