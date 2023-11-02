#include <stdio.h>
#include <string.h>

int find_largest_(int a[], int n)
{
  int ans = a[0];
  for (int i = 0; i < n; ++i)
  {
    if (ans < a[i])
    {
      ans = a[i];
    }
  }
  return ans;
}

int *find_largest2_(int a[], int n)
{
  int *ans = &a[0];
  for (int i = 0; i < n; ++i)
  {
    if (*ans < a[i])
    {
      ans = &a[i];
    }
  }
  return ans;
}

void swap(char *prefectures[], int i, int j)
{
  char *tmp = prefectures[i];
  prefectures[i] = prefectures[j];
  prefectures[j] = tmp;
}

void swap2(int **arr, int i, int j)
{
  int *tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

int main(int argc, char **argv)
{
  // char **p1;
  // char *p2 = "abc";
  // p1 = &p2;

  // char **p3;
  // char *p4[] = {"abc", "def", "ghi"};
  // p3 = &p4;
  // p3[0][1];

  const char *strs[] = {"toyama", "ishikawa", "fukui"};
  for (int i = 1; i < argc; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      if (strcmp(strs[j], argv[i]) == 0)
      {
        printf("hokuriku!\n");
        return 0;
      }
    }
  }

  return 0;
}
