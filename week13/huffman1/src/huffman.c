#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "huffman.h"
#include "LZSS.h"

#define NSYMBOLS 256

// 構造体定義
struct huffman_node
{
    int symbol;
    int count;
    huffman_Node *left;
    huffman_Node *right;
};

static int symbol_count[NSYMBOLS];

static void count_symbols(LZSS_Node *root)
{
    LZSS_Node *node = root;
    while (node != NULL)
    {
        ++symbol_count[node->flag];
        ++symbol_count[node->dat1];
        if (node->flag == 1)
            ++symbol_count[node->dat2];
        node = node->next;
    }
}
static void reset_count(void)
{
    for (int i = 0; i < NSYMBOLS; i++)
        symbol_count[i] = 0;
}

static huffman_Node *create_huffman_node(int symbol, int count, huffman_Node *left, huffman_Node *right)
{
    huffman_Node *ret = (huffman_Node *)malloc(sizeof(huffman_Node));
    *ret = (huffman_Node){.symbol = symbol, .count = count, .left = left, .right = right};
    return ret;
}

static huffman_Node *pop_min(int *n, huffman_Node *nodep[])
{
    int argmin = 0;
    for (int i = 0; i < *n; i++)
    {
        if (nodep[i]->count < nodep[argmin]->count)
        {
            argmin = i;
        }
    }

    huffman_Node *node_min = nodep[argmin];

    for (int i = argmin; i < (*n) - 1; i++)
    {
        nodep[i] = nodep[i + 1];
    }
    (*n)--;

    return node_min;
}

static huffman_Node *build_tree(void)
{
    int n = 0;
    huffman_Node *nodep[NSYMBOLS];

    for (int i = 0; i < NSYMBOLS; i++)
    {
        if (symbol_count[i] == 0)
            continue;
        nodep[n++] = create_huffman_node(i, symbol_count[i], NULL, NULL);
    }

    const int dummy = -1;
    while (n >= 2)
    {
        huffman_Node *node1 = pop_min(&n, nodep);
        huffman_Node *node2 = pop_min(&n, nodep);
        nodep[n++] = create_huffman_node(dummy, node1->count + node2->count, node1, node2);
    }

    return (n == 0) ? NULL : nodep[0];
}

static char codes[NSYMBOLS][NSYMBOLS + 1];
void traverse_tree(const int depth, const huffman_Node *np, char *str)
{
    if (np == NULL || np->right == NULL)
    {
        strcpy(codes[np->symbol], str);
        codes[np->symbol][depth] = '\0';
        return;
    }

    char *str_ = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(str_, str);
    str_[depth] = '0';
    traverse_tree(depth + 1, np->left, str_);
    str_[depth] = '1';
    traverse_tree(depth + 1, np->right, str_);
    free(str_);
}

huffman_Node *encode(LZSS_Node *root)
{
    reset_count();
    count_symbols(root);
    huffman_Node *res = build_tree();

    if (res == NULL)
    {
        fprintf(stderr, "A tree has not been constructed.\n");
    }

    return res;
}

int convert_binary_to_int(const char *code)
{
    int res = 0;
    for (int i = 0; i < strlen(code); i++)
    {
        res = res * 2 + (code[i] - '0');
    }
    return res;
}

char *convert_int_to_binary(int num)
{
    char *res = (char *)malloc(sizeof(char) * (CHAR_BIT * sizeof(int) + 1));
    for (int i = 0; i < CHAR_BIT * sizeof(int); i++)
    {
        res[CHAR_BIT * sizeof(int) - i - 1] = (num & 1) + '0';
        num >>= 1;
    }
    res[CHAR_BIT * sizeof(int)] = '\0';
    return res;
}

char *get_code(int symbol)
{
    return strlen(codes[symbol]) ? codes[symbol] : "*";
}

int get_symbol(const char *code)
{
    for (int i = 0; i < NSYMBOLS; i++)
    {
        if (strcmp(codes[i], code) == 0)
        {
            return i;
        }
    }
}

void draw_tree(const huffman_Node *node, const int depth, const int is_right, int *is_parent_right)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < depth - 1; i++)
    {
        if (i > 0)
        {
            printf("   ");
        }
        printf("%s", is_parent_right[i + 1] ? " " : "│");
    }
    if (depth > 0)
    {
        if (depth > 1)
        {
            printf("   ");
        }
        const char *code = get_code(node->symbol);
        if (code[0] == '*')
        {
            printf("%s *\n", is_right ? "└──" : (depth == 1 ? "┌──" : "├──"));
        }
        else
        {
            printf("%s [%d] %s\n", is_right ? "└──" : (depth == 1 ? "┌──" : "├──"), node->symbol, code);
        }
    }
    is_parent_right[depth] = is_right;
    draw_tree(node->left, depth + 1, 0, is_parent_right);
    draw_tree(node->right, depth + 1, 1, is_parent_right);
}

void write_dat(FILE *fp, const char *code)
{
    fwrite(code, sizeof(char), strlen(code), fp);
}

void write_code(LZSS_Node *root, const char *dst_filename)
{
    FILE *fp = fopen(dst_filename, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open compressed.dat\n");
        exit(1);
    }

    LZSS_Node *node = root;
    printf("encoded:\t");
    while (node != NULL)
    {
        write_dat(fp, get_code(node->flag));
        write_dat(fp, get_code(node->dat1));
        if (node->flag == 1)
        {
            write_dat(fp, get_code(node->dat2));
            printf("(%s, %s, %s) ", (get_code(node->flag)), (get_code(node->dat1)), (get_code(node->dat2)));
        }
        else
        {
            printf("(%s, %s) ", (get_code(node->flag)), (get_code(node->dat1)));
        }
        node = node->next;
    }
    printf("\n");

    fclose(fp);
}

LZSS_Node *read_code(const char *src_filename, const huffman_Node *node)
{
    FILE *fp = fopen(src_filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open compressed.dat\n");
        exit(1);
    }

    printf("huffman:\t");
    char c;
    LZSS_Node *first_node = NULL;
    LZSS_Node *last_node = NULL;
    while (fread(&c, sizeof(char), 1, fp) != 0)
    {
        printf("%d", get_symbol(c));
        if (get_symbol(c) == 1)
        {
            char d1, d2;
            fread(&d1, sizeof(char), 1, fp);
            fread(&d2, sizeof(char), 1, fp);
            printf("(%d, %d, %d) ", get_symbol(c), get_symbol(d1), get_symbol(d2));
            last_node = create_LZSS_node(get_symbol(c), get_symbol(d1), get_symbol(d2), last_node);
        }
        else
        {
            char d1;
            fread(&d1, sizeof(char), 1, fp);
            printf("(%d, %d) ", get_symbol(c), get_symbol(d1));
            last_node = create_LZSS_node(get_symbol(c), get_symbol(d1), -1, last_node);
        }
        if (first_node == NULL)
        {
            first_node = last_node;
        }
    }
    printf("\n");

    fclose(fp);

    return first_node;
}
