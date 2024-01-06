#pragma once

typedef struct point Point;
typedef struct points Points;

Point *init_point(char *loc, double alt, double temp);
Points *init_points(size_t size);

void set_point(Points *ps, size_t i, Point *p);

char *get_loc(Points *ps, size_t i);
double get_alt(Points *ps, size_t i);
double get_temp(Points *ps, size_t i);
