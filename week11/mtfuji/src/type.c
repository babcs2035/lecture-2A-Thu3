#include <stdlib.h>
#include <string.h>
#include "type.h"

struct point
{
  char *loc;
  double alt;
  double temp;
};
struct points
{
  Point *points;
  size_t size;
};

Point *init_point(char *loc, double alt, double temp)
{
  Point *p = (Point *)malloc(sizeof(Point));
  p->loc = (char *)malloc(sizeof(char) * (strlen(loc) + 1));
  strcpy(p->loc, loc);
  p->alt = alt;
  p->temp = temp;
  return p;
}
Points *init_points(size_t size)
{
  Points *ps = (Points *)malloc(sizeof(Points));
  ps->points = (Point *)malloc(sizeof(Point) * size);
  ps->size = size;
  return ps;
}

void set_point(Points *ps, size_t i, Point *p)
{
  ps->points[i] = *p;
}

char *get_loc(Points *ps, size_t i)
{
  return ps->points[i].loc;
}

double get_alt(Points *ps, size_t i)
{
  return ps->points[i].alt;
}

double get_temp(Points *ps, size_t i)
{
  return ps->points[i].temp;
}
