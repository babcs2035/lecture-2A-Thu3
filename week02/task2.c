#include <stdio.h>

int main()
{
  int N;
  scanf("%d", &N);

  int coins[] = {100, 50, 10, 5, 1};
  int coins_cnt = 5;
  int coins_length[] = {3, 2, 2, 1, 1};
  int ans[] = {0, 0, 0, 0, 0};
  int max_ans = 0;
  for (int i = 0; i < coins_cnt; ++i)
  {
    ans[i] = N / coins[i];
    if (max_ans < ans[i])
    {
      max_ans = ans[i];
    }
    N %= coins[i];
  }

  for (int i = 0; i < coins_cnt; ++i)
  {
    for (int j = 0; j < max_ans - ans[i]; ++j)
    {
      printf(" ");
    }
    for (int j = 0; j < ans[i]; ++j)
    {
      printf("*");
    }
    printf(" : ");
    for (int j = 0; j < 3 - coins_length[i]; ++j)
    {
      printf(" ");
    }
    printf("%d yen\n", coins[i]);
  }

  return 0;
}
