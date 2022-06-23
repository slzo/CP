#include <stdio.h>

void enter(char *name, char* type, int offset);

void lookup(char *name);
void gen();

int* makelist(int i);
void merge(int *p1, int *p2);
void backpatch(int *p, int i);
