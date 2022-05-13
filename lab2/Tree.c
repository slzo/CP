#include "Tree.h"
FILE* dot;
int thn=0;
Node* Make1(char* type) {
	Node* node=(Node*)malloc(sizeof(Node));
	node->c_n = 0;
	strcpy(node->type, type);
	return node;
}

Node* Make2(int n) {
	Node* node=(Node*)malloc(sizeof(Node));
	node->c_n=0;
	char type[]="Number";
	strcpy(node->type, type);
	node->number = n;
	return node;
}

Node* Make3( char* id) {
	Node* node=(Node*)malloc(sizeof(Node));
	node->c_n=0;
	char type[]="ID";
	strcpy(node->type,type);
	strcpy(node->id, id);
	return node;
}



void Insert(Node* p, Node* c) {
	p->child[p->c_n++] = c;
	c->p = p;
	return;
}

int Ifnode(Node* p, int th) {
	char Key[32][10]={"int","void","if","else","while","break","continue","return","const","&&","||","==","!=","<=",">=","+","-","*","/","%","!",">","<","=","{","}","[","]","(",")",",",";"};
	char id[]="ID", num[]="Number";
	if(!strcmp(p->type, id) ) {
		fprintf(dot, "<f%d> %s", th, p->id);
		return 1;
	}
	if(!strcmp(p->type, num)) {
		fprintf(dot, "<f%d> %d", th, p->number);
		return 1;
	}
	for(int i = 0; i < 32; i++)
		if(!strcmp(p->type, Key[i])){
				fprintf(dot, "<f%d> %s", th, Key[i]);
				return 1;
		}
	return 0;
}

void DFS(Node* p, int n) {
	if(!p->c_n)
		return;
	p->nth=thn++;
	for(int i = 0; i < p->c_n; i++)
		DFS(p->child[i], i);
	printf("%s %d\n", p->type, p->nth);
	fprintf(dot, "node%d[label=\"",p->nth); //nodex[label="<fi> ....."]
	for(int i = 0; i < p->c_n; i++) { 
		if( !Ifnode(p->child[i], i) )
			fprintf(dot, "<f%d> %s", i, p->child[i]->type);
		fprintf(dot, "|");
	}
	fprintf(dot, "\"];\n");
	if(p->p)
		fprintf(dot, "\"node%d\":f%d->\"node%d\";\n",p->p->nth, n, p->nth); //"nodex":fi->"nodey"
}

void Create(Node* p) {
	dot=fopen("Tree.dot","w");
	fprintf(dot, "digraph \" \"{\n"); //digraph " "{
	fprintf(dot, "node [shape = record,height=.1]\n");
	fprintf(dot, "node0[label=\"<f0>%s;\n", p->type);
	DFS(p, 0);
	fprintf(dot,"}\n");
	fclose(dot);
	return;
}
