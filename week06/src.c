#include <stdio.h>

typedef struct bit_array
{
  unsigned long W[2];
} BitArray;

void print_bit_ulong1(BitArray *B)
{
  const unsigned long b1 = B->W[0];
  for (int i = 0; i < 64; ++i)
  {
    printf("%d", (b1 & ((unsigned long)1 << (63 - i)) ? 1 : 0));
    if (i % 14 == 7 && i < 63)
    {
      printf("|");
    }
  }
  printf("\n");

  const unsigned long b2 = B->W[1];
  for (int i = 0; i < 64; ++i)
  {
    printf("%d", (b2 & ((unsigned long)1 << (63 - i)) ? 1 : 0));
    if (i % 14 == 1)
    {
      printf("|");
    }
  }
  printf("\n");
}

void set_val(BitArray *B, int n, int val)
{
  if (n <= 3)
  {
    for (int i = 0; i < 14; ++i)
    {
      if (val & (1 << i))
      {
        B->W[0] |= ((unsigned long)1 << (14 * n + i));
      }
    }
  }
  else if (n == 4)
  {
    for (int i = 0; i < 8; ++i)
    {
      if (val & (1 << i))
      {
        B->W[0] |= ((unsigned long)1 << (14 * 4 + i));
      }
    }
    for (int i = 0; i < 6; ++i)
    {
      if (val & (1 << (i + 8)))
      {
        B->W[1] |= ((unsigned long)1 << i);
      }
    }
  }
  else
  {
    for (int i = 0; i < 14; ++i)
    {
      if (val & (1 << i))
      {
        B->W[1] |= ((unsigned long)1 << (14 * (n - 5) + 6 + i));
      }
    }
  }
}

int get_val(const BitArray B, int n)
{
  int res = 0;
  if (n <= 3)
  {
    for (int j = 0; j < 14; ++j)
    {
      if (B.W[0] & ((unsigned long)1 << (14 * n + j)))
      {
        res += ((unsigned long)1 << j);
      }
    }
  }
  else if (n == 4)
  {
    for (int j = 0; j < 8; ++j)
    {
      if (B.W[0] & ((unsigned long)1 << (14 * n + j)))
      {
        res += ((unsigned long)1 << j);
      }
    }
    for (int j = 0; j < 6; ++j)
    {
      if (B.W[1] & ((unsigned long)1 << j))
      {
        res += ((unsigned long)1 << (j + 8));
      }
    }
  }
  else
  {
    for (int j = 0; j < 14; ++j)
    {
      if (B.W[1] & ((unsigned long)1 << (14 * (n - 5) + 6 + j)))
      {
        res += ((unsigned long)1 << j);
      }
    }
  }
  return res;
}

int convert_str_to_num(char *s)
{
  int res = 0;
  for (int i = 0; s[i] != '\0'; ++i)
  {
    res = res * 10 + (s[i] - '0');
  }
  return res;
}

int main(int argc, char *argv[])
{
  BitArray B = {{0, 0}};
  for (int i = 1; argv[i] != NULL; i += 2)
  {
    set_val(&B, convert_str_to_num(argv[i]), convert_str_to_num(argv[i + 1]));
  }
  print_bit_ulong1(&B);
  for (int i = 0; i < 9; ++i)
  {
    printf("%d: %d\n", i, get_val(B, i));
  }
  return 0;
}
