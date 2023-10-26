#include <stdio.h>

void my_strcpy(char *dst, const char *src)
{
  while ((*dst = *src) != '\0')
  {
    ++dst;
    ++src;
  }
}

int main()
{
  char str1[] = "aaaaaaaaaa";
  char str2[] = "hoge";
  printf("str1: %s\n", str1); // str1: aaaaaaaaaa
  my_strcpy(str1, str2);
  printf("str1: %s\n", str1); // str1: hoge
  // ここでは、str1は以下のようになっています：
  // {'h', 'o', 'g', 'e', '\0', 'a', 'a', 'a', 'a', 'a', '\0'}
  return 0;
}
