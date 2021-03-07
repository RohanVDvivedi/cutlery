#include<array.h>
#include<heap.h>

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

#include<stdlib.h>

void merge_sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > end_index || end_index >= array_p->total_size)
		return;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	unsigned int total_elements = end_index - start_index + 1;
	if(total_elements <= 1)
		return;

	// we iteratively merge adjacent sorted chunks from src and store them in dest
	const void** src  = array_p->data_p_p + start_index;
	const void** dest = malloc(sizeof(void*) * total_elements);

	// start with sorted chunk size equals 1, (a single element is always sorted)
	unsigned int sort_chunk_size = 1;
	while(sort_chunk_size <= total_elements)
	{

		// in each iteration of the internal loop
		// merge 2 adjacent sorted chunks of src array
		// to form 1 chunk of twice the size in dest array
		unsigned int dest_index = 0;
		while(dest_index < total_elements)
		{
			// start and end indices of chunk 1
			unsigned int a_start = dest_index;
			unsigned int a_end = a_start + sort_chunk_size - 1;

			// start and end indices of chunk 2
			unsigned int b_start = a_end + 1;
			unsigned int b_end = b_start + sort_chunk_size - 1;

			// *_start and *_end are both inclusive indices

			if(b_start > total_elements - 1)
			{
				if(a_end > total_elements - 1)
					a_end = total_elements - 1;

				memory_move(dest + dest_index, src + a_start, (a_end - a_start + 1) * sizeof(void*));
				break;
			}
			else
			{
				if(b_end > total_elements - 1)
					b_end = total_elements - 1;

				while(dest_index <= b_end)
				{
					if((b_start > b_end) || (a_start <= a_end && compare(src[a_start], src[b_start]) < 0))
						dest[dest_index++] = src[a_start++];
					else
						dest[dest_index++] = src[b_start++];
				}
			}
		}

		// src becomes dest, and dest becomes src
		const void** temp = src;
		src = dest;
		dest = temp;

		// double the chunk size, for next iteration
		sort_chunk_size = sort_chunk_size * 2;
	}

	// free the extra memory
	if((array_p->data_p_p + start_index) == src)
		free(dest);
	else
	{
		memory_move(array_p->data_p_p + start_index, src, total_elements * sizeof(void*));
		free(src);
	}
}

void heap_sort_array(array* array_p, unsigned int start_index, unsigned int end_index, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > end_index || end_index >= array_p->total_size)
		return;

	// compute the number of elements to sort; 0 or 1 number of elements do not need sorting
	unsigned int total_elements = end_index - start_index + 1;
	if(total_elements <= 1)
		return;

	// create a min heap
	heap sort_heap;
	initialize_heap(&sort_heap, total_elements, MIN_HEAP, compare, NULL, NULL);

	// push all the elements in the min heap
	for(unsigned int i = start_index; i <= end_index; i++)
		push_heap(&sort_heap, get_element(array_p, i));

	// place the top of the heap element in the array, then pop heap
	for(unsigned int i = start_index; i <= end_index; i++)
	{
		const void* min_data = get_top_heap(&sort_heap);
		set_element(array_p, min_data, i);
		pop_heap(&sort_heap);
	}

	// destroy the temporary heap
	deinitialize_heap(&sort_heap);
}


unsigned int linear_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > end_index || end_index >= array_p->total_size)
		return array_p->total_size;

	for(unsigned int i = start_index; i <= end_index; i++)
	{
		if(compare(get_element(array_p, i), data_p) == 0)
			return i;
	}

	// we return answer or return an element out of bounds
	return array_p->total_size;
}

unsigned int binary_search_in_array(const array* array_p, unsigned int start_index, unsigned int end_index, const void* data_p, int (*compare)(const void* data1, const void* data2))
{
	if(start_index > end_index || end_index >= array_p->total_size)
		return array_p->total_size;

	if(compare(get_element(array_p, start_index), data_p) >= 0)
		return start_index;

	if(compare(get_element(array_p, end_index), data_p) <= 0)
		return end_index;

	unsigned int mid = (start_index + end_index) / 2;
	while(start_index < end_index)
	{
		int cmp = compare(get_element(array_p, mid), data_p);
		if(cmp > 0)
			end_index = mid;
		else if(cmp < 0)
			start_index = mid;
		else
			return mid;

		mid = (start_index + end_index) / 2;

		if(mid == start_index || mid == end_index)
			break;
	}

	// we return answer else return some other element closer to it
	return mid;
}