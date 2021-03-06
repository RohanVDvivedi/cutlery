#include<cutlery_stds.h>

#include<stdio.h>

static const unsigned int int_size = sizeof(int); // we assume that int is always 2^n bytes in size, i.e. 1,2,4,8 etc
static const unsigned int int_bits_size = sizeof(int) * 8;
static const unsigned int int_alignment_bit_mask = -((int)sizeof(int));	// 0b 111...111 000...000 where 0s represent alignment

// on a 32 bit int system
// int_size = 4
// int_bits_size = 32
// int_alignment_bis_mask = 0xfffffffc (i.e. 2's complement of 4 OR binary representation of -4)

void memory_move(void* dest_start, const void* src_start, unsigned int size)
{
	// if they are the same memory locations, or if the copy size if 0, skip the copy operation
	if(src_start == dest_start || size == 0)
		return;

	// compute the last src and dest byte address that needs to be copied
	const void* src_last = src_start + (size - 1);
	void* dest_last = dest_start + (size - 1);

	// decide to make a forward pass or backward pass 
	// based on whether the pass could corrupt the src memory address before we could copy it

	// make forward pass
	if( dest_start < src_start )
	{
		// intialize our iterators for forward copy
		const char* src = src_start;
		char* dest = dest_start;

		// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
		if( (size >= 3 * int_size) &&
			(
				(((long int)src_start) & ~int_alignment_bit_mask) == (((long int)dest_start) & ~int_alignment_bit_mask)
			)
		)
		{
			// perform a byte-by-byte copy until the addresses are int aligned
			while( ( ((long int)src) & ~int_alignment_bit_mask ) )
				*(dest++) = *(src++);

			// perform an int-by-int transfer in this scope
			// dest_int and src_int must not leave this scope
			// they must equal the dest and src, right before and right after this scope
			{
				const int* src_int = (int*)src;
				int* dest_int = (int*)dest;

				// additonal bytes that you might have to copy after completing the int copy
				unsigned long int additional_bytes = ((unsigned long int)(src_last + 1)) & ~int_alignment_bit_mask;

				// this is the address of the last byte that must be copied under the int-by-int copy loop
				const int* src_last_int = src_last - additional_bytes;

				// int-by-int copy loop
				while( src_int <= src_last_int )
					*(dest_int++) = *(src_int++);

				src = (char*)src_int;
				dest = (char*)dest_int;
			}
		}

		// finish up remaining with an old fashioned byte-by-byte copy loop
		while( src <= ((char*)(src_last)) )
			*(dest++) = *(src++);
	}
	// else make backward pass
	else
	{
		// intialize our iterators for backward copy
		const char* src = src_last;
		char* dest = dest_last;

		// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
		if( (size >= 3 * int_size) &&
			(
				(((long int)src_start) & ~int_alignment_bit_mask) == (((long int)dest_start) & ~int_alignment_bit_mask)
			)
		)
		{
			// perform a byte-by-byte copy until the addresses are int - 1 aligned
			while( ( ((long int)src) & ~int_alignment_bit_mask ) != ( ~int_alignment_bit_mask ) )
				*(dest--) = *(src--);

			// point src and dest to next int that needs to be copied
			src -= (int_size - 1);
			dest -= (int_size - 1);

			// perform an int-by-int transfer in this scope
			// dest_int and src_int must not leave this scope
			// they must equal the dest and src, right before and right after this scope
			{
				const int* src_int = (int*)src;
				int* dest_int = (int*)dest;

				// additonal bytes that you might have to copy after completing the int copy
				unsigned long int additional_bytes = (int_size - (((unsigned long int)(src_start)) & ~int_alignment_bit_mask)) & ~int_alignment_bit_mask;

				// this is the address of the last byte that must be copied under the int-by-int copy loop
				const int* src_start_int = src_start + additional_bytes;

				// int-by-int copy loop
				while( src_int >= src_start_int )
					*(dest_int--) = *(src_int--);

				src = (char*)src_int;
				dest = (char*)dest_int;
			}

			src += (int_size - 1);
			dest += (int_size - 1);
		}

		// finish up remaining with an old fashioned byte-by-byte copy loop
		while( src >= ((char*)(src_start)) )
			*(dest--) = *(src--);
	}
}

void memory_set(void* dest_start, char byte_value, unsigned int size)
{
	// if the copy size is zero, skip the copy operation
	if(size == 0)
		return;

	// compute the last dest byte address that needs to be copied
	void* dest_last = dest_start + (size - 1);

	// intialize our iterators for the copy operation
	char* dest = dest_start;

	// check if int copy is possible, and requires atleast 3 iterations (atleast 2 int copy iterations)
	if(size >= 3 * int_size)
	{
		// perform a byte-by-byte copy until the address is int aligned
		while( ( ((long int)dest) & ~int_alignment_bit_mask ) )
			*(dest++) = byte_value;

		// perform an int-by-int transfer in this scope
		// dest_int must not leave this scope
		// it must equal the dest, right before and right after this scope
		{
			int* dest_int = (int*)dest;

			// generate the int_value that we could copy to the int locations int-by-int
			int int_value = 0;
			for(unsigned int i = 0; i < int_bits_size; i += 8)
				int_value |= ( (((int)byte_value) & 0xff) << i );

			// additonal bytes that you might have to copy after completing the int copy
			unsigned long int additional_bytes = ((unsigned long int)(dest_last + 1)) & ~int_alignment_bit_mask;

			// this is the address of the last byte that must be copied under the int-by-int copy loop
			const int* dest_last_int = dest_last - additional_bytes;

			// int-by-int copy loop
			while( dest_int <= dest_last_int )
				*(dest_int++) = int_value;

			dest = (char*)dest_int;
		}
	}

	// finish up remaining with an old fashioned byte-by-byte copy
	while( dest <= ((char*)(dest_last)) )
		*(dest++) = byte_value;
}