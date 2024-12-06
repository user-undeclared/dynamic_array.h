/* dynamic_array.h: an ansi c header-only dynamic array library.
   this library has it's own github repository, located at
   https://github.com/user-undeclared/dynamic_array.h

   this file is provided under the MIT license.
   see the end of this file for license information */

#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

/* TODO: figure out a shorter prefix than dynamic_array_ maybe? */

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

#if defined(DYNAMIC_ARRAY_IMPL) || defined(DYNAMIC_ARRAY_IMPLEMENTATION)
#include <stddef.h>
#include <string.h>

#if !defined(DYNAMIC_ARRAY_MALLOC) && !defined(DYNAMIC_ARRAY_REALLOC) && !defined(DYNAMIC_ARRAY_FREE)
#   include <stdlib.h>
#   define DYNAMIC_ARRAY_MALLOC  malloc
#   define DYNAMIC_ARRAY_REALLOC realloc
#   define DYNAMIC_ARRAY_FREE    free
#endif

Dynamic_Array dynamic_array_create_with_capacity(size_t item_size, size_t capacity) {
    Dynamic_Array array;
    void* array_heap_allocation;
    size_t array_heap_allocation_size;

    Dynamic_Array_Header array_header;
    array_header.item_size = item_size;
    array_header.capacity = capacity != 0 ? capacity : DYNAMIC_ARRAY_DEFAULT_INITIAL_CAPACITY;
    array_header.length = 0;

    array_heap_allocation_size = sizeof(array_header) + (array_header.item_size * array_header.capacity);
    array_heap_allocation = DYNAMIC_ARRAY_MALLOC(array_heap_allocation_size);
    if(array_heap_allocation == NULL) {
        return NULL;
    }

    memcpy(array_heap_allocation, &array_header, sizeof(array_header));

    array = ((Dynamic_Array_Header*) array_heap_allocation) + 1;
    return array;
}

void dynamic_array_destroy(Dynamic_Array array) {
    void* array_heap_allocation = ((Dynamic_Array_Header*) array) - 1;
    DYNAMIC_ARRAY_FREE(array_heap_allocation);
}

/* TODO: it would be nice to be able to indicate an error in a way that does not require the user to have a duplicate array variable */
Dynamic_Array dynamic_array_push(Dynamic_Array array, const void* item) {
    Dynamic_Array_Header* array_header;

    void* old_array_heap_allocation;
    void* new_array_heap_allocation;
    size_t new_array_heap_allocation_size;

    size_t item_slot_offset;
    void* item_slot;

    if(array == NULL) {
        return NULL;
    }

    array_header = ((Dynamic_Array_Header*) array) - 1;
    if(array_header->length + 1 > array_header->capacity) {
        array_header->capacity *= 2;

        old_array_heap_allocation = array_header;
        new_array_heap_allocation_size = sizeof(*array_header) + (array_header->capacity * array_header->item_size);
        new_array_heap_allocation = DYNAMIC_ARRAY_REALLOC(old_array_heap_allocation, new_array_heap_allocation_size);
        if(new_array_heap_allocation == NULL) {
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
    return DYNAMIC_ARRAY_REALLOC(array_heap_allocation, array_size);
}

#endif

/*
Copyright 2024 user-undeclared

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
