#include <stdio.h>

int main()
{
  const int N = 5;
  const double values[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
  const double weights[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
  FILE *fp;
  fp = fopen("./data/input.dat", "wb");
  fwrite(&N, sizeof(int), 1, fp);
  fwrite(values, sizeof(double), N, fp);
  fwrite(weights, sizeof(double), N, fp);
  fclose(fp);
}
