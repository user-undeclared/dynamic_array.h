#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

char* repeat_string(const char* string, unsigned repeat_count);

int main(void) {
    const char* const message = "happy happy joy joy ";
    const unsigned message_repetition_count = 4;
    unsigned character_index;
    char* character;

    char* repeated_message = repeat_string(message, message_repetition_count);
    if(repeated_message == NULL) {
        return 1;
    }
    for(character_index = 0; character_index < strlen(repeated_message); character_index++) {
        character = &repeated_message[character_index];
        *character = toupper(*character);
    }

    puts(repeated_message);

    free(repeated_message);
    return 0;
}

#define DYNAMIC_ARRAY_IMPLEMENTATION
#include "dynamic_array.h"

char* repeat_string(const char* string, unsigned repeat_count) {
    char* repeated_string;
    size_t string_length;
    unsigned repeat_index, character_index;
    const char nul = '\0';

    if(string == NULL || repeat_count == 0) {
        return NULL;
    }
    string_length = strlen(string);

    repeated_string = dynamic_array_create_with_capacity(sizeof(*repeated_string), string_length + 1);
    if(repeated_string == NULL) {
        return NULL;
    }

    for(repeat_index = 0; repeat_index < repeat_count; repeat_index++) {
        for(character_index = 0; character_index < string_length; character_index++) {
            repeated_string = dynamic_array_push(repeated_string, &string[character_index]);
            if(repeated_string == NULL) {
                return NULL;
            }
        }
    }
    repeated_string = dynamic_array_push(repeated_string, &nul);

    return dynamic_array_to_heap_allocated_array(repeated_string);
}
