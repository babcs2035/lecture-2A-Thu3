#pragma once
#include "canvas.h"

// display functions
void rewind_screen(unsigned int line);
void clear_command(void);
void clear_screen(void);

int max(const int a, const int b);
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1);
