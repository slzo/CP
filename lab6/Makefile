CC := g++
CFLAGS := -Wall -Wextra -std=c++11 -g
TARGET := ./main.out

.PHONY: all clean main run lex yacc test debug link testscope asm asmc nasm example-code out mytest

all: complier

pch.h.gch: pch.h
	g++ -x c++-header -o pch.h.gch -c pch.h

lex:
	flex --noyywrap -o lexer.cpp lexer.l

yacc:
	bison -o parser.cpp --defines=parser.h -v parser.y

main: pch.h.gch
	$(CC) $(CFLAGS) $(shell ls *.cpp) -o $(TARGET)

complier: lex yacc main

clean:
	rm -f lexer.cpp parser.cpp parser.h pch.h.gch $(TARGET) *.o ./bin/*
	rm -f *.output

