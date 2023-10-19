#include <stdio.h>

int convert_char_to_int(char c)
{
  return c - '0';
}

int convert_str_to_int(char *str)
{
  int res = 0;
  int i = 0;
  while (str[i] != '\0')
  {
    res = 10 * res + convert_char_to_int(str[i]);
    ++i;
  }
  return res;
}

int main()
{
  char c;
  char sign;
  char num[5];
  int num_cnt = 0;
  int signs[20], nums[20];
  int input_num = 0;
  while ((c = getchar()) != EOF)
  {
    if (c == '\n')
    {
      signs[input_num] = sign;
      nums[input_num] = convert_str_to_int(num);
      ++input_num;
      sign = '\0';
      num[0] = '\0';
      num[1] = '\0';
      num_cnt = 0;
    }
    else if ('a' <= c && c <= 'z')
    {
      sign = c;
    }
    else if ('0' <= c && c <= '9')
    {
      num[num_cnt] = c;
      ++num_cnt;
    }
  }

  printf("  0    5   10   15\n");
  char ans[] = "--+----+----+----+-->";
  for (int i = 0; i < input_num; ++i)
  {
    ans[nums[i] + 2] = signs[i];
  }
  printf("%s\n", ans);
  return 0;
}
