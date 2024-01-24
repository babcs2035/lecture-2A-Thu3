#pragma once
#define MAX_TEXT_LENGTH 140000
#define SLIDING_WINDOW_SIZE 258

typedef struct LZSS_node LZSS_Node;
struct LZSS_node
{
  int flag, dat1, dat2;
  LZSS_Node *next;
};

LZSS_Node *create_LZSS_node(int flag, int dat1, int dat2, LZSS_Node *last_node);

LZSS_Node *compress(const char *filename);

char *extract(LZSS_Node *head);
