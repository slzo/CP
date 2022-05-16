%{
#include <stdio.h>
#include "y.tab.h"
#include "Tree.h"
int rownum = 1, colnum = 1; //记录当前检索位置的行和列号
%}

Alp [a-zA-Z]
Dec [1-9][0-9]*|0
Hex 0[x|X][1-9a-fA-F][0-9a-fA-F]*
Oct 0[1-7][0-7]*
Fl "."[1-9][0-9]*
Bracket "("|"["|"{"|")"|"]"|"}"
Notation ","|";"
BType int|void
If if|else
While while|break|continue
One_OP "+"|"-"|"*"|"/"|"%"|"="|"<"|">"|"!"
Two_OP "&&"|"||"|"=="|"!="|"<="|">="
Identifier [a-zA-Z_][a-zA-Z0-9_]*
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
					yylval.node = Make1(yytext);
					switch(*yytext) {
						case 'i' :  return( INT );
						case 'v' :  return( VOID );
					}
				}
<INITIAL>{If} { colnum+=yyleng;
				yylval.node = Make1(yytext);
				switch(*yytext) {
					case 'i' :  return( IF );
					case 'e' :  return( ELSE );
				}
			}
<INITIAL>return { colnum+=yyleng; yylval.node = Make1(yytext); return (RETURN); }
<INITIAL>const { colnum+=yyleng; yylval.node= Make1(yytext); return (CONST); }

<INITIAL>{While} { colnum+=yyleng;
					yylval.node = Make1(yytext);
					switch(*yytext) {
						case 'w' :  return( WHILE );
						case 'b' :  return( BREAK );
						case 'c' :  return( CONTINUE );
					}
				}
<INITIAL>{One_OP} {	colnum+=yyleng;
					yylval.node = Make1(yytext);
					return *yytext;
				}
<INITIAL>{Two_OP} {colnum+=yyleng;
					yylval.node = Make1(yytext);
					switch(*yytext) {
						case '&' :  return( AND );
						case '|' :  return( OR );
						case '=' :  return( EQ );
						case '!' :  return( NE );
						case '<' :  return( LE );
						case '>' :  return( GE );
					}
				}
<INITIAL>{Dec} { int number = atoi(yytext);
					colnum+=yyleng;
					yylval.node = Make2(number);
					return Number;
					}
<INITIAL>{Hex} { int number;
					for(int i = 2; i < strlen(yytext); i++) {
						number*=16;
						if(yytext[i]>='0' && yytext[i] <= '9')
							number += (yytext[i]-'0');
						if(yytext[i]>='a' && yytext[i] <= 'f')
							number += (yytext[i]-'a'+10);
						if(yytext[i]>='A' && yytext[i] <= 'F')
							number += (yytext[i]-'A'+10);
					}
					colnum+=yyleng;
					yylval.node = Make2(number);
					return Number;
					}
<INITIAL>{Oct} { int number = 0;
					for(int i = 1; i < strlen(yytext); i++) {
						number*=8;
						number += (yytext[i]-'0');
					}
					colnum+=yyleng;
					yylval.node = Make2(number);
					return Number;
					}

<INITIAL>{Border} {	colnum+=yyleng;
					yylval.node = Make1(yytext);
					return *yytext;
				}

<INITIAL>{Identifier} { colnum+=yyleng;
						yylval.node = Make3(yytext);
						return ( ID );
						}

<INITIAL>{Other} { colnum+=yyleng; }
<Annotation>. { colnum++; }
<INITIAL>. {  colnum++; }
%%
int yywrap(){
	return 1;
}
