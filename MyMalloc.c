#include "MyMalloc.h"


void *malloc(size_t requested_size)
{

	metadata *block = NULL;

	if (requested_size <= 0)
	{
		return NULL; // Nice try script kiddies...
	}
	
	if (!global_list_base) // First time using talloc...
	{
		//PRINT_DEBUG("First Time");
		
		block = request_memory_space(NULL, requested_size);
		if (!block) // If NULL
		{
			return NULL;
		}
		
		global_list_base = block;

	}
	else // Not first call
	{

		metadata *last = global_list_base;

		block = find_free_block(&last, requested_size);

		if (!block) // Failed to find free block...
		{
			block = request_memory_space(last, requested_size);
			if (!block)
			{
				return NULL;
			}
			
		}
		else
		{ // Node created by free block is now occupent
			// TODO: splitting block becuse size might be to big.
			block->free = 0;
			block->magic_debugger = 'B';
		}
	}
	
	//PRINT_METADATA_BLOCK(block);

	// Return pointer...
	return (block + 1);

}

void free(void *ptr)
{
	//PRINT_DEBUG("At tFree");	
	
	if (!ptr) // ptr == NULL, script kiddies
	{
		return;
	}

	//TODO: consider merging block once splitting blocks is implemented.
	metadata *block_ptr = get_block_ptr(ptr);
	assert(block_ptr->free == 0);
	assert(block_ptr->magic_debugger == 'A' || block_ptr->magic_debugger == 'B');

	block_ptr->free = 1; //Set block as free...
	block_ptr->magic_debugger = 'C';

	memset(ptr, 0, block_ptr->size);
	
	merge_empty_cells();

}

