#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include<stdio.h>

typedef struct linkedlist linkedlist;
struct linkedlist;

typedef struct llnode llnode;
struct llnode
{
	// the previous and next node in the linked list
	llnode* next;
	llnode* prev;

	// the pointer to the linkedlist that this node belongs to
	linkedlist* belongs_to_ll;
};

struct linkedlist
{
	// head->next->...->next = tail
	// always head->prev = NULL
	llnode* head;

	// tail->prev->...->prev = head
	// always tail->next = NULL
	llnode* tail;

	// number of nodes in the linkedlist
	// this is how we reach node addresses from provided user's structure data addresses
	unsigned int node_count;

	// defines the address of the data, with respect to the linkedlist node
	unsigned int node_offset;
};

// initializes to a new linked list
void initialize_linkedlist(linkedlist* ll, unsigned int node_offset);

// always initialize your linkedlist node before using it
void initialize_llnode(llnode* node_p);

// simply gets head node data
const void* get_head(linkedlist* ll);

// simply gets tail node data
const void* get_tail(linkedlist* ll);

// inserts will return 0, and fail if llnode of data is not a new node or if node iexists in this or any other linkedlist

// inserts a new head element, returns 0 if it fails
int insert_head(linkedlist* ll, const void* data);

// inserts a new tail element, returns 0 if it fails
int insert_tail(linkedlist* ll, const void* data);

// inserts the new data in linkedlist before data_xist, returns 0 if it fails
int insert_before(linkedlist* ll, const void* data_xist, const void* data);

// inserts the new data in linkedlist before data_xist, returns 0 if it fails
int insert_after(linkedlist* ll, const void* data_xist, const void* data);

// removes will return 0, and fail if llnode of data is a new node or if llnode does not exist in this linkedlist

// removes the head, now the new head is next of the previous head
// returns 0 if it fails
int remove_head(linkedlist* ll);

// removes the tail, now the new tail is prev of the previous head
// returns 0 if it fails
int remove_tail(linkedlist* ll);

// remove the given element from the linked list
// returns 0 if it fails
int remove_from_list(linkedlist*ll, const void* data);

// get pointer to nth element from head by doing next next
const void* get_nth_from_head(linkedlist* ll, unsigned int n);

// get pointer to nth element from tail by doing next next
const void* get_nth_from_tail(linkedlist* ll, unsigned int n);

// get the data from the linkedlist, that equals data, based on the comparator provided
// in the compare function the first parameter is the data from the linkedlist,
// while the second parameter is the data that has been provided by you
// it will return the pointer to the linkedlist data that compares equal (i.e. compare function returns 0)
const void* find_equals_in_list(const linkedlist* ll, const void* data, int (*compare)(const void* ll_data, const void* data));

// check if a given data exists in the linkedlist
// it used belongs_to_list attribute of the node
int exists_in_list(const linkedlist* ll, const void* data);

int is_linkedlist_empty(const linkedlist* ll);

// perform operation on all the elements of the linked list
void for_each_in_list(const linkedlist* ll, void (*operation)(const void* data_p, const void* additional_params), const void* additional_params);

// prints complete linked list
void print_linkedlist(linkedlist* ll, void (*print_element)(const void* data_p));

#endif