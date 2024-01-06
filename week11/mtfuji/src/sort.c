#include <stddef.h>
#include "sort.h"

void sort_points(Points *ps, size_t size)
{
  for (int i = 0; i < size; ++i)
  {
    for (int j = i + 1; j < size; ++j)
    {
      if (get_alt(ps, i) < get_alt(ps, j))
      {
        Point *tmp1 = init_point(get_loc(ps, i), get_alt(ps, i), get_temp(ps, i));
        Point *tmp2 = init_point(get_loc(ps, j), get_alt(ps, j), get_temp(ps, j));
        set_point(ps, j, tmp1);
        set_point(ps, i, tmp2);
      }
    }
  }
}
