#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <getopt.h>

#include <unistd.h>
#include <ctype.h>

#include "mylib.h"
#include "htable.h"
#include "tree.h"

/*#define TABLE_SIZE 113*/
#define TRUE 1
#define FALSE 0

static void print_help(){
    printf("-T\t\tUse a tree data structure (the default is a hash table)\n\n");
    printf("-c filename\tCheck the spelling of words in filename using words read from stdin as the dictionary.\n ");
    printf("\tPrint all unknown words to stdout.\n");
    printf("\tPrint timing information and unknown word count to stderr.\n");
    printf("\tWhen this option is given the -p or -o options should be ignored.\n\n");
    printf("-d\t\tUse double hashing as the collision resolution strategy (linear probing is the default).\n\n");
    printf("-e\t\tDisplay entire contents of hash table on stderr using the format string \"%5d␣%5d␣%5d␣␣␣%s\n\" to display the index, frequency,stats, and the key if it exists.\n ");
    printf("\t(Note that spaces have been made visible in the format string so you can see how many there are).\n\n");
    printf("-o\t\tOutput a representation of the tree in \"dot\" form to the file ‘tree-view.dot’ using the functions given in output-dot.txt.\n\n");
    printf("-p\t\tPrint stats info using the functions provided in print-stats.txt instead of printing the frequencies and words.\n\n");
    printf("-r\t\tMake the tree an rbt instead of the default bst.\n\n");
    printf("-s snapshots\tDisplay up to the given number of stats snapshots when given -p as an argument.\n");
    printf("\tIf the table is not full then fewer snapshots will be displayed. Snapshots with 0 entries are not shown.\n\n");
    printf("-t tablesize\tUse the first prime >= tablesize as the size of your hash table.\n");
    printf("\tYou can assume that tablesize will be a number greater than 0.\n\n");
    printf("-h\t\tPrint a help message describing how to use the program\n\n");
}

static int is_prime(int num)
{
    if (num <= 1) return 0;
    if (num % 2 == 0 && num > 2) return 0;
    for(int i = 3; i < num / 2; i+= 2)
        {
            if (num % i == 0)
                return 0;
        }
    return 1;
}

static void print_info(int freq, char *word) {
    printf("%-4d %s\n", freq, word);
}

int main(int argc, char **argv) {


    const char *optstring = "Tc:deprsth";
    char option;

    int tablesize = 113;
    hashing_t type = LINEAR_P;
    htable h;
    tree t;

    clock_t start_fill, end_fill, start_search, end_search;
    char word[256];

    int i;
    int unknown = 0;
    FILE *fp;

    int flag_T = FALSE;

    int flag_c = FALSE;

    int flag_e = FALSE;

    int flag_o = FALSE;

    int flag_p = FALSE;

    int flag_r = FALSE;

    int flag_t = FALSE;

    while ((option = getopt(argc, argv, optstring)) != EOF) {
        switch (option) {
            case 'T':
                printf("T was entered\n");
                flag_T = TRUE;
                break;

            case 'c':
                printf("c was entered\n");
                flag_c = TRUE;
                break;

            case 'd':
                printf("d was entered\n");
                type = DOUBLE_H;
                break;

            case 'e':
                printf("e was entered\n");
                flag_e = TRUE;
                break;

            case 'o':
                printf("o was entered\n");
                flag_o = TRUE;
                break;

            case 'p':
                printf("p was entered\n");
                flag_p = TRUE;
                break;

            case 'r':
                printf("r was entered\n");
                flag_r = TRUE;
                break;

            case 's':
                printf("s was entered\n");
                break;

            case 't':
                printf("t was entered\n");
                flag_t = TRUE;
                break;

            case 'h':
                printf("h was entered\n");
                print_help();
                break;

            default:
                /* if an unknown option is given */
                printf("default\n");
                break;
        }
    }
    
    if(flag_T){

        if(flag_r){
            tree t = tree_new(RBT);
        }
        else {
            tree t = tree_new(BST);
        }

        start_fill = clock();
        while (getword(word, sizeof word, stdin) != EOF) {
            tree_insert(t, word);
        }
        end_fill = clock();
        
        if(flag_c){
            fp = fopen(optarg, "r");
            if(NULL == fp){
                fprintf(stderr, "Failed to open file.\n");
                exit(EXIT_FAILURE);
            }
            start_search = clock();
            while(getword(word, sizeof word, fp) != EOF){
                if(tree_search(t, word) == 0){
                    unknown++;
                    fprintf(stderr, "Unkown Word: %s\n", word);
                }
            }
            fclose(fp);
            end_search = clock();

            printf("Fill time     : %f\n", (end_fill - start_fill) / (double) CLOCKS_PER_SEC);
            printf("Search time   : %f\n", (end_search - start_search) / (double) CLOCKS_PER_SEC);
            printf("Unknown Count = %d\n", unknown);
        }

        if(flag_o && !flag_c){
            tree_output_dot(t, stdout);
        }
        
        tree_free(t);
        return EXIT_SUCCESS;
        
    } else {
        
        if(flag_t){
            optarg = *fp;
            value = atoi(optarg);
            i = value;
            while(!is_prime(i)){
                i++
                    }
            tablesize = i;
        }
        
    h = htable_new(tablesize, type);

    start_fill = clock();
    while (getword(word, sizeof word, stdin) != EOF) {
        htable_insert(h, word);
    }
    end_fill = clock();

    if(flag_c){
        fp = fopen(optarg, "r");
        if(NULL == fp){
            fprintf(stderr, "Failed to open file.\n");
            exit(EXIT_FAILURE);
        }
        start_search = clock();
        while(getword(word, sizeof word, fp) != EOF){
            if(htable_search(h, word) == 0){
                unknown++;
                fprintf(stderr, "Unkown Word: %s\n", word);
            }
        }
        fclose(fp);
        end_search = clock();

        printf("Fill time     : %f\n", (end_fill - start_fill) / (double) CLOCKS_PER_SEC);
        printf("Search time   : %f\n", (end_search - start_search) / (double) CLOCKS_PER_SEC);
        printf("Unknown Count = %d\n", unknown);
    }

    print_info();

    if(flag_e){
        htable_print_entire_table(h, stderr);
    }

    if(flag_p){
        htable_print_stats(h, stderr, tablesize);
    }

    htable_free(h);
    
    return EXIT_SUCCESS;
    }
}
