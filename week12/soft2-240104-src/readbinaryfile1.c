#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s filename\n", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *fp;
  if ((fp = fopen(argv[1], "r")) == NULL)
  {
    perror(argv[1]);
    return EXIT_FAILURE;
  }

  size_t size;
  fscanf(fp, "%ld\n", &size);
  double *arr = (double *)malloc(sizeof(double) * size);
  for (int i = 0; i < size; ++i)
  {
    fscanf(fp, "%lf\n", &arr[i]);
  }

  return EXIT_SUCCESS;
}
