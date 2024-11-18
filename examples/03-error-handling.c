#include <stdio.h>

#define DYNAMIC_ARRAY_IMPL
#include "dynamic_array.h"

int main(int argc, char* argv[]) {
    int exit_code = 1;

    const char** even_character_count_words = NULL;
    const char** odd_character_count_words = NULL;
    const char** new_words_array = NULL;
    unsigned word_index;

    const char* arg;
    int arg_index;

    if(argc <= 1) {
        fputs("error: too few arguments provided\n", stderr);
        goto abort;
    }

    even_character_count_words = dynamic_array_create(sizeof(*even_character_count_words));
    if(even_character_count_words == NULL) {
        fputs("error: failed to create the even words array\n", stderr);
        goto abort;
    }
    odd_character_count_words = dynamic_array_create(sizeof(*odd_character_count_words));
    if(odd_character_count_words == NULL) {
        fputs("error: failed to create the odd words array\n", stderr);
        goto abort;
    }

    for(arg_index = 1; arg_index < argc; arg_index++) {
        arg = argv[arg_index];
        if(strlen(arg) % 2 == 0) {
            new_words_array = dynamic_array_push(even_character_count_words, &arg);
            if(new_words_array == NULL) {
                fputs("error: failed to push into the even words array\n", stderr);
                goto abort;
            }
            even_character_count_words = new_words_array;
        }
        else {
            new_words_array = dynamic_array_push(odd_character_count_words, &arg);
            if(new_words_array == NULL) {
                fputs("error: failed to push into the odd words array\n", stderr);
                goto abort;
            }
            odd_character_count_words = new_words_array;
        }
    }

    printf("words with an even number of characters:\n   ");
    for(word_index = 0; word_index < dynamic_array_length(even_character_count_words); word_index++) {
        printf(" %s", even_character_count_words[word_index]);
    }
    printf("\nwords with an odd number of characters:\n   ");
    for(word_index = 0; word_index < dynamic_array_length(odd_character_count_words); word_index++) {
        printf(" %s", odd_character_count_words[word_index]);
    }
    putchar('\n');

    exit_code = 0;
abort:
    if(even_character_count_words != NULL) {
        dynamic_array_destroy(even_character_count_words);
    }
    if(odd_character_count_words != NULL) {
        dynamic_array_destroy(odd_character_count_words);
    }
    return exit_code;
}
