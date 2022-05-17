// We were aided by this site:
// https://danluu.com/malloc-tutorial/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


#define META_SIZE sizeof(struct block_meta)

void *global_base = NULL;

struct block_meta {
  size_t size;
  struct block_meta *next;
  int free;
  int magic; 
};


struct block_meta *find_free_block(struct block_meta **last, size_t size)
{
    struct block_meta *current = (block_meta *)global_base;
    while (current && !(current->free && current->size >= size)) 
    {
        *last = current;
        current = current->next;
    }
    return current;
}

struct block_meta *request_space(struct block_meta* last, size_t size) 
{
    struct block_meta *block;
    block = (block_meta *)sbrk(0);
    void *request = sbrk(size + META_SIZE);
    assert((void*)block == request); // Not thread safe.

    if (request == (void*) -1) 
    {
        return NULL; // sbrk failed.    
    }

    if (last) 
    { // NULL on first request.
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0;
    block->magic = 0x12345678;
    return block;
}

void *malloc(size_t size) {
    struct block_meta *block;
    if (size <= 0) 
    {
        return NULL;
    }

    if (!global_base) 
    { 
        block = request_space(NULL, size);
        if (!block) 
        {
           return NULL;
        }
        global_base = block;
    } else {
        struct block_meta *last = (block_meta *)global_base;
        block = find_free_block(&last, size);
        if (!block) 
        { // Failed to find free block.
            block = request_space(last, size);
            if (!block) 
            {
                return NULL;
            }
        } else {      // Found free block
       
            block->free = 0;
            block->magic = 0x77777777;
        }
    }

    return(block+1);
}

struct block_meta *get_block_ptr(void *ptr) 
{
    return (struct block_meta*)ptr - 1;
}

void free(void *ptr) {
    if (!ptr) 
    {
        return;
    }
    struct block_meta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->free == 0);
    assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
    block_ptr->free = 1;
    block_ptr->magic = 0x55555555;
}

void *calloc(size_t nelem, size_t elsize) 
{
    size_t size = nelem * elsize; 
    void *ptr = malloc(size);
    memset(ptr, 0, size);
    return ptr;
}

 
// int main()
// {
 
//     // This pointer will hold the
//     // base address of the block created
//     int *ptr, *ptr1;
//     int n, i;
 
//     // Get the number of elements for the array
//     n = 5;
//     printf("Enter number of elements: %d\n", n);
 
//     // Dynamically allocate memory using malloc()
//     ptr = (int*)malloc(n * sizeof(int));
 
//     // Dynamically allocate memory using calloc()
//     ptr1 = (int*)calloc(n, sizeof(int));
 
//     // Check if the memory has been successfully
//     // allocated by malloc or not
//     if (ptr == NULL || ptr1 == NULL) {
//         printf("Memory not allocated.\n");
//         exit(0);
//     }
//     else {
 
//         // Memory has been successfully allocated
//         printf("Memory successfully allocated using malloc.\n");
 
//         // Free the memory
//         free(ptr);
//         printf("Malloc Memory successfully freed.\n");
 
//         // Memory has been successfully allocated
//         printf("\nMemory successfully allocated using calloc.\n");
 
//         // Free the memory
//         free(ptr1);
//         printf("Calloc Memory successfully freed.\n");
//     }
 
//     return 0;
// }