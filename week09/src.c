// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>

// struct point
// {
//   int x;
//   int y;
// };

// int main()
// {
//   struct point points[1000];

//   srand((unsigned int)time(NULL));
//   for (int i = 0; i < 1000; ++i)
//   {
//     points[i] = (struct point){.x = rand(), .y = rand()};
//   }

//   double gx = 0, gy = 0;
//   for (int i = 0; i < 1000; ++i)
//   {
//     gx += points[i].x;
//     gy += points[i].y;
//   }
//   gx /= 1000;
//   gy /= 1000;

//   printf("(%lf, %lf)\n", gx, gy);
//   return 0;
// }

#include <stdio.h>

int main()
{
  char c;
  int n;
  double lf;
  float f;
  printf("%p, %p, %p, %p\n", &c, &n, &lf, &f);
  return 0;
}
