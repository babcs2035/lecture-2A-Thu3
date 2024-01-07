#include <stdio.h>
#include <assert.h>

typedef struct
{
  long a, b, c, d;
} Matrix;

Matrix mul(Matrix m1, Matrix m2)
{
  return (Matrix){m1.a * m2.a + m1.b * m2.c, m1.a * m2.b + m1.b * m2.d, m1.c * m2.a + m1.d * m2.c, m1.c * m2.b + m1.d * m2.d};
}

Matrix A(int n)
{
  if (n == 0)
  {
    return (Matrix){1, 0, 0, 1};
  }
  if (n % 2 == 0)
  {
    Matrix m = A(n / 2);
    return mul(m, m);
  }
  else
  {
    Matrix m = A((n - 1) / 2);
    return mul(mul(m, m), (Matrix){1, 1, 1, 0});
  }
}

long fibo(int n)
{
  assert(n <= 90);
  if (n == 0)
  {
    return 0;
  }
  if (n == 1)
  {
    return 1;
  }
  Matrix m = A(n - 1);
  return m.a;
}

int main()
{
  for (int i = 0; i < 90; i++)
  {
    printf("%ld\n", fibo(i));
  }
}
