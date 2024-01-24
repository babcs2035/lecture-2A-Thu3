#pragma once
#include "LZSS.h"

typedef struct huffman_node huffman_Node;

huffman_Node *encode(LZSS_Node *root);

void traverse_tree(const int depth, const huffman_Node *root, char *str);

char *get_code(int symbol);

void draw_tree(const huffman_Node *node, const int depth, const int is_right, int *is_parent_right);

void write_code(LZSS_Node *root, const char *dst_filename);

LZSS_Node *read_code(const char *src_filename, const huffman_Node *node);
