#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include<arraylist.h>

#define TOTAL_OPERATIONS_SUPPORTED 8

typedef enum al_op al_op;
enum al_op
{
	PUSH_FRONT = 0,
	PUSH_BACK,
	POP_FRONT,
	POP_BACK,
	GET_FRONT,
	GET_BACK,
	GET_NTH_FRONT,
	GET_NTH_BACK
};

#define INITIAL_TOTAL_SIZE 3
#define NUMBER_OF_OPERATIONS 50
#define ELEMENT_POOL_SIZE 50

int element_pool[ELEMENT_POOL_SIZE];

void operate_on_arraylist(arraylist* al, al_op op)
{
	switch(op)
	{
		case PUSH_FRONT :
		{
			int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("PUSH_FRONT : %d :: %d\n", *data, push_front(al, data));
			break;
		}

		case PUSH_BACK :
		{
			int* data = element_pool + (((unsigned int)rand()) % ELEMENT_POOL_SIZE);
			printf("PUSH_BACK : %d :: %d\n", *data, push_back(al, data));
			break;
		}

		case POP_FRONT :
		{
			printf("POP_FRONT :: %d\n", pop_front(al));
			break;
		}

		case POP_BACK :
		{
			printf("POP_BACK :: %d\n", pop_back(al));
			break;
		}

		case GET_FRONT :
		{
			const int* data = get_front(al);
			printf("GET_FRONT : %d :: %p\n", ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case GET_BACK :
		{
			const int* data = get_back(al);
			printf("GET_BACK : %d :: %p\n", ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case GET_NTH_FRONT :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 2) * 1.5));
			const int* data = get_nth_from_front(al, index);
			printf("GET %u TH_FRONT : %d :: %p\n", index, ((data != NULL) ? (*data) : -1), data);
			break;
		}

		case GET_NTH_BACK :
		{
			unsigned int index = ((unsigned int)(rand())) % ((unsigned int)((get_element_count_arraylist(al) + 2) * 1.5));
			const int* data = get_nth_from_back(al, index);
			printf("GET %u TH_BACK : %d :: %p\n", index, ((data != NULL) ? (*data) : -1), data);
			break;
		}
	}
}

void print_int(const void* data)
{
	if(data != NULL)
		printf("%d", *((int*)data));
	else
		printf("NULL - INT");
}

int main()
{
	// seed the random number generator
	srand(12);
	// srand(time(0));

	// initializide element pool
	for(int i = 0; i < ELEMENT_POOL_SIZE; i++)
		element_pool[i] = i;

	arraylist alist;
	arraylist* al = &alist;
	initialize_arraylist(al, INITIAL_TOTAL_SIZE);

	int num_ops = NUMBER_OF_OPERATIONS;

	int expand_required = 0;

	while(num_ops--)
	{
		operate_on_arraylist(al, (rand() % TOTAL_OPERATIONS_SUPPORTED));
		print_arraylist(al, print_int);
		printf("\n\n");

		expand_required += is_full_arraylist(al);
		if(expand_required >= 2)
		{
			int has_expanded = expand_arraylist(al);
			if(has_expanded)
				expand_required = 0;
			printf("EXPAND ARRAYLIST : %d\n", has_expanded);
			print_arraylist(al, print_int);
			printf("\n\n");
		}

		if(num_ops % 20 == 0)
		{
			printf("SHRINK ARRAYLIST : %d\n", shrink_arraylist(al));
			print_arraylist(al, print_int);
			printf("\n\n");
		}
	}

	deinitialize_arraylist(al);
	return 0;
}