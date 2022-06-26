all: complier
complier: lexer.cpp parser.cpp tree.cpp
	g++ -g lexer.cpp parser.cpp tree.cpp -o complier

parser.cpp parser.h: parser.y
	bison -o parser.cpp --defines=parser.h -v parser.y

lexer.cpp: lexer.l
	flex --noyywrap -o lexer.cpp lexer.l

.PHONY:  clean run

run:
	./complier < data/1.sy > data/1.s
	./complier < data/2.sy > data/2.s
	./complier < data/3.sy > data/3.s
	./complier < data/4.sy > data/4.s
clean:
	rm -f lexer.cpp parser.cpp parser.h parser.output complier data/*.s

