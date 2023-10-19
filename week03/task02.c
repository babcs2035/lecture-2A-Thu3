#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_NUM_LEN 10
#define MAX_INPUT_NUM 50
#define MAX_OUTPUT_NUM 1000

int convert_char_to_int(char c)
{
  return c - '0';
}

int convert_str_to_int(char *str)
{
  int res = 0;
  bool is_minus = false;
  for (int i = 0; str[i] != '\0'; ++i)
  {
    if (str[i] == '-')
    {
      is_minus = true;
      continue;
    }
    res = 10 * res + convert_char_to_int(str[i]);
  }
  if (is_minus)
  {
    res *= -1;
  }
  return res;
}

int get_max_of_arr(int *arr, int len)
{
  int res = arr[0];
  for (int i = 0; i < len; ++i)
  {
    if (res < arr[i])
    {
      res = arr[i];
    }
  }
  return res;
}

int get_min_of_arr(int *arr, int len)
{
  int res = arr[0];
  for (int i = 0; i < len; ++i)
  {
    if (res > arr[i])
    {
      res = arr[i];
    }
  }
  return res;
}

int main()
{
  char c;
  char sign = '\0';
  char num[MAX_NUM_LEN] = {0};
  int num_cnt = 0;
  bool is_valid = true;
  int signs[MAX_INPUT_NUM] = {0}, nums[MAX_INPUT_NUM] = {0};
  int input_num = 0;

  system("clear");
  while ((c = getchar()) != EOF)
  {
    if (c == '\n')
    {
      if (is_valid)
      {
        signs[input_num] = sign;
        nums[input_num] = convert_str_to_int(num);
        ++input_num;
      }
      sign = '\0';
      for (int i = 0; i < MAX_NUM_LEN; ++i)
      {
        num[i] = '\0';
      }
      num_cnt = 0;
      is_valid = true;
    }
    else if ('a' <= c && c <= 'z')
    {
      if (sign != '\0')
      {
        system("clear");
        printf("You can't have two or more signs in a row!\n");
        is_valid = false;
      }
      else
      {
        sign = c;
      }
    }
    else if (('0' <= c && c <= '9') || c == '-')
    {
      num[num_cnt] = c;
      num_cnt++;
    }
  }

  int max = (get_max_of_arr(nums, input_num) / 5 + 1) * 5;
  int min = (get_min_of_arr(nums, input_num) / 5 - 1) * 5;
  char ans[2][MAX_OUTPUT_NUM] = {0};
  for (int i = 0; i < max - min + 6; ++i)
  {
    int number = min - 2 + i;
    if (number % 5 == 0)
    {
      int temp = number;
      int cnt = 0;
      if (temp == 0)
      {
        ans[0][i] = '0';
      }
      bool is_minus = false;
      if (temp < 0)
      {
        is_minus = true;
        temp *= -1;
      }
      while (temp != 0)
      {
        ans[0][i - cnt] = (temp % 10) + '0';
        temp /= 10;
        ++cnt;
      }
      if (is_minus)
      {
        ans[0][i - cnt] = '-';
      }
      ans[1][i] = '+';
    }
    else
    {
      ans[0][i] = ' ';
      ans[1][i] = '-';
    }
    if (i == max - min + 5)
    {
      ans[1][i] = '>';
    }
  }
  for (int i = 0; i < input_num; ++i)
  {
    ans[1][nums[i] - min + 2] = signs[i];
  }
  printf("%s\n%s\n", ans[0], ans[1]);
  return 0;
}
