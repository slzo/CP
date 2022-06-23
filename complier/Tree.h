#include <cstdio>
#include <iostream>
#include <string.h>
#include <malloc.h>
using namespace std;

#include <string>
#include <vector>
#include <map>
#include <stack>

// #define MAX_DIM 10
//
// enum valuetype{
// 	NO,
// 	INT_,
// 	VOID_,
// 	ARRAY_,
// 	FUNC_
// };
//
// struct Type{
// 	bool constvar;
// 	valuetype type;
//
// 	unsigned dim, curdim=0;
// 	int dimsize[MAX_DIM];
//
// 	int getsize(){
// 		if(type == INT_)
// 			return 4;
// 		else if(type == ARRAY_) {
// 			int size = 1;
// 			for(int i = 0; i < dim; i++)
// 				size *= dimsize[i];
// 			return size*4;
// 		}
// 		else return 0;
//
// };

typedef struct Node {
	char type[15]; //store the type of node
	char id[20]; //if node is a id store the id
	int number; //if node is number store the value
	struct Node* child[10]; //ptr to children
	struct Node* p; // ptr to parent
	int c_n; //number of child
	int nth; //the order number of current node
} Node;

Node* Make1(char*type); //make node of process and keyword
Node* Make2(int n); //make node of number
Node* Make3(char*id); //make node of id
void Relate(Node*p, Node*c); //Relate node c as the child of p
int IfKey(Node*p, int th); //judge p is a keyword node or not
void DFS(Node*p, int n); // dfs the tree
void Create(Node*p); //create the tree by dfs and output the Tree.dot
