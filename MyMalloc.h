#ifndef MEM_ALLOC_H
#define MEM_ALLOC_H

#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// ##############################################################
/** 
 * 
 *                      Infrastructure
 * 
 * */
// ##############################################################

void *global_list_base = NULL;

typedef struct metadata_header_block{
    size_t size;
    int unsigned free; // 1 - block is free, 0 - occupied.
    struct metadata_header_block *next; // pointer for the next at the linkedlist
    char magic_debugger; // for debugging option
} metadata;

#define METADATA_SIZE sizeof(metadata)

#define METADATA_PTR_SIZE sizeof(metadata *)

metadata *find_free_block(metadata **last, size_t requested_size);

metadata *request_memory_space(metadata *last, size_t requested_size);

metadata *get_block_ptr(void *ptr);

void merge_empty_cells();


void *malloc(size_t requested_size);

void free(void *ptr);

// ##########################################################
/**                     
 *                          
 *                      Utility Functions
 * 
 * */
// ##########################################################

#define PRINT_DEBUG(msg) printf("[DEBUG]=> %s\n", msg);

#define PRINT_METADATA_BLOCK(block){\
            printf("######## METADATA HEADER\n");\
            printf("METADATA : addr = (%p)\n", block);\
            printf("METADATA : free (%d)\n", block->free);\
            printf("METADATA : magic (%c)\n", block->magic_debugger);\
            printf("METADATA : size (%ld)\n", block->size);\
            printf("METADATA : next (%p)\n", block->next);\
            printf("METADATA : block + 1 addr = (%p)\n", block + 1);\
            printf("######## END  METADATA HEADER\n");\
                        }


// ##########################################################
/**                     
 *                          
 *                      Functions
 * 
 * */
// ##########################################################




/**
 * iterate through the memory linked list.
 * if we find a fitting block we can use it.
 * 
* */
metadata *find_free_block(metadata **last, size_t requested_size)
{
    //PRINT_DEBUG("At find_free_block");

    metadata *current = global_list_base;

    while(current != NULL && !(current->free && current->size >= requested_size)) // While not null and not( free && size comptiable)
    {
        *last = current; // Last is now pointing at current
        current = current->next; // move up current in the linked list.
    }
    

    if (current != NULL)
        {
        if (current->size > requested_size)
        {
            size_t remaining_space = current->size - requested_size;
            if (remaining_space > METADATA_SIZE) // We can fit a new block
            {
                // let's create a new node
                metadata newnode;

                newnode.free = 1;
                newnode.size = current->size - requested_size - METADATA_SIZE; // Size that will remain...
                newnode.next = current->next; // Use the new next...
                newnode.magic_debugger = 'C';
                
                char *cursr = (char *)(current + 1); // Pointer to current raw data section.

                memcpy(cursr + current->size, &newnode, METADATA_SIZE); // COPY newnode to heap starting at size location...

                metadata *ptr = (metadata *)((void*)(cursr + current->size));

                current->next = ptr;
                current->size = requested_size;
                
            }
            

        }
    }
    
    

    return current; // Returning NULL or a valid node. save last as backup..
    // By defenition last will point at a non-null space.
}


/***
 *  when searching the list failed, trying to allocate more memory using the sbrk syscall
 *  
**/
metadata *request_memory_space(metadata *last, size_t requested_size)
{
    //PRINT_DEBUG("At request_memory_space");

    metadata *block = NULL; // ALWAYS initalize!!!
    void *request = NULL;

    block = sbrk(0); // Returns a pointer to the current end of DS.

    request = sbrk(requested_size + METADATA_SIZE); // Push DS limit further to allocate place
    // Upon success sbrk returns pointer to the prior last break, meaning the if block == request thus, sbrk was success.

    assert((void*)block == request); // Exit if exp is false.

    if(request == (void*) -1)
    {
        return NULL;
    }

    if (last) // If not NULL
    {
        last->next = block;
    }

    block->size = requested_size;

    block->next = NULL;
    block->free = 0; //Occupied
    block->magic_debugger = 'A';

    return block;
}

metadata *get_block_ptr(void *ptr)
{
    return (metadata *)ptr - 1; // Return pointer to header...
}

void merge_empty_cells()
{
    //PRINT_DEBUG("At merge_empty_cells");

    metadata *curr = global_list_base;
    size_t total_new_size = 0;
    

    while (curr != NULL) // pass at all nodes in the link list
    {
        total_new_size = 0;
        if (curr->next != NULL && curr->free == 1) // there is another node and curr is free
        {
            if (curr->next->free == 1) // It's available
            {
                // Start merging...
                // New size...
                total_new_size = curr->size + curr->next->size + METADATA_SIZE;
                //printf("%ld + %ld + %ld = %ld\n", curr->size, curr->next->size, METADATA_SIZE, total_new_size);
                curr->size = total_new_size;

                //change linking...
                curr->next = curr->next->next; // Gain the next next's

                //clean memory area:
                memset(curr + 1, 0, total_new_size); // (curr + 1) at the start of the allocation section

            }
            
    
        }

		curr = curr->next;
        
    }
    
}

#endif
