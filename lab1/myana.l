%{
#include <stdio.h>
#include <stdlib.h>
int rownum = 1, colnum = 1; //记录当前检索位置的行和列号
%}

Alp [a-zA-Z]
Dec [1-9][0-9]*|0
Hex 0[x|X][1-9a-fA-F][0-9a-fA-F]*
Oct 0[1-7][0-7]*
Fl "."[1-9][0-9]*
SN [1-9]{Fl}{0,1}[e|E][+-]*[0-9]+
Bracket "("|"["|"{"|")"|"]"|"}"
Notation ","|";"
BType int|void
If if|else
While while|break|continue
AL "+"|"-"|"*"|"/"|"%"
Logical "&&"|"||"|"!"
Relational "=="|"!="|"<"|">"|"<="|">="
Identifier [a-zA-Z_][a-zA-Z0-9_]*
Key {BType}|{If}|{While}|return|main|const
Opt {AL}|{Logical}|{Relational}|"="
Number {Dec}|{Hex}|{Oct}|{SN}
Border {Bracket}|{Notation}
Annotation1 "//".*\n
NO [@#$:"?\`]|"^"|"."
All {NO}|[a-zA-Z0-9_]
NI [0-9]+{All}+
ILG {NO}+{All}*
Other {NO}|{NI}
%s Annotation
%%
" " { colnum++; }
\n { rownum++; colnum = 1; }
\t { colnum++; }
<INITIAL>"/*" { BEGIN Annotation; colnum+=2; }
<Annotation>"*/" { BEGIN 0; colnum+=2;  }
<INITIAL>{Annotation1} { colnum = 1; rownum++; }
<INITIAL>{Key} { printf("%s: K (%d, %d)\n", yytext, rownum, colnum) ; colnum+=yyleng; }
<INITIAL>{Opt} { printf("%s: O (%d, %d)\n", yytext, rownum, colnum) ; colnum+=yyleng; }
<INITIAL>{Number} { printf("%s: C (%d, %d)\n", yytext, rownum, colnum) ; colnum+=yyleng; }
<INITIAL>{Border} { printf("%s: D (%d, %d)\n", yytext, rownum, colnum) ; colnum+=yyleng; }
<INITIAL>{Identifier} { printf("%s: I (%d, %d)\n", yytext, rownum, colnum) ; colnum+=yyleng; }
<INITIAL>{Other} { printf("%s: T (%d, %d)\n", yytext, rownum, colnum); colnum+=yyleng; }
<Annotation>. { colnum++; }
<INITIAL>. { printf("%s: T (%d, %d,)\n", yytext, rownum, colnum); colnum++; }
%%

int main() {
		yylex();
		return 0;
}
int yywrap(){
		return 1;
}
