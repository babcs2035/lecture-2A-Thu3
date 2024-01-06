#pragma once
#include "type.h"

double calc_norm(double v[2]);

double f_value(Points *ps, size_t size, const double a, const double b);

void f_gradient(Points *ps, size_t size, const double a, const double b, double g[]);

int optimize(Points *ps, size_t size, const double alpha, double *a, double *b);
