#include<heap.h>

void* get_heap(unsigned long long int expected_size, heap_type type, int (*key_compare)(const void* key0, const void* key1))
{
	heap* heap_p = ((heap*)(malloc(sizeof(heap))));
	heap_p->type = type;
	heap_p->key_compare = key_compare;
	heap_p->heap_size = 0;
	heap_p->heap_holder = get_array(expected_size);
	return heap_p;
}

// utility : gets index of the bucket, that is the parent to the given bucket at index = child
unsigned long long int get_parent_index(unsigned long long int child)
{
	return (child-1)/2;
}

// utility : gets index of the bucket, that is the left child to the given parent bucket at index = child
unsigned long long int get_left_child_index(unsigned long long int parent)
{
	return (2 * parent) + 1;
}

// utility : gets index of the bucket, that is the right child to the given parent bucket at index = child
unsigned long long int get_right_child_index(unsigned long long int parent)
{
	return (2 * parent) + 2;
}

// utility : interchanges bucket at indices i1 and i2
void inter_change_buckets_for_indexes(heap* heap_p, unsigned long long int i1, unsigned long long int i2)
{
	bucket* bucket_p_i1 = ((bucket*)get_element(heap_p->heap_holder, i1));
	bucket* bucket_p_i2 = ((bucket*)get_element(heap_p->heap_holder, i2));
	set_element(heap_p->heap_holder, bucket_p_i1, i2);
	set_element(heap_p->heap_holder, bucket_p_i2, i1);
}

// returns true (1) if, the reordering is required, else 0
int is_reordering_required(heap* heap_p, unsigned long long int parent_index, unsigned long long int child_index)
{
	if( parent_index != get_parent_index(child_index) || child_index >= heap_p->heap_holder->total_size)
	{
		// we dont allow reordering if, parent index and child index are not immediately related 
		return 0;
	}

	bucket* parent = ((bucket*)get_element(heap_p->heap_holder, parent_index));
	bucket* child  = ((bucket*)get_element(heap_p->heap_holder, child_index));

	int reordering_required = 0;

	switch(heap_p->type)
	{
		case MIN_HEAP :
		{
			// in min heap, parent has to be smaller than child, we have an issue if parent is greater than child 
			if(heap_p->key_compare(parent->key, child->key) > 0)
			{
				reordering_required = 1;
			}
			break;
		}
		case MAX_HEAP :
		{
			// in min heap, parent has to be greater than child, we have an issue if parent is smaller than child 
			if(heap_p->key_compare(parent->key, child->key) < 0)
			{
				reordering_required = 1;
			}
			break;
		}
	}

	return reordering_required;
}

void bubble_up(heap* heap_p, unsigned long long int index)
{
	// an element at index 0, or thew index is out of range, can not be bubbled up
	if(index == 0 || index >= heap_p->heap_holder->total_size)
	{
		return;
	}

	// get parent index for the index
	unsigned long long int parent_index = get_parent_index(index);

	// if a reordering is required, we interchange the parent and child bucket
	if(is_reordering_required(heap_p, parent_index, index))
	{
		inter_change_buckets_for_indexes(heap_p, parent_index, index);
		bubble_up(heap_p, parent_index);
	}
}

void push(heap* heap_p, void* key, void* value)
{
	// expand heap_holder if necessary
	if(heap_p->heap_size >= heap_p->heap_holder->total_size)
	{
		expand_array(heap_p->heap_holder);
	}

	// set the element to the last index and increment its size
	set_element(heap_p->heap_holder, get_bucket(key, value), heap_p->heap_size++);

	// bubble up element at index heap_p->heap_size-1
	bubble_up(heap_p, heap_p->heap_size-1);
}

const void* get_top(heap* heap_p)
{
	// ther is no top bucket value, if there are no buckets in the heap
	if(heap_p->heap_size == 0)
	{
		return NULL;
	}

	// return the value of the first bucket, from the heap holder
	return ((bucket*)get_element(heap_p->heap_holder, 0))->value;
}

void bubble_down(heap* heap_p, unsigned long long int index)
{

}

void pop(heap* heap_p)
{
	// we can pop only if the heap size if greater than 0, and there is atleast 1 element
	if(heap_p->heap_size > 0)
	{
		// put the first bucket at last and last bucket to first
		inter_change_buckets_for_indexes(heap_p, 0, heap_p->heap_size-1);

		// get the last bucket and delete it
		delete_bucket((bucket*)get_element(heap_p->heap_holder, heap_p->heap_size-1));

		// and set the last element to null
		set_element(heap_p->heap_holder, NULL, --heap_p->heap_size);

		// bubble down the element at index 0
		bubble_down(heap_p, 0);
	}
}

void delete_bucket_wrapper(void* data_p, unsigned long long int index, const void* additional_params)
{
	if(data_p != NULL)
	{
		delete_bucket(data_p);
	}
}

void delete_heap(heap* heap_p)
{
	for_each_in_array(heap_p->heap_holder, delete_bucket_wrapper, NULL);
	delete_array(heap_p->heap_holder);
	free(heap_p);
}