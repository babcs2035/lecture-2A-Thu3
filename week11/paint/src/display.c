#include <stdio.h>
#include <stdlib.h>
#include "display.h"

void rewind_screen(unsigned int line)
{
  printf("\e[%dA", line);
}

void clear_command(void)
{
  printf("\e[2K");
}

void clear_screen(void)
{
  printf("\e[2J");
}

int max(const int a, const int b)
{
  return (a > b) ? a : b;
}
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1)
{
  const int width = get_canvas_width(c);
  const int height = get_canvas_height(c);
  char pen = get_canvas_pen(c);

  const int n = max(abs(x1 - x0), abs(y1 - y0));
  if ((x0 >= 0) && (x0 < width) && (y0 >= 0) && (y0 < height))
    set_canvas_cell(c, x0, y0, pen);
  for (int i = 1; i <= n; i++)
  {
    const int x = x0 + i * (x1 - x0) / n;
    const int y = y0 + i * (y1 - y0) / n;
    if ((x >= 0) && (x < width) && (y >= 0) && (y < height))
      set_canvas_cell(c, x, y, pen);
  }
}
