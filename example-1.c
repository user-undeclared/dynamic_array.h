#include <stddef.h>
#include <stdio.h>

#define DYNAMIC_ARRAY_IMPLEMENTATION
#include "dynamic_array.h"

int main(void) {
    unsigned index, number_count;
    int* numbers, number;

    numbers = dynamic_array_create(sizeof(*numbers));

    number = 1;
    numbers = dynamic_array_push(numbers, &number);
    numbers = dynamic_array_push(numbers, &number);

    number_count = dynamic_array_length(numbers);
    while(numbers[number_count - 1] < 800) {
        number = numbers[number_count - 1] + numbers[number_count - 2];
        numbers = dynamic_array_push(numbers, &number);
        number_count = dynamic_array_length(numbers);
    }

    for(index = 0; index < dynamic_array_length(numbers); index++) {
        printf("%3i\n", numbers[index]);
    }

    dynamic_array_destroy(numbers);
    return 0;
}
