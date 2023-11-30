// #include <stdio.h>
// #include <stdlib.h>

// typedef struct person
// {
//   char name[10];
//   unsigned char age;
// } Person;

// int main(int argc, char **argv)
// {
//   Person p = {.name = "hoge", .age = 28};

//   printf("Input the name: ");
//   scanf("%9s", p.name);

//   printf("age: %d\n", (int)p.age);
//   printf("name: %s\n", p.name);

//   return EXIT_SUCCESS;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  const size_t bufsize = 500;
  char buf[bufsize];

  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp;

  if ((fp = fopen(argv[1], "r")) == NULL)
  {
    fprintf(stderr, "Cannot open file: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  while (fgets(buf, bufsize, fp) != NULL)
  {
    size_t len = strlen(buf) - 1;
    printf("%zd\n", len);
  }

  fclose(fp);

  return EXIT_SUCCESS;
}