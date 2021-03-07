#include<bst.h>

#include<nonbalancing_bst.h>
#include<avl_bst.h>
#include<redblack_bst.h>
#include<bst_util.h>

#include<stdio.h>

void initialize_bst(bst* bst_p, bsttype type, unsigned int node_offset, int (*compare)(const void* data1, const void* data2))
{
	bst_p->type = type;
	bst_p->compare = compare;
	bst_p->node_offset = node_offset;
	bst_p->root = NULL;
}

void initialize_bstnode(bstnode* node_p)
{
	node_p->parent = NULL;
	node_p->left = NULL;
	node_p->right = NULL;
}

int is_new_bstnode(const bst* bst_p, const bstnode* node_p)
{
	return ((node_p->parent == NULL) && (node_p->left == NULL) && (node_p->right == NULL) 
		&& (bst_p->root != node_p));
}


#define is_greater(compare_A_with_B) 	(compare_A_with_B > 0)
#define is_lesser(compare_A_with_B)		(compare_A_with_B < 0)
#define is_equal(compare_A_with_B)		(compare_A_with_B == 0)

// searches for bstnode that holds data equal to the given data
static bstnode* find_node(const bst* bst_p, const void* data)
{
	bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = bst_p->compare(data, get_data(node_p));
		if(is_equal(compared_data_with_current_node))
			return node_p;
		else if(is_lesser(compared_data_with_current_node))
			node_p = node_p->left;
		else if(is_greater(compared_data_with_current_node))
			node_p = node_p->right;
	}
	return NULL;
}

static bstnode* find_node_preceding_or_equals(const bst* bst_p, const void* data)
{
	bstnode* result = NULL;
	bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = bst_p->compare(data, get_data(node_p));
		if(is_equal(compared_data_with_current_node))
			return node_p;
		else if(is_lesser(compared_data_with_current_node))
			node_p = node_p->left;
		else if(is_greater(compared_data_with_current_node))
		{
			result = node_p;
			node_p = node_p->right;
		}
	}
	return result;
}

static bstnode* find_node_succeeding_or_equals(const bst* bst_p, const void* data)
{
	bstnode* result = NULL;
	bstnode* node_p = bst_p->root;
	while(node_p != NULL)
	{
		int compared_data_with_current_node = bst_p->compare(data, get_data(node_p));
		if(is_equal(compared_data_with_current_node))
			return node_p;
		else if(is_lesser(compared_data_with_current_node))
		{
			result = node_p;
			node_p = node_p->left;
		}
		else if(is_greater(compared_data_with_current_node))
			node_p = node_p->right;
	}
	return result;
}

const void* find_equals_in_bst(const bst* bst_p, const void* data)
{
	bstnode* node_p = find_node(bst_p, data);
	return (node_p != NULL) ? get_data(node_p) : NULL;
}

const void* find_preceding_or_equals(const bst* bst_p, const void* data)
{
	bstnode* node_p = find_node_preceding_or_equals(bst_p, data);
	return (node_p != NULL) ? get_data(node_p) : NULL;
}

const void* find_succeeding_or_equals(const bst* bst_p, const void* data)
{
	const bstnode* node_p = find_node_succeeding_or_equals(bst_p, data);
	return (node_p != NULL) ? get_data(node_p) : NULL;
}

const void* find_smallest(const bst* bst_p)
{
	bstnode* node_p = get_smallest_node_from_node(bst_p->root);
	return (node_p != NULL) ? get_data(node_p) : NULL;
}

const void* find_largest(const bst* bst_p)
{
	bstnode* node_p = get_largest_node_from_node(bst_p->root);
	return (node_p != NULL) ? get_data(node_p) : NULL;
}

static unsigned int find_all_in_range_recursive(const bst* bst_p, const bstnode* node_p, const void* lower_bound, const void* upper_bound, sort_direction sort_dirctn, unsigned int max_result_count, void (*result_accumulator)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return 0;

	unsigned int results_accumulated = 0;

	const void* data_p = get_data(node_p);

	int lower_bound_check = (lower_bound != NULL) ? (bst_p->compare(lower_bound, data_p) <= 0) : 1;
	int upper_bound_check = (upper_bound != NULL) ? (bst_p->compare(upper_bound, data_p) >= 0) : 1;

	switch(sort_dirctn)
	{
		case ASCENDING_ORDERED :
		{
			// node_p is greater than the lower_bound, i.e. left sub tree may have nodes lesser than the lower bound
			if(lower_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->left, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, result_accumulator, additional_params);

			// node_p is in range [lower_bound, upper_bound], consider this node aswell
			if(lower_bound_check && upper_bound_check && (results_accumulated < max_result_count))
			{
				result_accumulator(data_p, additional_params);
				results_accumulated += 1;
			}

			// node_p is lesser than the upper_bound, i.e. right sub tree may have nodes greater than the lower bound
			if(upper_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->right, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, result_accumulator, additional_params);
		
			break;
		}
		case DESCENDING_ORDERED :
		{
			// node_p is lesser than the upper_bound, i.e. right sub tree may have nodes greater than the lower bound
			if(upper_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->right, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, result_accumulator, additional_params);

			// node_p is in range [lower_bound, upper_bound], consider this node aswell
			if(lower_bound_check && upper_bound_check && (results_accumulated < max_result_count))
			{
				result_accumulator(data_p, additional_params);
				results_accumulated += 1;
			}

			// node_p is greater than the lower_bound, i.e. left sub tree may have nodes lesser than the lower bound
			if(lower_bound_check && (results_accumulated < max_result_count))
				results_accumulated += find_all_in_range_recursive(bst_p, node_p->left, lower_bound, upper_bound, sort_dirctn, max_result_count - results_accumulated, result_accumulator, additional_params);

			break;
		}
	}

	return results_accumulated;
}

unsigned int find_all_in_range(const bst* bst_p, const void* lower_bound, const void* upper_bound, sort_direction sort_dirctn, unsigned int max_result_count, void (*result_accumulator)(const void* data, const void* additional_params), const void* additional_params)
{
	// errror in providing values 
	// if both of the lower and upper bounds are provided, then lower_bound must not be greater than upper bound
	if(((lower_bound != NULL) && (upper_bound != NULL)) && (bst_p->compare(lower_bound, upper_bound) > 0))
		return 0;
	return find_all_in_range_recursive(bst_p, bst_p->root, lower_bound, upper_bound, sort_dirctn, max_result_count, result_accumulator, additional_params);
}

int insert_in_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data);
	
	if(!is_new_bstnode(bst_p, node_p))	// insert only a new node
		return 0;

	// if the root of the tree is NULL, i.e. the tree is empty, add a new root to the tree
	if(is_balancedbst_empty(bst_p))
	{
		bst_p->root = node_p;
		node_p->parent = NULL;

		// if avl => 1 : number of nodes to reach NULL node, if red-black => root is always black, (black = 1)
		node_p->node_property = (bst_p->type == NON_SELF_BALANCING) ? 0 : 1;
	}
	else
	{
		// else insert node as per the balancing that this tree uses
		switch(bst_p->type)
		{
			case NON_SELF_BALANCING:
			{
				insert_node_in_non_self_balancing_tree(bst_p, node_p);
				break;
			}
			case RED_BLACK_TREE :
			{
				insert_node_in_red_black_tree(bst_p, node_p);
				break;
			}
			case AVL_TREE :
			{
				insert_node_in_avl_tree(bst_p, node_p);
				break;
			}
		}
	}

	return 1;
}

int remove_from_bst(bst* bst_p, const void* data)
{
	bstnode* node_p = get_node(data);

	if(is_new_bstnode(bst_p, node_p))	// for attempting to remove the node, it must be present in bst, i.e. not a new node
		return 0;

	switch(bst_p->type)
	{
		case NON_SELF_BALANCING:
		{
			remove_node_from_non_self_balancing_tree(bst_p, node_p);
			break;
		}
		case RED_BLACK_TREE :
		{
			remove_node_from_red_black_tree(bst_p, node_p);
			break;
		}
		case AVL_TREE :
		{
			remove_node_from_avl_tree(bst_p, node_p);
			break;
		}
	}

	// NULL all references of the removed node
	initialize_bstnode(node_p);	// you must reinitialize the node before final removal

	return 1;
}

static void for_each_node_pre_order(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	operation(get_data(node_p), additional_params);
	for_each_node_pre_order(bst_p, node_p->left, operation, additional_params);
	for_each_node_pre_order(bst_p, node_p->right, operation, additional_params);
}

static void for_each_node_in_order(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	for_each_node_in_order(bst_p, node_p->left, operation, additional_params);
	operation(get_data(node_p), additional_params);
	for_each_node_in_order(bst_p, node_p->right, operation, additional_params);
}

static void for_each_node_post_order(const bst* bst_p, const bstnode* node_p, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	if(node_p == NULL)
		return;
	for_each_node_post_order(bst_p, node_p->left, operation, additional_params);
	for_each_node_post_order(bst_p, node_p->right, operation, additional_params);
	operation(get_data(node_p), additional_params);
}

void for_each_in_bst(const bst* bst_p, bsttraversal traversal, void (*operation)(const void* data, const void* additional_params), const void* additional_params)
{
	switch(traversal)
	{
		case PRE_ORDER :
			for_each_node_pre_order(bst_p, bst_p->root, operation, additional_params); return;
		case IN_ORDER :
			for_each_node_in_order(bst_p, bst_p->root, operation, additional_params); return;
		case POST_ORDER :
			for_each_node_post_order(bst_p, bst_p->root, operation, additional_params); return;
	}
}

static void print_node(const bst* bst_p, const bstnode* node_p, void (*print_element)(const void* data))
{
	if(node_p != NULL)
	{
		if(is_root_node(node_p))
			printf("  ROOT  ");
		else if(is_leaf_node(node_p))
			printf("  LEAF  ");
		else if(is_internal_node(node_p))
			printf("  INTER ");

		printf("\t\t[%p]", node_p);
		print_element(get_data(node_p));
		printf("(%d)\n", node_p->node_property);

		if( !is_root_node(node_p) )
		{
			if(is_left_of_its_parent(node_p))
				printf("    LEFT  of");
			else if(is_right_of_its_parent(node_p))
				printf("    RIGHT of");
			printf(" \t\t[%p]", node_p->parent);
			print_element(get_data(node_p->parent));
			printf("\n");
		}

		if( (!is_leaf_node(node_p)) )
		{
			if(node_p->left != NULL)
			{
				printf("    has LEFT  \t\t[%p]", node_p->left);
				print_element(get_data(node_p->left));
				printf("\n");
			}
			if(node_p->right != NULL)
			{
				printf("    has RIGHT \t\t[%p]", node_p->right);
				print_element(get_data(node_p->right));
				printf("\n");
			}
		}
		printf("\n");
	}
}

static void print_tree(const bst* bst_p, const bstnode* node_p, void (*print_element)(const void* data))
{
	if(node_p != NULL)
		print_tree(bst_p, node_p->left, print_element);
	print_node(bst_p, node_p, print_element);
	printf("\n");
	if(node_p != NULL)
		print_tree(bst_p, node_p->right, print_element);
}

void print_bst(const bst* bst_p, void (*print_element)(const void* data))
{
	printf("BINARY SEARCH TREE\n");
	switch(bst_p->type)
	{
		case NON_SELF_BALANCING :
		{
			printf("NON_SELF_BALANCING\n");
			break;
		}
		case AVL_TREE :
		{
			printf("AVL_TREE\n");
			break;
		}
		case RED_BLACK_TREE :
		{
			printf("RED_BLACK_TREE\n");
			break;
		}
	}
	printf("node_offset : [%u]\n", bst_p->node_offset);
	printf("root : [%p]\n", bst_p->root);

	print_tree(bst_p, bst_p->root, print_element);
	printf("--\n\n\n");
}

void print_bstnode_debug(bstnode* node_p)
{
	printf("DEBUG -> self[%p] ", node_p);
	if(node_p != NULL)
		printf("parent[%p], left[%p], right[%p], node_property = %d\n", node_p->parent, node_p->left, node_p->right, node_p->node_property);
	else
		printf("\n");
}