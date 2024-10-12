#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#ifndef DYNAMIC_ARRAY_DEFAULT_INITIAL_CAPACITY
#   define DYNAMIC_ARRAY_DEFAULT_INITIAL_CAPACITY 64
#endif

typedef void* Dynamic_Array;

typedef struct {
    size_t item_size;
    size_t capacity;
    size_t length;
} Dynamic_Array_Header;

#define dynamic_array_length(array) (((Dynamic_Array_Header*) (array)) - 1)->length

#define dynamic_array_create(item_size) dynamic_array_create_with_capacity((item_size), 0)
Dynamic_Array dynamic_array_create_with_capacity(size_t item_size, size_t capacity);
void dynamic_array_destroy(Dynamic_Array array);

Dynamic_Array dynamic_array_push(Dynamic_Array array, const void* item);

void* dynamic_array_to_heap_allocated_array(Dynamic_Array array);

#endif /* DYNAMIC_ARRAY_H_ */

#ifdef DYNAMIC_ARRAY_IMPLEMENTATION
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

Dynamic_Array dynamic_array_create_with_capacity(size_t item_size, size_t capacity) {
    Dynamic_Array array;
    void* array_heap_allocation;
    size_t array_heap_allocation_size;

    Dynamic_Array_Header array_header;
    array_header.item_size = item_size;
    array_header.capacity = capacity != 0 ? capacity : DYNAMIC_ARRAY_DEFAULT_INITIAL_CAPACITY;
    array_header.length = 0;

    array_heap_allocation_size = sizeof(array_header) + (array_header.item_size * array_header.capacity);
    array_heap_allocation = malloc(array_heap_allocation_size);
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

Dynamic_Array dynamic_array_push(Dynamic_Array array, const void* item) {
    Dynamic_Array_Header* array_header = ((Dynamic_Array_Header*) array) - 1;

    void* old_array_heap_allocation;
    void* new_array_heap_allocation;
    size_t new_array_heap_allocation_size;

    size_t item_slot_offset;
    void* item_slot;

    if(array_header->length + 1 > array_header->capacity) {
        array_header->capacity *= 2;

        old_array_heap_allocation = ((Dynamic_Array_Header*) array) - 1;
        new_array_heap_allocation_size = sizeof(*array_header) + (array_header->capacity * array_header->item_size);
        new_array_heap_allocation = realloc(old_array_heap_allocation, new_array_heap_allocation_size);
        if(new_array_heap_allocation == NULL) {
            free(old_array_heap_allocation);
            return NULL;
        }

        array_header = new_array_heap_allocation;
        array = array_header + 1;
    }

    item_slot_offset = array_header->length * array_header->item_size;
    item_slot = ((unsigned char*) array) + item_slot_offset;
    array_header->length++;
    memcpy(item_slot, item, array_header->item_size);
    return array;
}

void* dynamic_array_to_heap_allocated_array(Dynamic_Array array) {
    void* array_heap_allocation = ((Dynamic_Array_Header*) array) - 1;
    Dynamic_Array_Header* array_header = array_heap_allocation;
    const size_t array_size = array_header->length * array_header->item_size;
    memmove(array_heap_allocation, array, array_size);
    return realloc(array_heap_allocation, array_size);
}

#endif /* DYNAMIC_ARRAY_IMPLEMENTATION */