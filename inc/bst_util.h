#ifndef BST_UTIL_H
#define BST_UTIL_H

/*
*	MACROS TO CHECK IF THE NODE IS NEW NODE <you can only insert a new node, and remove an existing node>
*/

#define initialize_bstnode(node_p)									\
	{node_p->parent = NULL; node_p->left = NULL; node_p->right = NULL; node_p->belongs_to_bst = NULL;}

#define is_new_bstnode(node_p)										\
	((node_p->parent == NULL) && (node_p->left == NULL) && (node_p->right == NULL) && (node_p->belongs_to_bst == NULL))

#define bstnode_exists_in_this_bst(node_p)								\
	(node_p->belongs_to_bst == bst_p)

#define bstnode_exists_in_any_bst(node_p)								\
	(node_p->belongs_to_bst != NULL)


/*
*	MACROS TO CONVERT FROM DATA TO NODE AND NODE TO DATA
*/
#define get_data(node_p) 	(((void*)(node_p)) - bst_p->node_offset)

#define get_node(data_p) 	(((void*)(data_p)) + bst_p->node_offset)

/*
*	MACROS TO DEFINE THE TYPE OF NODE
*/

#define is_leaf_node(node_p)									\
	(node_p->left == NULL && node_p->right == NULL)

#define is_root_node(node_p)									\
	(node_p->parent == NULL)

#define is_internal_node(node_p)								\
	(!(is_leaf_node(node_p) || is_root_node(node_p)))


/*
*	MACROS TO DEFINE SURROUNDINGS OF THE NODE
*/

#define has_only_right_sub_tree(node_p)							\
	(node_p->left == NULL && node_p->right != NULL)

#define has_only_left_sub_tree(node_p)							\
	(node_p->left != NULL && node_p->right == NULL)

#define is_left_of_its_parent(node_p)							\
	((!(is_root_node(node_p))) && (node_p->parent->left == node_p))

#define is_right_of_its_parent(node_p)							\
	((!(is_root_node(node_p))) && (node_p->parent->right == node_p))

/*
*	MARCOS TO CHECK WHETHER TREE IS EMPTY
*/

#define is_balancedbst_empty(balancedbst_p)						\
	(balancedbst_p->root == NULL)

#endif