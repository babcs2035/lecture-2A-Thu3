#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_N 9
#define INF 123456

int N;
int x[MAX_N], y[MAX_N];
double memo[MAX_N][1 << MAX_N];

double min(double a, double b)
{
  return a < b ? a : b;
}

double get_dist(int i, int j)
{
  return sqrt((x[i] - x[j]) * (x[i] - x[j]) +
              (y[i] - y[j]) * (y[i] - y[j]));
}

double dp(int i, int S)
{
  if (S == (1 << N) - 1)
    return get_dist(i, 0);
  if (memo[i][S] != INF)
    return memo[i][S];
  for (int j = 0; j < N; ++j)
  {
    if (!(S & (1 << j)))
    {
      memo[i][S] = min(memo[i][S], dp(j, S | (1 << j)) + get_dist(i, j));
    }
  }
  return memo[i][S];
}

int main(int argc, char *argv[])
{
  FILE *fp;
  if ((fp = fopen(argv[1], "r")) == NULL)
  {
    printf("Cannot open file.\n");
    return EXIT_FAILURE;
  }
  fscanf(fp, "%d", &N);
  for (size_t i = 0; i < N; ++i)
  {
    fscanf(fp, "%d %d", &x[i], &y[i]);
  }

  for (size_t i = 0; i < N; ++i)
  {
    for (int j = 0; j < (1 << N); ++j)
    {
      memo[i][j] = INF;
    }
  }
  printf("%lf\n", dp(0, 1));

  return EXIT_SUCCESS;
}
