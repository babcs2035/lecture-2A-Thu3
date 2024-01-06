#pragma once
#include "canvas.h"

// Command 構造体と History構造体
typedef struct command Command;

typedef struct
{
  Command *begin;
  size_t bufsize;
} History;

History *init_history();

// enum for interpret_command results
typedef enum res
{
  EXIT,
  LINE,
  UNDO,
  SAVE,
  UNKNOWN,
  ERRNONINT,
  ERRLACKARGS,
  NOCOMMAND
} Result;

// Result 型に応じて出力するメッセージを返す
char *strresult(Result res);

Result interpret_command(const char *command, History *his, Canvas *c);
void save_history(const char *filename, History *his);

// [*] list.c のpush_backと同じ
Command *push_command(History *his, const char *str);
