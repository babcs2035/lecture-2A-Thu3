#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "sort.h"
#include "optimize.h"

int main()
{
  FILE *fp;
  if ((fp = fopen("./data/data.csv", "r")) == NULL)
  {
    fprintf(stderr, "Error: cannot open file.\n");
    return EXIT_FAILURE;
  }

  const size_t buf_size = 256;
  char buf[buf_size];
  int cnt = 0;
  while (fgets(buf, buf_size, fp) != NULL)
  {
    ++cnt;
  }
  rewind(fp);

  Points *ps = init_points(cnt);
  cnt = 0;
  while (fgets(buf, buf_size, fp) != NULL)
  {
    char *loc;
    double alt, temp;
    sscanf(buf, "%[^,],%lf,%lf", loc, &alt, &temp);
    Point *p = init_point(loc, alt, temp);
    set_point(ps, cnt, p);
    ++cnt;
  }

  double a = -5, b = 30;
  optimize(ps, cnt, 0.01, &a, &b);
  printf("The temp at Mt.Fuji: %lf\n", a * 3.776 + b);

  sort_points(ps, cnt);
  for (int i = 0; i < cnt; ++i)
  {
    printf("%s, %lf, %lf\n", get_loc(ps, i), get_alt(ps, i), get_temp(ps, i));
  }
}
