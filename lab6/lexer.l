%option nounput
%{
#define YYSTYPE TreeNode *
#include "tree.h"
#include "parser.h"
int lineno = 1;

%}

BLOCKCOMMENT_BEGIN "/*"
BLOCKCOMMENT_ELE .
BLOCKCOMMENT_LINE \n
BLOCKCOMMENT_END "*/"
%x BLOCKCOMMENT
LINECOMMENT \/\/[^\n]*
EOL	(\r\n|\r|\n)
WHILTESPACE [[:blank:]]

OCTINT 0[0-7]+
DECINT [0-9]+
HEXINT 0[Xx][0-9a-fA-F]+

IDENTIFIER [[:alpha:]_][[:alpha:][:digit:]_]*

%%

{BLOCKCOMMENT_BEGIN} {BEGIN BLOCKCOMMENT;}
<BLOCKCOMMENT>{BLOCKCOMMENT_ELE} {}
<BLOCKCOMMENT>{BLOCKCOMMENT_LINE} {lineno++;}
<BLOCKCOMMENT>{BLOCKCOMMENT_END} {BEGIN INITIAL;}
{LINECOMMENT}  {}

"int" return T_INT;
"void" return T_VOID;


"(" return LPAREN;
")" return RPAREN;
"[" return LBRACKET;
"]" return RBRACKET;
"{" return LBRACE;
"}" return RBRACE;
"," return COMMA;
";" return SEMICOLON;

"==" return EQ;
">=" return GRAEQ;
"<=" return LESEQ;
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
    yylval = node;
    return INTEGER;
}

{DECINT} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = atoi(yytext);
    yylval = node;
    return INTEGER;
}

{HEXINT} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = strtol(yytext, NULL, 16);
    yylval = node;
    return INTEGER;
}

{IDENTIFIER} {
    TreeNode* node = new TreeNode(lineno, NODE_VAR);
    node->var_name = string(yytext);
    yylval = node;
    return IDENTIFIER;
}

{WHILTESPACE} {}

{EOL} {lineno++;}

. {
    cerr << "[line "<< lineno <<" ] unknown character:" << yytext << endl;
}
%%
