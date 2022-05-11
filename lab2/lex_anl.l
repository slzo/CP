%option noyywrap yylineno
%{
#include <stdio.h>
#include "gram_anl.tab.h"
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
One_OP "+"|"-"|"*"|"/"|"%"|"="|"<"|">"|"!"
Two_OP "&&"|"||"|"=="|"!="|"<="|">="
Identifier [a-zA-Z_][a-zA-Z0-9_]*
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

<INITIAL>{BType} { colnum+=yyleng;
					switch(*yytext) {
						case 'i' :  return( INT );
						case 'v' :  return( VOID );
					}
				}
<INITIAL>{If} { colnum+=yyleng;
					switch(*yytext) {
						case 'i' :  return( IF );
						case 'e' :  return( ELSE );
					}
				}
<INITIAL>main { colnum+=yyleng; return (MAIN); }
<INITIAL>return { colnum+=yyleng; return (RETURN); }
<INITIAL>const { colnum+=yyleng; return (CONST); }

<INITIAL>{While} { colnum+=yyleng;
					switch(*yytext) {
						case 'w' :  return( WHILE );
						case 'b' :  return( BREAK );
						case 'c' :  return( CONTINUE );
					}
				}
<INITIAL>{One_OP} {	colnum+=yyleng;
					return *yytext;
				}
<INITIAL>{Two_OP} {colnum+=yyleng;
					switch(*yytext) {
						case '&' :  return( AND );
						case '|' :  return( OR );
						case '=' :  return( EQ );
						case '!' :  return( NE );
						case '<' :  return( LE );
						case '>' :  return( GE );
					}
				}
<INITIAL>{Number} { yylval.intval = atoi(yytext);
					colnum+=yyleng;
					return Number;
					}
<INITIAL>{Border} {	colnum+=yyleng;
					return *yytext;
				}

<INITIAL>{Identifier} { yylval.strval = yytext;
						colnum+=yyleng;
						return ( ID );
						}

<INITIAL>{Other} { colnum+=yyleng; }
<Annotation>. { colnum++; }
<INITIAL>. {  colnum++; }
%%
