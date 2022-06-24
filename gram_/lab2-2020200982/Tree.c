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

void Relate(Node* p, Node* c) {
	p->child[p->c_n++] = c;
	c->p = p;
	return;
}

int IfKey(Node* p, int th) {
	char id[]="ID", num[]="Number", LR[]="{", RR[]="}", GT[]=">", GE[]=">=", LE[]="<=", LS[]="<";
	if(!strcmp(p->type, id) ) {
		fprintf(dot, "<f%d> %s", th, p->id);
		return 1;
	}
	if(!strcmp(p->type, num)) {
		fprintf(dot, "<f%d> %d", th, p->number);
		return 1;
	}
	if(!strcmp(p->type, LR) ) {
		fprintf(dot, "<f%d> \\{", th);
		return 1;
	}
	if(!strcmp(p->type, RR)) {
		fprintf(dot, "<f%d> \\}", th);
		return 1;
	}
	if(!strcmp(p->type, GT)) {
		fprintf(dot, "<f%d> \\>", th);
		return 1;
	}
	if(!strcmp(p->type, GE) ) {
		fprintf(dot, "<f%d> \\>=", th);
		return 1;
	}
	if(!strcmp(p->type, LE)) {
		fprintf(dot, "<f%d> \\<=", th);
		return 1;
	}	if(!strcmp(p->type, LS)) {
		fprintf(dot, "<f%d> \\<", th);
		return 1;
	}
	if(!strcmp(p->type, LR) ) {
		fprintf(dot, "<f%d> \\{", th);
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
	fprintf(dot, "node%d[label=\"",p->nth); //nodex[label="<fi> ....."]
	for(int i = 0; i < p->c_n; i++) { 
		if( !IfKey(p->child[i], i) )
			fprintf(dot, "<f%d> %s", i, p->child[i]->type);
		if(i == p->c_n-1) break;
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
	fprintf(dot, "node0[label=\"<f0>%s\"];\n", p->type);
	DFS(p, 0);
	fprintf(dot,"}\n");
	fclose(dot);
	return;
}
