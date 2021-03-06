#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "mylib.h"
#include "tree.h"

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

int main(void) {
    tree b = tree_new(0);
    char word[256];
    char op;
    while (2 == scanf(" %c %255s", &op, word)) {
        if ('+' == op) {
            b = tree_insert(b, word);
        } else if ('?' == op) {
            printf("%d %s\n", tree_search(b, word), word);
        }
    }
    printf("\n\n");
    tree_inorder(b, print_info);
    tree_free(b);
    return EXIT_SUCCESS;
} 
