#include <stdio.h>
#include <stdlib.h>

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

void divide_unique(int *a, int N, int *p1, int *p2)
{
  int len1 = p2 - p1;
  int len2 = N - len1;
  int *arr1 = (int *)malloc(sizeof(int) * len1);
  int *arr2 = (int *)malloc(sizeof(int) * len2);
  for (int *p = p1; p < p2; ++p)
  {
    arr1[p - p1] = *p;
  }
  for (int *p = a; p < p1; ++p)
  {
    arr2[p - a] = *p;
  }
  for (int *p = p2; p < a + N; ++p)
  {
    arr2[p - p2 + (p1 - a)] = *p;
  }
  unique(arr1, len1);
  unique(arr2, len2);
  for (int *p = arr1; p < arr1 + len1; ++p)
  {
    a[(p1 - a) + p - arr1] = *p;
  }
  for (int *p = arr2; p < arr2 + (p1 - a); ++p)
  {
    a[p - arr2] = *p;
  }
  for (int *p = arr2 + (p1 - a); p < arr2 + len2; ++p)
  {
    a[len1 + (p - arr2)] = *p;
  }
}

int main()
{
  int pos1 = 2;
  int pos2 = 5;
  int N = 9;
  int nums[] = {12, 12, 42, 42, 31, 8, 9, 7, 8};

  divide_unique(nums, N, nums + pos1, nums + pos2);

  for (int i = 0; i < N; i++)
  {
    printf("%d ", nums[i]);
  }
  printf("\n");
  return 0;
}
