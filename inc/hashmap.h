#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct bucket bucket;
struct bucket
{
	// pointer to the key, for this bucket
	const void* key;

	// pointer to the data, that is bucket holds
	const void* value;

	// for collision handling mechanism 
	bucket* next_bucket;
};

typedef struct hashmap hashmap;
struct hashmap
{
	// the number of buckets this hashmap holds
	unsigned long long int bucket_count;

	// hash function ( <3 my love )
	unsigned long long int (*hash_function)(const void* key);

	// compare keys and returns 0 if they are same, else non-zero
	int (*key_compare)(const void* key1, const void* key2);

	// pinter to the array of buckets
	bucket** buckets;
};

// build and get hashmap with a fixed bucket count,
// bucket count remains the same unless rehash is called with a new size
hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(const void* key), int (*key_compare)(const void* key1, const void* key2));

// place the bucket in the hashmap
void put_entry(hashmap* hashmap_p, const void* key, const void* value);

// get the value from the hashmap, stored at a particular key
const void* get_value(const hashmap* hashmap_p, const void* key);

// returns 1 if the bucket is found and removed from hashmap and deleted
int remove_bucket(hashmap* hashmap_p, const void* key);

// the following function rehashes the hashmap pointed by hashmap_p, to a new size (probably larger)
// used to expand hashmap once the load factor is greater than 0.7 
void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size);

// prints individual bucket
void print_bucket(const bucket* bucket_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

// print complete hashmap
void print_hashmap(const hashmap* hashmap_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

// deletes all the data allocated by the hashmap and the hashmap itself
void delete_hashmap(hashmap* hashmap_p);

#endif