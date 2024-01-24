#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LZSS.h"
#include "utilities.h"

LZ77_Node *create_node(int flag, int dat1, int dat2, LZ77_Node *last_node)
{
  LZ77_Node *node = (LZ77_Node *)malloc(sizeof(LZ77_Node));
  node->flag = flag;
  node->dat1 = dat1;
  node->dat2 = dat2;
  node->next = NULL;
  if (last_node != NULL)
  {
    last_node->next = node;
  }
  return node;
}

LZ77_Node *compress(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "error: cannot open %s\n", filename);
    exit(1);
  }
  char *str = (char *)malloc(sizeof(char) * MAX_TEXT_LENGTH);
  fread(str, sizeof(char), MAX_TEXT_LENGTH, fp);
  fclose(fp);

  LZ77_Node *first_node = NULL;
  LZ77_Node *last_node = NULL;

  // todo: repeat
  int i = 0;
  while (str[i] != '\0')
  {
    int max_length = -1, offset = -1;
    for (int j = get_max(0, i - SLIDING_WINDOW_SIZE); j < i; ++j)
    {
      for (int k = j; k < i; ++k)
      {
        if (str[k] != str[i + k - j])
        {
          const int length = k - j;
          if (length > max_length)
          {
            max_length = length;
            offset = i - j;
          }
          break;
        }
      }
    }
    if (first_node != NULL)
    {
      printf(" -> ");
    }
    if (max_length <= 0)
    {
      last_node = create_node(0, str[i], -1, last_node);
      printf("(0, %d)", str[i]);
      ++i;
    }
    else
    {
      last_node = create_node(1, offset, max_length, last_node);
      printf("(1, %d, %d)", offset, max_length);
      i += max_length;
    }
    if (first_node == NULL)
    {
      first_node = last_node;
    }
  }
  printf("\n");
  return first_node;
}

char *extract(LZ77_Node *head)
{
  char *str = (char *)malloc(sizeof(char) * MAX_TEXT_LENGTH);
  int i = 0;
  while (head != NULL)
  {
    if (head->flag == 0)
    {
      str[i++] = head->dat1;
    }
    else
    {
      for (int j = 0; j < head->dat2; ++j)
      {
        str[i + j] = str[i - head->dat1 + j];
      }
      i += head->dat2;
    }
    head = head->next;
  }
  str[i] = '\0';
  return str;
}
