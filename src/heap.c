#include<heap.h>

#include<cutlery_stds.h>

// utility : gets index of the parent data to the data at index = child
static unsigned int get_parent_index(unsigned int child)
{
	return (child-1)/2;
}

// utility : gets index of the left child to the data at index = parent
static unsigned int get_left_child_index(unsigned int parent)
{
	return (2 * parent) + 1;
}

// utility : gets index of the right child to the data at index = parent
static unsigned int get_right_child_index(unsigned int parent)
{
	return (2 * parent) + 2;
}

// utility : interchanges data elements at indices i1 and i2
static void inter_change_elements_for_indexes(heap* heap_p, unsigned int i1, unsigned int i2)
{
	swap_elements(&(heap_p->heap_holder), i1, i2);

	// once the elements have been interchanged we call update index on the elements
	if(heap_p->heap_index_update_callback != NULL)
	{
		heap_p->heap_index_update_callback(get_element(&(heap_p->heap_holder), i1), i1, heap_p->callback_params);
		heap_p->heap_index_update_callback(get_element(&(heap_p->heap_holder), i2), i2, heap_p->callback_params);
	}
}

// returns true (1) if, the reordering is required, else 0
// we do not check if parent index is actually the parent of the child
// hence, this function can be used to test if the order could be made correct
static int is_reordering_required(const heap* heap_p, unsigned int parent_index, unsigned int child_index)
{
	if(parent_index >= heap_p->element_count || child_index >= heap_p->element_count)
	{
		// we dont allow reordering if, parent index or child index are out of bounds of element_count
		return 0;
	}

	const void* parent = get_element(&(heap_p->heap_holder), parent_index);
	const void* child  = get_element(&(heap_p->heap_holder), child_index );

	// if the child or parent is NULL, you can not reorder 
	if(child == NULL || parent == NULL)
	{
		return 0;
	}

	int reordering_required = 0;

	switch(heap_p->type)
	{
		case MIN_HEAP :
		{
			// in min heap, parent has to be smaller than or equal to the child
			// we have an issue if parent is greater than child 
			if(heap_p->compare(parent, child) > 0)
			{
				reordering_required = 1;
			}
			break;
		}
		case MAX_HEAP :
		{
			// in max heap, parent has to be greater than or equal to the child
			// we have an issue if parent is smaller than child 
			if(heap_p->compare(parent, child) < 0)
			{
				reordering_required = 1;
			}
			break;
		}
	}

	return reordering_required;
}

static void bubble_up(heap* heap_p, unsigned int index)
{
	// exit at index 0, or thew index is out of range
	while(index != 0 && index < heap_p->element_count)
	{
		unsigned int parent_index = get_parent_index(index);

		// exit, if reordering is not required
		if(!is_reordering_required(heap_p, parent_index, index))
			break;

		inter_change_elements_for_indexes(heap_p, parent_index, index);

		index = parent_index;
	}
}

static void bubble_down(heap* heap_p, unsigned int index)
{
	// we can not bubble down the last node
	while(index < heap_p->element_count)
	{
		unsigned int left_child_index = get_left_child_index(index);
		unsigned int right_child_index = get_right_child_index(index);

		unsigned int new_parent_index = -1;

		int left_reordering_required = is_reordering_required(heap_p, index, left_child_index);
		int right_reordering_required = is_reordering_required(heap_p, index, right_child_index);

		if(left_reordering_required && !right_reordering_required)
			new_parent_index = left_child_index;
		else if(!left_reordering_required && right_reordering_required)
			new_parent_index = right_child_index;
		else if(left_reordering_required && right_reordering_required)
		{
			if(is_reordering_required(heap_p, left_child_index, right_child_index))
				new_parent_index = right_child_index;
			else
				new_parent_index = left_child_index;
		}

		if(new_parent_index == -1)// if no reordering was required, exit the loop
			break;

		inter_change_elements_for_indexes(heap_p, new_parent_index, index);

		index = new_parent_index;
	}
}

void initialize_heap(heap* heap_p, unsigned int initial_size, heap_type type, int (*compare)(const void* data1, const void* data2), void (*heap_index_update_callback)(const void* data, unsigned int heap_index, const void* callback_params), const void* callback_params)
{
	heap_p->type = type;
	heap_p->compare = compare;
	initialize_array(&(heap_p->heap_holder), initial_size);
	heap_p->element_count = 0;
	heap_p->heap_index_update_callback = heap_index_update_callback;
	heap_p->callback_params = callback_params;
}

void initialize_heap_with_allocator(heap* heap_p, unsigned int initial_size, heap_type type, int (*compare)(const void* data1, const void* data2), void (*heap_index_update_callback)(const void* data, unsigned int heap_index, const void* callback_params), const void* callback_params, memory_allocator array_mem_allocator)
{
	heap_p->type = type;
	heap_p->compare = compare;
	initialize_array_with_allocator(&(heap_p->heap_holder), initial_size, array_mem_allocator);
	heap_p->element_count = 0;
	heap_p->heap_index_update_callback = heap_index_update_callback;
	heap_p->callback_params = callback_params;
}

int push_heap(heap* heap_p, const void* data)
{
	// fail, heap is full
	if(is_full_heap(heap_p))
		return 0;

	// insert new element to the heap_holder at the last index + 1 and increment element_count
	set_element(&(heap_p->heap_holder), data, heap_p->element_count++);

	// after insertion we need to make a callback, to notify element index has been updated
	if(heap_p->heap_index_update_callback != NULL)
		heap_p->heap_index_update_callback(data, heap_p->element_count - 1, heap_p->callback_params);

	// bubble up the newly added element at index heap_p->element_count - 1, to its desired place
	bubble_up(heap_p, heap_p->element_count - 1);

	return 1;
}

int pop_heap(heap* heap_p)
{
	// remove the 0th element from the heap
	return remove_from_heap(heap_p, 0);
}

const void* get_top_heap(const heap* heap_p)
{
	// ther is no top element, if there are no elements in the heap
	if(is_empty_heap(heap_p))
		return NULL;

	return (void*)get_element(&(heap_p->heap_holder), 0);
}

int remove_from_heap(heap* heap_p, unsigned int index)
{
	// an element can be removed, only if heap is not empty, and the index provided is within bounds
	if(is_empty_heap(heap_p) || index >= heap_p->element_count)
		return 0;

	// put the indexed element at last and last element to indexed place
	inter_change_elements_for_indexes(heap_p, index, heap_p->element_count - 1);

	// and set the last to NULL, and decrement the element_count of the heap
	set_element(&(heap_p->heap_holder), NULL, --heap_p->element_count);

	// bubble down the element at index 0
	bubble_down(heap_p, index);

	return 1;
}

void heapify_at(heap* heap_p, unsigned int index)
{
	// do not provide out of heap-bound index
	if(index >= heap_p->element_count)
		return;

	// pre-evaluate parent, left child and right child indexes for the corresponding index
	unsigned int parent_index = get_parent_index(index);
	unsigned int left_child_index = get_left_child_index(index);
	unsigned int right_child_index = get_right_child_index(index);

	// if re-ordering is required at the parent side, we bubble up
	if(is_reordering_required(heap_p, parent_index, index))
		bubble_up(heap_p, index);

	// else if the re ordering is required at any of the children's side we bubble down
	else if(is_reordering_required(heap_p, index, left_child_index) || 
			is_reordering_required(heap_p, index, right_child_index))
		bubble_down(heap_p, index);
}

void deinitialize_heap(heap* heap_p)
{
	deinitialize_array(&(heap_p->heap_holder));
}

unsigned int get_total_size_heap(const heap* heap_p)
{
	return heap_p->heap_holder.total_size;
}

unsigned int get_element_count_heap(const heap* heap_p)
{
	return heap_p->element_count;
}

int is_full_heap(const heap* heap_p)
{
	return heap_p->element_count == heap_p->heap_holder.total_size;
}

int is_empty_heap(const heap* heap_p)
{
	return heap_p->element_count == 0;
}

int expand_heap(heap* heap_p)
{
	return expand_array(&(heap_p->heap_holder));
}

int shrink_heap(heap* heap_p)
{
	return shrink_array(&(heap_p->heap_holder), heap_p->element_count);
}

void for_each_in_heap(const heap* heap_p, void (*operation)(void* data, unsigned int heap_index, const void* additional_params), const void* additional_params)
{
	for_each_non_null_in_array(&(heap_p->heap_holder), operation, additional_params);
}

void sprint_heap(dstring* append_str, const heap* heap_p, void (*sprint_element)(dstring* append_str, const void* data, unsigned int tabs), unsigned int tabs)
{
	sprint_chars(append_str, '\t', tabs++);
	switch(heap_p->type)
	{
		case MIN_HEAP :
		{snprintf_dstring(append_str, "heap (MIN_HEAP) :\n");break;}
		case MAX_HEAP :
		{snprintf_dstring(append_str, "heap (MAX_HEAP) :\n");break;}
	}

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "element_count : %u\n", heap_p->element_count);

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "heap_holder : \n");
	sprint_array(append_str, &(heap_p->heap_holder), sprint_element, tabs + 1);
	snprintf_dstring(append_str, "\n");

	sprint_chars(append_str, '\t', tabs);
	snprintf_dstring(append_str, "top : ");
	if(get_top_heap(heap_p) != NULL)
		sprint_element(append_str, get_top_heap(heap_p), 0);
	else
		snprintf_dstring(append_str, "NULL");
	snprintf_dstring(append_str, "\n");
}