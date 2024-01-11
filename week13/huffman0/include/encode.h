#pragma once

typedef struct node Node;

// ファイルをエンコードし木のrootへのポインタを返す
Node *encode(const char *filename);
// Treeを走査して表示する
void traverse_tree(const int depth, const Node *root, char *str);

char *get_code(int symbol);

void draw_tree(const Node *node, const int depth, const int is_right, int *is_parent_right);
