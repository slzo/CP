#include <stdio.h>
#include <string.h>
#include <malloc.h>
typedef struct Node {
	char type[15];
	char id[20];
	int number;
	struct Node* child[5];
	struct Node* p;
	int c_n; //number of child
	int nth; //the order number of current node
} Node;

Node* Make1(char*type);
Node* Make2(int n);
Node* Make3(char*id);
void Insert(Node*p, Node*c);
int Ifnode(Node*p, int th);
void DFS(Node*p, int n);
void Creat(Node*p);
