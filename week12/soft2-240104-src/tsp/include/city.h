#pragma once

typedef struct city City;
typedef struct answer
{
  int distance;
  int *route;
} Answer;

double distance(const City *a, const City *b);
City *load_cities(const char *filename, int *n);

City *get_city(const City *a, size_t i);

int get_x(const City *c);
int get_y(const City *c);
