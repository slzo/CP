all: myanl
myanl: y.tab.c lex.yy.c Tree.c
	gcc y.tab.c lex.yy.c Tree.c -o myanl

y.tab.c y.tab.h: gram_anl.y
	yacc -d gram_anl.y

lex.yy.c: lex_anl.l
	flex lex_anl.l
.PYHONY: clean
clean:
	-@ rm y.tab.c  y.tab.h lex.yy.c
