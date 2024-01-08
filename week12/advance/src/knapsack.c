#include <stdio.h>
#include <stdlib.h>
#define MAX_N 200
#define MAX_W 200000
#define INF 1123456789

int N, W;
long long v[MAX_N], w[MAX_N];
long long memo[MAX_N + 1][MAX_W + 1];

long long max(long long a, long long b)
{
  return a > b ? a : b;
}

long long dp(int i, int j)
{
  if (j < 0)
    return -INF;
  if (i < 0)
    return 0;
  if (memo[i][j] >= 0)
    return memo[i][j];
  return memo[i][j] = max(dp(i - 1, j - w[i]) + v[i], dp(i - 1, j));
}

int main(int argc, char *argv[])
{
  FILE *fp;
  if ((fp = fopen(argv[1], "r")) == NULL)
  {
    printf("Cannot open file.\n");
    return EXIT_FAILURE;
  }
  fscanf(fp, "%d %d", &N, &W);
  for (size_t i = 0; i < N; ++i)
  {
    fscanf(fp, "%lld %lld", &v[i], &w[i]);
  }

  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j <= W; ++j)
    {
      memo[i][j] = -1;
    }
  }
  printf("%lld\n", dp(N - 1, W));

  return EXIT_SUCCESS;
}
