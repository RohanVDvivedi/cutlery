#include<memory_allocator_interface.h>

#include<stdlib.h>
#include<string.h>

void* STD_C_mem_alloc_impl(void* old_memory, unsigned int old_size, unsigned int new_size, unsigned int new_alignment, memory_allocator_initialization initialization)
{
	int new_memory_allocation_failed = 0;
	void* new_memory = NULL;

	if(new_size > 0)	// a new memory allocation is required
	{
		if(new_alignment == 0 || new_alignment == 1)	// case : no alignment constraint
		{
			switch(initialization)
			{
				case DONT_CARE :
				{
					new_memory = malloc(new_size);
					break;
				}
				case ZERO :
				{
					new_memory = calloc(new_size, 1);
					break;
				}
				case PRESERVE :
				{
					if(old_memory != NULL && old_size > 0)
					{
						new_memory = realloc(old_memory, new_size);
						if(new_memory != NULL)	// a successfull allocation suggests the old_memory was freed
						{	// this if statement, protects you from double free errors, fatal on some systems
							old_memory = NULL;
							old_size = 0;
						}
					}
					else
						new_memory = malloc(new_size);
					break;
				}
				default:{break;}
			}
		}
		else	// case : fixed alignment on return address
		{
			new_memory = aligned_alloc(new_alignment, new_size);
			if(new_memory != NULL)
			{
				switch(initialization)
				{
					case ZERO :
					{
						memset(new_memory, 0, new_size);
						break;
					}
					case PRESERVE :
					{
						if(old_memory != NULL && old_size > 0)
							memcpy(new_memory, old_memory, ((new_size < old_size) ? new_size : old_size));
						break;
					}
					default:{break;}
				}
			}
		}

		// a NULL return from any of the stdlibc allocation function means a failure, so set the flag
		if(new_memory == NULL)
			new_memory_allocation_failed = 1;
	}

	// please note :
	// that at this point in time (of execution at runtime) in this function
	// if new_size was 0, then a new memory allocation is not considered failed,
	// since a memory allocation was not even required, in the first place

	// a new_size == 0, refers to a case where only deallocation of old memory is required

	if(old_memory != NULL && old_size > 0 &&	// there is some old_memory to free
	 !new_memory_allocation_failed)		// the new_allocation succeeded OR was not required
	{
		free(old_memory);
		// and set to NULL and 0, just to keep things clean
		old_memory = NULL;
		old_size = 0;
	}

	return new_memory;
}
const memory_allocator STD_C_mem_alloc = STD_C_mem_alloc_impl;