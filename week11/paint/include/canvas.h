#pragma once

// Structure for canvas
typedef struct canvas Canvas;

int get_canvas_width(Canvas *c);
int get_canvas_height(Canvas *c);
char get_canvas_pen(Canvas *c);
void set_canvas_cell(Canvas *c, int x, int y, char ch);

// functions for Canvas type
Canvas *init_canvas(int width, int height, char pen);
void reset_canvas(Canvas *c);
void print_canvas(Canvas *c);
void free_canvas(Canvas *c);
