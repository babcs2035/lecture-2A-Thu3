#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LZSS.h"
#include "huffman.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <src filename (without extension)>\n", argv[0]);
        exit(1);
    }

    char *filename = (char *)malloc(sizeof(char) * MAX_TEXT_LENGTH);
    strcpy(filename, argv[1]);
    sprintf(filename, "%s.txt", argv[1]);
    LZSS_Node *LZSS_root = compress(filename);

    huffman_Node *huffman_root = encode(LZSS_root);
    traverse_tree(0, huffman_root, "");

    sprintf(filename, "%s_compressed.dat", argv[1]);
    write_code(LZSS_root, filename);

    // LZSS_root = read_code(filename, huffman_root);
    // const char *str = extract(LZSS_root);
    // printf("%s\n", str);

    return EXIT_SUCCESS;
}
