#ifndef ARRAY_H
#define ARRAY_H

#include<cutlery_stds.h>
#include<memory_allocator_interface.h>

typedef struct array array;
struct array
{
	// we maintain array of pointers each of which point to actual data
	const void** data_p_p;

	// initial_size = the size with which the array was requested from using the initialize_array or get_array function
	// The array is not allowed to shrink below its initial size
	unsigned int initial_size;

	// this many elements can be accomodated in array, without expanding
	// expanding which involves copying pointers, and
	// requesting larger pointer array size than previously we had
	// this could get expensive and is non productive computation
	unsigned int total_size;

	// new_total_size of data_p_p = old_total_size of data_p_p * increment_factor + increment_offset
};

// initializes and gives necessary memory to manage internal element contents
void initialize_array(array* array_p, unsigned int initial_size);

// returns pointer to the data at index = index
const void* get_element(const array* array_p, unsigned int index);

// returns 1 for success, and fails with a return of 0 for index out of bounds
int set_element(array* array_p, const void* data_p, unsigned int index);

// swap elements of array at given indexes
void swap_elements(array* array_p, unsigned int i1, unsigned int i2);

// frees all the data being held by the array, this function, does not release memory of the actual array structure, only the memory of the components
// the same array can be reused by calling initialize_array function, after it is deinitialized
void deinitialize_array(array* array_p);

// it expands array
// it returns 1, for success if the array container was successfull expanded,
// else this function may fail with 0, to indicate a failure, fron the memory allocator
int expand_array(array* array_p);

// shrinks the array, if the array is considerably larger than the minimum size ( = end_index - start_index + 1) that was required
// element at the start_index comes to 0, and element at end_index comes to end_index - start_index
// it returns 1 if the array was shrunk
// note : The array is not allowed to shrink below its initial size,
// it shrinks only if the new_total_size is greater than or equal to the initial size
int shrink_array(array* array_p, unsigned int start_index, unsigned int end_index);

// perform operation on all the elements of the array, the method operation takes in 2 params, the data_p to operation on and its index
void for_each_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);
//also ->
void for_each_non_null_in_array(const array* array_p, void (*operation)(void* data_p, unsigned int index, const void* additional_params), const void* additional_params);

// prints the array
void print_array(const array* array_p, void (*print_element)(const void* data_p));

#include<search_sort_array.h>

#endif