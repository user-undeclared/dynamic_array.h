#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DYNAMIC_ARRAY_DEFAULT_INITIAL_CAPACITY 64

typedef void* Dynamic_Array;

typedef struct {
    size_t item_size;
    size_t capacity;
    size_t length;
} Dynamic_Array_Header;

#define dynamic_array_length(array) ((((Dynamic_Array_Header*) (array)) - 1)->length)

#define dynamic_array_create(item_size) dynamic_array_create_with_capacity((item_size), 0)

Dynamic_Array dynamic_array_create_with_capacity(size_t item_size, size_t capacity) {
    Dynamic_Array array;
    void* array_heap_allocation;
    size_t array_heap_allocation_size;

    Dynamic_Array_Header array_header;
    array_header.item_size = item_size;
    array_header.capacity = capacity != 0 ? capacity : DYNAMIC_ARRAY_DEFAULT_INITIAL_CAPACITY;
    array_header.length = 0;

    array_heap_allocation_size = array_header.item_size * array_header.capacity;
    array_heap_allocation = malloc(sizeof(array_header) + array_heap_allocation_size);
    if(array_heap_allocation == NULL) {
        return NULL;
    }

    memcpy(array_heap_allocation, &array_header, sizeof(array_header));

    array = ((Dynamic_Array_Header*) array_heap_allocation) + 1;
    return array;
}

void dynamic_array_destroy(Dynamic_Array array) {
    void* array_heap_allocation = ((Dynamic_Array_Header*) array) - 1;
    free(array_heap_allocation);
}

Dynamic_Array dynamic_array_push(Dynamic_Array array, void* item) {
    Dynamic_Array_Header* array_header = ((Dynamic_Array_Header*) array) - 1;

    void* array_heap_allocation;
    void* new_array_heap_allocation;
    size_t new_array_heap_allocation_size;

    size_t item_slot_offset;
    void* item_slot;

    if(array_header->length + 1 >= array_header->capacity) {
        array_header->capacity *= 2;

        array_heap_allocation = ((Dynamic_Array_Header*) array) - 1;
        new_array_heap_allocation_size = array_header->capacity * array_header->item_size;
        new_array_heap_allocation = realloc(array_heap_allocation, new_array_heap_allocation_size);
        if(new_array_heap_allocation == NULL) {
            return NULL;
        }

        array = ((Dynamic_Array_Header*) new_array_heap_allocation) + 1;
    }

    item_slot_offset = array_header->length * array_header->item_size;
    item_slot = ((unsigned char*) array) + item_slot_offset;
    array_header->length++;
    memcpy(item_slot, item, array_header->item_size);
    return array;
}

int main(void) {
    unsigned counter, index;
    int* xs, x;

    xs = dynamic_array_create(sizeof(*xs));

    x = 1;
    xs = dynamic_array_push(xs, &x);
    xs = dynamic_array_push(xs, &x);

    for(counter = 0; counter < 14; counter++) {
        x = xs[dynamic_array_length(xs) - 1] + xs[dynamic_array_length(xs) - 2];
        xs = dynamic_array_push(xs, &x);
    }

    for(index = 0; index < dynamic_array_length(xs); index++) {
        printf("%3i\n", xs[index]);
    }

    dynamic_array_destroy(xs);
    return 0;
}