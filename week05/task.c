#include <stdio.h>

int get_str_length(char *str)
{
  int cnt = 0;
  while (str[cnt] != '\0')
  {
    ++cnt;
  }
  return cnt;
}

int convert_num_from_str(char *str)
{
  int cnt = get_str_length(str);
  int ans = 0, pow = 1;
  for (int i = cnt - 1; i >= 0; --i)
  {
    ans += pow * (str[i] - '0');
    pow *= 10;
  }
  return ans;
}

int is_strs_same(char *str1, char *str2)
{
  if (get_str_length(str1) != get_str_length(str2))
  {
    return 0;
  }
  int length = get_str_length(str1);
  for (int i = 0; i < length; ++i)
  {
    if (str1[i] != str2[i])
    {
      return 0;
    }
  }
  return 1;
}

void copy_table(int size, char src[size][size], char dst[size][size])
{
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      dst[i][j] = src[i][j];
    }
  }
}

void swap_nums(int *n, int *m)
{
  int t = *n;
  *n = *m;
  *m = t;
}

int main(int argc, char *argv[])
{
  const int length = get_str_length(argv[1]);
  int width = convert_num_from_str(argv[2]);
  int height = length / width;
  const int size = (width >= height ? width : height);
  char table[size][size];
  for (int i = 0; i < length; ++i)
  {
    table[i / width][i % width] = argv[1][i];
  }

  for (int i = 3; argv[i] != NULL; ++i)
  {
    char next_table[size][size];

    if (is_strs_same(argv[i], "--rotate"))
    {
      for (int i = 0; i < width; ++i)
      {
        for (int j = 0; j < height; ++j)
        {
          next_table[i][j] = table[height - 1 - j][i];
        }
      }
      swap_nums(&width, &height);
    }
    if (is_strs_same(argv[i], "--trim_right"))
    {
      for (int i = 0; i < height; ++i)
      {
        for (int j = 0; j < width - 1; ++j)
        {
          next_table[i][j] = table[i][j];
        }
      }
      --width;
    }
    if (is_strs_same(argv[i], "--trim_left"))
    {
      for (int i = 0; i < height; ++i)
      {
        for (int j = 1; j < width; ++j)
        {
          next_table[i][j - 1] = table[i][j];
        }
      }
      --width;
    }

    copy_table(size, next_table, table);
  }

  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      printf("%c", table[i][j]);
    }
    printf("\n");
  }

  return 0;
}
