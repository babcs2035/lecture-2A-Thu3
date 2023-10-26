#include <stdio.h>

void delete(int *a, int N, int pos)
{
  for (int *p = a + pos; p < a + N - 1; ++p)
  {
    *p = *(p + 1);
  }
  *(a + N - 1) = -1;
}

void unique(int *a, int N)
{
  for (int *p = a; p < a + N && *p != -1; ++p)
  {
    int isDeleted = 0;
    for (int *q = p + 1; q < a + N; ++q)
    {
      if (*p == *q)
      {
        delete (a, N, q - a);
        isDeleted = 1;
      }
    }
    if (isDeleted)
    {
      --p;
    }
  }
}

int main()
{
  int N = 8;
  int nums[8] = {7, 44, 9, 44, 9, 3, 5, 5};

  unique(nums, N);

  for (int i = 0; i < N; i++)
  {
    printf("%d ", nums[i]);
  }
  printf("\n");
  return 0;
}
