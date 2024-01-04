#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  // strtol のエラー判定用
#include <string.h> // strerror(errno) のため

int comp_int(const void *x0, const void *x1)
{
  const int *p0 = x0; // x0 is interpreted as a pointer to an integer
  const int y0 = *p0; // y0 is the int value that exists at p0
  //  const int y0 = *(int*)x0;  // The above can be written in one line.

  const int *p1 = x1;
  const int y1 = *p1;
  //  const int y1 = *(int*)x1;

  if (y0 > y1)
    return -1;
  if (y0 < y1)
    return 1;
  return 0;
}

int load_int(const char *str)
{
  errno = 0;
  char *e;
  long n = strtol(str, &e, 10);

  // エラーチェック
  if (errno == ERANGE)
  {
    fprintf(stderr, "%s: %s\n", str, strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (*e != '\0')
  {
    fprintf(stderr, "%s: an irregular character '%c' is detected.\n", str, *e);
    exit(EXIT_FAILURE);
  }
  return (int)n;
}
