#ifndef DSTRING_H
#define DSTRING_H

#include<memory_allocator_interface.h>

typedef struct dstring dstring;
struct dstring
{
	// actual string it does not terminate with '\0'
	char* cstring;

	// number of bytes in the string
	unsigned int bytes_occupied;

	// this are the number of bytes that are allocated
	// i.e you may reference cstring for this many no of bytes until seg fault
	// yet this value is 0, for slize or slices
	// slices are dstrings that point to other cstrings, and have cstring pointing to theinr data
	// a slice could be used to represent a substring of a dstring 
	unsigned int bytes_allocated;
};

// dstring_DUMMY_* macro below do not allocate any memory
// they rather starts pointing to the data that you provided
// deinitializing such a dstring will not and must not cause any error
// so if you plan to hold a reference to even a dstring_DUMMY, 
// please deinitialize it before the end of the scope or refrain from holding a reference to it or passing it to functions that could modify it
#define dstring_DUMMY_DATA(data, data_size) &((const dstring){.cstring = ((char*)(data)), .bytes_occupied = (data_size),    .bytes_allocated = 0})
#define dstring_DUMMY_CSTRING(cstr)         &((const dstring){.cstring = ((char*)(cstr)), .bytes_occupied = strlen((cstr)), .bytes_allocated = 0})

void init_dstring(dstring* str_p, const char* data, unsigned int data_size);
void init_empty_dstring(dstring* str_p, unsigned int init_size);

void make_dstring_empty(dstring* str_p);

int compare_dstring(const dstring* str_p1, const dstring* str_p2);
int case_compare_dstring(const dstring* str_p1, const dstring* str_p2);

// increases the size of dstring by additional_size number of bytes
int expand_dstring(dstring* str_p, unsigned int additional_size);
// shrinks dstring to its bytes_occupied size
int shrink_dstring(dstring* str_p);

// concatenates str_p2 to str_p1
void concatenate_dstring(dstring* str_p1, const dstring* str_p2);

// append_to_dstring_formatted is a snprintf implementation for dstrings
void snprintf_dstring(dstring* str_p, const char* cstr_format, ...);

// append count number of chr char at the end of the dstring str_p
void sprint_chars(dstring* str_p, char chr, unsigned int count);

// marcos for io using dstring
#define printf_dstring(str_p)				(((str_p)->cstring != NULL && (str_p)->bytes_occupied > 0) ? printf("%.*s", (str_p)->bytes_occupied, (str_p)->cstring) : 0)

void toLowercase(dstring* str_p);
void toUppercase(dstring* str_p);

#include<dstring_substring.h>
#include<dstring_diff.h>

void deinit_dstring(dstring* str_p);

// This is the global memory allocator that will be used for contents 
// of every dstring that ever gets created/modified
extern memory_allocator DSTRING_mem_alloc;

#endif