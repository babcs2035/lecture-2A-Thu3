#pragma once
#define MAX_TEXT_LENGTH 140000
#define SLIDING_WINDOW_SIZE 16

typedef struct LZ77_node LZ77_Node;
struct LZ77_node
{
  int flag, dat1, dat2;
  LZ77_Node *next;
};

LZ77_Node *compress(const char *filename);

char *extract(LZ77_Node *head);
