#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "optimize.h"

double calc_norm(double v[2])
{
  return sqrt(v[0] * v[0] + v[1] * v[1]);
}

double f_value(Points *ps, size_t size, const double a, const double b)
{
  double res = 0;
  for (size_t i = 0; i < size; ++i)
  {
    const double x = get_alt(ps, i);
    const double y = get_temp(ps, i);
    res += (y - a * x - b) * (y - a * x - b);
  }
  return res;
}

void f_gradient(Points *ps, size_t size, const double a, const double b, double g[])
{
  g[0] = 0;
  g[1] = 0;
  for (size_t i = 0; i < size; ++i)
  {
    const double x = get_alt(ps, i);
    const double y = get_temp(ps, i);
    g[0] += 2 * (y - a * x - b) * (-x);
    g[1] += 2 * (y - a * x - b) * (-1);
  }
}

int optimize(Points *ps, size_t size, const double alpha, double *a, double *b)
{
  double *g = malloc(2 * sizeof(double));

  int cnt = 0;
  while (++cnt < 1000)
  {
    f_gradient(ps, size, *a, *b, g);
    const double norm = calc_norm(g);
    printf("%3d norm = %7.4f, value = %7.4f (a = %7.4f, b = %7.4f)\n", cnt, norm, f_value(ps, size, *a, *b), *a, *b);

    if (f_value(ps, size, *a, *b) < 5)
      break;

    *a -= alpha * g[0];
    *b -= alpha * g[1];
  }

  free(g);
  return cnt;
}
