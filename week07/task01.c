#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 5

typedef struct point
{
  int x; // x座標
  int y; // y座標
  int v; // 進化度
} Point;

typedef struct board
{
  char field[BOARD_SIZE][BOARD_SIZE + 1];
} Board;

void print_board(Board b, Point p, int flag)
{
  if (flag == 1)
  {
    b.field[p.y][p.x] = p.v + '0';
  }

  for (int i = 0; i < BOARD_SIZE; ++i)
  {
    printf("%s\r\n", b.field[i]);
  }
}

int merge(Board *b)
{
  int flag = 0;
  for (int y = BOARD_SIZE - 1; y >= 0; --y)
  {
    for (int x = 0; x < BOARD_SIZE; ++x)
    {
      if (b->field[y][x] != '-')
      {
        char next_num = b->field[y][x] + 1;
        if (y >= 1 && b->field[y][x] == b->field[y - 1][x])
        {
          b->field[y - 1][x] = '-';
          b->field[y][x] = next_num;
          flag = 1;
        }
        else if (x < BOARD_SIZE - 1 && b->field[y][x] == b->field[y][x + 1])
        {
          b->field[y][x + 1] = '-';
          b->field[y][x] = next_num;
          flag = 1;
        }
      }
    }
  }
  return flag;
}

int main(int argc, char *argv[])
{

  Board b = {.field = {
                 "-----",
                 "-----",
                 "-----",
                 "-----",
                 "-----"}};

  Point p = {.x = 0, .y = 0, .v = 1}; // 現在操作しているオブジェクト
  int c = '_';                        // Dummy

  system("/bin/stty raw onlcr"); // enterを押さなくてもキー入力を受け付けるようになる

  int is_moving = 1;

  while (1)
  {
    system("clear");
    printf("Press '.' to close\r\n");
    printf("You pressed '%c'\r\n", c);

    if (is_moving == 1)
    {
      if (c == 'd')
      {
        ++p.x;
      }
      if (c == 'a')
      {
        --p.x;
      }

      if (p.x < 0)
      {
        p.x = 0;
      }
      if (p.x >= BOARD_SIZE)
      {
        p.x = BOARD_SIZE - 1;
      }

      if (c == 's')
      {
        for (int y = BOARD_SIZE - 1; y >= 0; --y)
        {
          if (b.field[y][p.x] == '-')
          {
            p.y = y;
            break;
          }
        }
        b.field[p.y][p.x] = p.v + '0';
        is_moving = 0;
      }
    }
    else
    {
      if (merge(&b) == 0)
      {
        is_moving = 1;
        p.x = 0;
        p.y = 0;
      }
    }

    print_board(b, p, is_moving);

    if ((c = getchar()) == '.')
    { // '.' を押すと抜ける
      break;
    }
  }

  system("/bin/stty cooked"); // 後始末

  return 0;
}