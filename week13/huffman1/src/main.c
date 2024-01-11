#include <stdio.h>
#include <stdlib.h>
#include "encode.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <src filename> <dst filename>\n", argv[0]);
        exit(1);
    }

    Node *root = encode(argv[1]);

    traverse_tree(0, root, "");

    int *is_parent_right = (int *)calloc(256, sizeof(int));
    is_parent_right[0] = 1;
    draw_tree(root, 0, 1, is_parent_right);

    write_code(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
