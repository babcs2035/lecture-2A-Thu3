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
  if ((fp = fopen(argv[1], "rb")) == NULL)
  {
    perror(argv[1]);
    return EXIT_FAILURE;
  }

  size_t size;
  fread(&size, sizeof(size_t), 1, fp);
  double *arr = (double *)malloc(sizeof(double) * size);
  fread(arr, sizeof(double), size, fp);

  return EXIT_SUCCESS;
}
