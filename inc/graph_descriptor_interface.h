#ifndef GRAPH_DESCRIPTOR_INTERFACE_H
#define GRAPH_DESCRIPTOR_INTERFACE_H

// the below struct is a collection of method 
// that defines/describes implementation of a graph data structure
// and access methods to access it by the graph algorithms of cutlery

// here the context defines 
//  * the query that we are solving
//  * any current query specific variables
//  * or a representation of a query that the current graph traversal is solving

typedef struct node_descriptor node_descriptor;
struct node_descriptor
{
	// get the number of edges going out of this node
	//
	// if a node has 1 bidirectional edge, forming a loop on itself for example
	// 
	//  +--A--+      +--A<--+
	//  |     |  OR  |      |   then the edge count of A is still 2,
	//  +-----+      +------+   although I can not exit A at all
	unsigned int get_outgoing_edge_count(void* node, void* context);

	// get an edge that is going out of this node
	// here the edge_index is always lesser than the result of get_outgoing_edge_count
	// it varies from 0 to get_outgoing_edge_count() - 1
	void* get_outgoing_edge(void* node, void* context, unsigned int edge_index);

	// to mark a given node as visited
	void mark_visited(void* node, void* context);

	// to check if a given node has been visited ( 1 if visited, 0 if not visited )
	int is_visited(void* node, void* context);
};

#define DIRECTED   1
#define UNDIRECTED 2

typedef struct edge_descriptor edge_descriptor;
struct edge_descriptor
{
	// an edge type can be DIRECTED or UNDIRECTED
	unsigned int get_edge_type(void* edge, void* context);

	// in an UNDIRECTED edge, node_index can be 0 or 1 => representing 2 destinations for an edge
	// while in a DIRECTED edge, node_index can be 0 only => A directed edge can only lead you to the 1 only next node
	void* get_destination_node(void* edge, void* context, unsigned int node_index);

	// this is the cost incurred while traversing this particular edge
	int get_traversal_cost(void* edge, void* context);

	// to mark a given edge as traversed
	void mark_traversed(void* edge, void* context);

	// to check if a given edge has been traversed ( 1 if traversed, 0 if not traversed )
	int is_traversed(void* edge, void* context);
};

typedef struct graph_descriptor graph_descriptor;
struct graph_descriptor
{
	// returns root node of this graph, all algorithms start from here, unless a separate root has been provided
	void* get_root(void* graph, void* context);

	// get total number of nodes in the given graph
	unsigned int get_node_count(void* graph, void* context);

	// get total number of edges in the given graph
	unsigned int get_edge_count(void* graph, void* context);

	node_descriptor node_descriptor_funcs;

	edge_descriptor edge_descriptor_funcs;
};

#endif