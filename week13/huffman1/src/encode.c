#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "encode.h"

#define NSYMBOLS 256

// 構造体定義
struct node
{
    int symbol;
    int count;
    Node *left;
    Node *right;
};

static int symbol_count[NSYMBOLS];

// 以下このソースで有効なstatic関数のプロトタイプ宣言

// ファイルを読み込み、static配列の値を更新する関数
static void count_symbols(const char *filename);

// symbol_count をリセットする関数
static void reset_count(void);

// 与えられた引数でNode構造体を作成し、そのアドレスを返す関数
static Node *create_node(int symbol, int count, Node *left, Node *right);

// Node構造体へのポインタが並んだ配列から、最小カウントを持つ構造体をポップしてくる関数
// n は 配列の実効的な長さを格納する変数を指している（popするたびに更新される）
static Node *pop_min(int *n, Node *nodep[]);

// ハフマン木を構成する関数
static Node *build_tree(void);

// 以下 static関数の実装
static void count_symbols(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open %s\n", filename);
        exit(1);
    }

    // 1Byteずつ読み込み、カウントする
    /*
      write a code for counting
    */
    char c = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        ++symbol_count[(int)c];
    }

    fclose(fp);
}
static void reset_count(void)
{
    for (int i = 0; i < NSYMBOLS; i++)
        symbol_count[i] = 0;
}

static Node *create_node(int symbol, int count, Node *left, Node *right)
{
    Node *ret = (Node *)malloc(sizeof(Node));
    *ret = (Node){.symbol = symbol, .count = count, .left = left, .right = right};
    return ret;
}

static Node *pop_min(int *n, Node *nodep[])
{
    // Find the node with the smallest count
    // カウントが最小のノードを見つけてくる
    int argmin = 0;
    for (int i = 0; i < *n; i++)
    {
        if (nodep[i]->count < nodep[argmin]->count)
        {
            argmin = i;
        }
    }

    Node *node_min = nodep[argmin];

    // Remove the node pointer from nodep[]
    // 見つかったノード以降の配列を前につめていく
    for (int i = argmin; i < (*n) - 1; i++)
    {
        nodep[i] = nodep[i + 1];
    }
    // 合計ノード数を一つ減らす
    (*n)--;

    return node_min;
}

static Node *build_tree(void)
{
    int n = 0;
    Node *nodep[NSYMBOLS];

    for (int i = 0; i < NSYMBOLS; i++)
    {
        // カウントの存在しなかったシンボルには何もしない
        if (symbol_count[i] == 0)
            continue;
        nodep[n++] = create_node(i, symbol_count[i], NULL, NULL);
    }

    const int dummy = -1; // ダミー用のsymbol を用意しておく
    while (n >= 2)
    {
        Node *node1 = pop_min(&n, nodep);
        Node *node2 = pop_min(&n, nodep);

        // Create a new node
        // 選ばれた2つのノードを元に統合ノードを新規作成
        // 作成したノードはnodep にどうすればよいか?
        nodep[n++] = create_node(dummy, node1->count + node2->count, node1, node2);
    }

    // なぜ以下のコードで木を返したことになるか少し考えてみよう
    return (n == 0) ? NULL : nodep[0];
}

// Perform depth-first traversal of the tree
// 深さ優先で木を走査する
// 現状は何もしていない（再帰してたどっているだけ）
static char codes[NSYMBOLS][NSYMBOLS + 1];
void traverse_tree(const int depth, const Node *np, char *str)
{
    if (np == NULL || np->right == NULL)
    {
        strcpy(codes[np->symbol], str);
        codes[np->symbol][depth] = '\0';
        // printf("%c (%d): %s\n", np->symbol, np->count, codes[np->symbol]);
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

// この関数は外部 (main) で使用される (staticがついていない)
Node *encode(const char *filename)
{
    reset_count();
    count_symbols(filename);
    Node *root = build_tree();

    if (root == NULL)
    {
        fprintf(stderr, "A tree has not been constructed.\n");
    }

    return root;
}

char *get_code(int symbol)
{
    return strlen(codes[symbol]) ? codes[symbol] : "*";
}

void draw_tree(const Node *node, const int depth, const int is_right, int *is_parent_right)
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
        else if (node->symbol == '\n')
        {
            printf("%s [\\n] %s\n", is_right ? "└──" : (depth == 1 ? "┌──" : "├──"), code);
        }
        else
        {
            printf("%s [%c] %s\n", is_right ? "└──" : (depth == 1 ? "┌──" : "├──"), node->symbol, code);
        }
    }
    is_parent_right[depth] = is_right;
    draw_tree(node->left, depth + 1, 0, is_parent_right);
    draw_tree(node->right, depth + 1, 1, is_parent_right);
}

void write_code(const char *src_filename, const char *dst_filename)
{
    FILE *fp_src = fopen(src_filename, "r");
    if (fp_src == NULL)
    {
        fprintf(stderr, "error: cannot open %s\n", src_filename);
        exit(1);
    }
    FILE *fp_dst = fopen(dst_filename, "wb");
    if (fp_dst == NULL)
    {
        fprintf(stderr, "error: cannot open %s\n", dst_filename);
        exit(1);
    }

    char c = 0;
    while ((c = fgetc(fp_src)) != EOF)
    {
        fwrite(codes[(int)c], sizeof(char), strlen(codes[(int)c]), fp_dst);
        // fwrite(&c, sizeof(int), 1, fp_dst);
    }

    fclose(fp_src);
    fclose(fp_dst);
}
