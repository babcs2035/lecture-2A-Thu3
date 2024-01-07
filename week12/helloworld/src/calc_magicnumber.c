#include <stdio.h>
#include <string.h>

void solve(char *str)
{
  float f[4] = {0.};
  const size_t len = strlen(str);
  for (size_t i = 0; i < len; ++i)
  {
    char *p = (char *)f + i;
    *p = str[i];
  }
  // const int n = len % sizeof(float);
  // printf("n = %d\n", n);
  // for (int i = 0; i < n; ++i)
  // {
  //   char *p1 = (char *)f + len - n + i;
  //   char *p2 = p1 + (sizeof(float) - n);
  //   *p2 = *p1;
  //   *p1 = 0;
  // }
  // memcpy(f, str, strlen(str));

  for (int i = 0; i < 4; ++i)
  {
    printf("%f\n", f[i]);
  }
  printf("%s", (char *)f);
}

int main()
{
  solve("Hello,World\n");
  printf("\n----------\n");
  solve("hello,World\n");
}
