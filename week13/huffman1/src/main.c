#include <stdio.h>
#include <stdlib.h>
#include "LZSS.h"
#include "encode.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <src filename>\n", argv[0]);
        exit(1);
    }

    LZ77_Node *root = compress(argv[1]);
    const char *str = extract(root);

    // Node *root = encode(argv[1]);
    // traverse_tree(0, root, "");
    // write_code(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
