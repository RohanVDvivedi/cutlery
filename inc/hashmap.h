#ifndef HASHMAP_H
#define HASHMAP_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct bucket bucket;
struct bucket
{
	// this is size in bytes for the key
	unsigned long long int size_of_key;

	// pointer to the key, without storing key we cant effectively get exact data
	// considering our hash function as deterministic but not unique
	void* key;

	// this is the size in bytes for the data value
	unsigned long long int size_of_value;

	// pointer to the data, that is bucket holds
	void* value;

	// for collision handling mechanism 
	bucket* next_bucket;
};

// hashmap manages its own data
// if hashmap takes a pointer from you, it will be to copy your data

typedef struct hashmap hashmap;
struct hashmap
{
	// the number of buckets this hashmap holds
	unsigned long long int bucket_count;

	// occupancy is the number of elements in hashmap
	// ocupancy <= 0.7 * bucket_count
	unsigned long long int occupancy;

	// hash function ( <3 my love )
	unsigned long long int (*hash_function)(void* key);

	// pinter to the array of buckets
	bucket** buckets;
};

// build and get hashmap with a fixed bucket count,
// bucket count remains the same unless rehash is called with a new size
hashmap* get_hashmap(unsigned long long int bucket_count, unsigned long long int (*hash_function)(void* key));

// build a bucket with key and value
bucket* get_bucket(void* key, unsigned long long int size_of_key, void* value, unsigned long long int size_of_value);

void put(hashmap* hashmap_p, bucket* bucket_p);

void* get(hashmap* hashmap_p, void* key, unsigned long long int size_of_key);

// returns 1 if the bucket is found and removed from hashmap and deleted
int remove_bucket(hashmap* hashmap_p, void* key, unsigned long long int size_of_key);

void rehash_to_size(hashmap* hashmap_p, unsigned long long int new_bucket_size);

void print_bucket(bucket* bucket_p, void (*print_key)(void* key), void (*print_value)(void* value));

void print_hashmap(hashmap* hashmap_p, void (*print_key)(void* key), void (*print_value)(void* value));

void delete_hashmap(hashmap* hashmap_p);

void delete_bucket(bucket* bucket_p);

#endif