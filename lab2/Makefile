LEX = flex
YYAC = bison
CC = gcc

all: myanl

myanl: gram_anl.tab.o lex_anl.o
	$(CC) -o $@ gram_anl.tab.o lex_anl.o

gram_anl.tab.c gram_anl.tab.h: gram_anl.y
	$(YYAC) -vd gram_anl.y

lex_anl.c: lex_anl.l
	$(LEX) -o $@ $<

lex_anl.o: lex_anl.c gram_anl.tab.h

.PYHONY: clean

clean:
	-@ rm gram_anl.tab.c gram_anl.tab.h lex_anl.c gram_anl.output *.o
