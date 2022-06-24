%option nounput
%{
#define YYSTYPE TreeNode *
#include "tree.h"
#include "parser.h"
int lineno = 1;

%}

EOL	(\r\n|\r|\n)
OCTINT 0[1-7][0-7]*
DECINT [1-9][0-9]*|0
HEXINT 0[Xx][1-9a-fA-F][0-9a-fA-F]*
IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

LINECOMMENT "//".*\n
%x BLOCKCOMMENT
%%
"/*" {BEGIN BLOCKCOMMENT;}
<BLOCKCOMMENT>. {}
<BLOCKCOMMENT>\n {lineno++;}
<BLOCKCOMMENT>"*/" {BEGIN 0;}
{LINECOMMENT}  {lineno++;}

"int" return INT;
"void" return VOID;


"(" return LPAREN;
")" return RPAREN;
"[" return LBRACKET;
"]" return RBRACKET;
"{" return LBRACE;
"}" return RBRACE;
"," return COMMA;
";" return SEMICOLON;

"==" return EQ;
">=" return GE;
"<=" return LE;
"!=" return NEQ;
"=" return ASSIGN;
">" return GRA;
"<" return LES;
"+" return PLUS;
"-" return MINUS;
"*" return MUL;
"/" return DIV;
"%" return MOD;
"&&" return AND;
"||" return OR;
"!" return NOT;

"const" return CONST;
"if" return IF_;
"else" return ELSE;
"while" return WHILE_;
"break" return BREAK;
"continue" return CONTINUE;
"return" return RETURN;

"printf" {
    nodePrintf->var_name = "printf";
    nodePrintf->var_scope = "1";
    yylval = nodePrintf;
    return IDENTIFIER;
};
"scanf" {
    nodeScanf->var_name = "scanf";
    nodeScanf->var_scope = "1";
    yylval = nodeScanf;
    return IDENTIFIER;
};

{OCTINT} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = strtol(yytext, NULL, 8);
    if(node->int_val)
        node->b_val = true;
    yylval = node;
    return INTEGER;
}

{DECINT} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = atoi(yytext);
    if(node->int_val)
        node->b_val = true;
    yylval = node;
    return INTEGER;
}

{HEXINT} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = strtol(yytext, NULL, 16);
    if(node->int_val)
        node->b_val = true;
    yylval = node;
    return INTEGER;
}

{IDENTIFIER} {
    TreeNode* node = new TreeNode(lineno, NODE_VAR);
    node->var_name = string(yytext);
    yylval = node;
    return IDENTIFIER;
}

" " {}
\t {}
{EOL} {lineno++;}

. {
    cerr << "[line "<< lineno <<" ] unknown character:" << yytext << endl;
}
%%
