#ifndef BUCKET_H
#define BUCKET_H

#include<stdio.h>
#include<stdlib.h>

typedef struct bucket bucket;
struct bucket
{
	const void* data;
	union
	{
		const void* key;
		const void* value;
	};
};

// initialize_bucket
void initialize_bucket(bucket* bucket_p, const void* key, const void* value);

// print the bucket
void print_bucket(const bucket* bucket_p, void (*print_key)(const void* key), void (*print_value)(const void* value));

// compare bucket function
// since a bucket is unique by its key
// we compare buckets, by their keys
int bucket_compare(const bucket* bucket_p1, const bucket* bucket_p2, int (*key_compare)(const void* key_1, const void* key_2));

#endif