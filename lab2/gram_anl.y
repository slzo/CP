%{
    #include<stdio.h>
    int yylex();
    int yygram_anl();
    void yyerror(const char*);
%}
%union {
    int intval;
    char* strval;
}
%token INT
%token VOID //基本类型
%token IF ELSE WHILE BREAK CONTINUE MAIN RETURN CONST//关键字
%token <strval>ID //标识符
%token AND OR EQ NE LE GE  //长度为2运算 && || == != <= >=
%token '+' '-' '*' '/' '%' '[' ']' '(' ')' '{' '}' ',' ';'
%token <intval>Number
%%
CompUnit: Decl {printf("Decl\n");}
        | FuncDef {printf("FuncDef\n");}
        | CompUnit Decl {printf("CompUnit | Decl\n");}
        | CompUnit FuncDef {printf("CompUnit | FuncDef\n");}
        ;
Decl: ConstDecl {printf("ConstDecl\n");}
        | VarDecl {printf("VarDecl\n");}
        ;
ConstDecl: CONST Btype ConstDef ';' {printf("const int ConstDef\n");}//const declare
            ;
Btype: INT
        ;

ConstDef:ID '=' ConstInitVal { printf("%s = ConstInitVal\n",$ID);}
        | ID ConstArr '=' ConstInitVal { printf("%s ConstArraryIndex\n",$ID); }
        | ID '=' ConstInitVal ',' ConstDef { printf("%s = ConstInitVal, ConstDef\n",$ID);}
        | ID ConstArr '=' ConstInitVal ',' ConstDef { printf("%s ConstArraryIndex, ConstDef\n",$ID); }
        ;

ConstArr:'['ConstExp']' { printf("[ConstExp]\n"); }
        | '['ConstExp']' ConstArr
        ;
ConstInitVal : ConstExp {printf("ConstExp\n");}//const int value
                | '{'ConstInitVal','ConstInitVal'}' {printf("{ConstInitVal, ConstInitVal}\n");}
                ;
VarDecl: Btype VarDef ';' {printf("int VarDef;\n");} //declare
        ;
VarDef:  ID '=' InitVal { printf("%s = InitVal\n",$ID);}
        | ID ConstArr '=' InitVal { printf("%s ArraryIndex\n",$ID); }
        | ID '=' InitVal ',' ConstDef { printf("%s = InitVal, ConstDef\n",$ID);}
        | ID ConstArr '=' InitVal ',' ConstDef { printf("%s ArraryIndex, ConstDef\n",$ID); }
        ;
InitVal: ConstExp  {printf("ConstExp\n");}
        |'{'InitVal','InitVal'}' {printf("{InitVal, CInitVal}\n");}
        ;
FuncDef:INT ID '(' FuncFParams ')' Block { printf("int %s (FuncFParams) Block\n",$ID);}//function declare
        |INT ID '('  ')' Block { printf("int %s () Block\n",$2);}
        |VOID ID '(' FuncFParams ')' Block { printf("void %s (FuncFParams) Block\n",$ID);}//function declare
        |VOID ID '('  ')' Block { printf("void %s () Block\n",$2);}
        ;
FuncFParams:  Btype ID  { printf("int %s\n",$2); }
            | Btype ID FPArr
            | Btype ID ','FuncFParams { printf("int %s\n",$2); }
            | Btype ID FPArr','FuncFParams
            ;
FPArr: '['']'
        |FPArr '['Exp']'
        ;
Block: '{''}'
        |'{'BlockItem'}' {printf("{BlockItem}\n");}
        ;
BlockItem:  Decl {printf("Decl\n"); }
            | Stmt {printf("Stmt\n");}
            | Decl ',' BlockItem {printf("Decl, BlockItem\n"); }
            | Stmt ',' BlockItem {printf("Stmt, BlockItem\n");}
            ;
Stmt: LVal'='Exp';' {printf("LVal = Exp\n");}
    | Exp ';' {printf("Exp\n");}
    | Block {printf("Block\n"); }
    | IF'(' Cond ')' Stmt ELSE Stmt {printf("if (Cond) Stmt else Stmt\n"); }
    | IF'(' Cond ')' Stmt {printf("if (Cond) Stmt\n"); }
    | WHILE '(' Cond ')' Stmt {printf("while (Cond) Stmt\n"); }
    | BREAK ';' {printf("break;\n"); }
    | CONTINUE ';' {printf("continue;\n"); }
    | RETURN Exp ';' {printf("return Exp;\n"); }
    | RETURN ';' {printf("return empty;\n"); }
    ;
Exp: AddExp {printf("AddExp\n"); }
    ;
Cond: LOrExp {printf("LOrExp\n"); }
    ;
LVal: ID  { printf("%s\n",$ID); }//left of = must be var
    | ID VarArr
    ;
VarArr: '['Exp']'
        |VarArr '['Exp']'
        ;
PrimaryExp: '('Exp')'
            | LVal {printf("LVal\n");}
            | Number {printf("%d\n",$Number);}
            ;
UnaryExp: PrimaryExp { printf("PrimaryExp\n"); }
        | ID '('')' //function call
        | ID '('FuncRParams')' //function call
        | UnaryOp UnaryExp
        ;
UnaryOp: '+'
        |'-'
        |'!'
        ;
FuncRParams: FuncRParams','Exp
            | Exp
            ;
MulExp: UnaryExp { printf("UnaryExp\n"); }
        | MulExp'*'UnaryExp {printf("MulExp * UnaryExp\n"); }
        | MulExp'/'UnaryExp {printf("MulExp / UnaryExp\n"); }
        | MulExp'%'UnaryExp {printf("MulExp % UnaryExp\n"); }
        ;
AddExp: MulExp { printf("MulExp\n"); }
        | AddExp'+'MulExp {printf("AddExp + MulExp\n"); }
        | AddExp'-'MulExp {printf("AddExp - MulExp\n"); }
        ;
RelExp: AddExp {printf("AddExp\n"); }
        | RelExp'>'AddExp {printf("RelExp > AddExp\n"); }
        | RelExp'<'AddExp {printf("RelExp < AddExp\n"); }
        | RelExp GE AddExp {printf("RelExp >= AddExp\n"); }
        | RelExp LE AddExp {printf("RelExp <= AddExp\n"); }
        ;
EqExp: RelExp {printf("RelExp\n"); }
        | EqExp EQ RelExp {printf("EqExp == RelExp\n"); }
        | EqExp NE RelExp {printf("EqExp != RelExp\n"); }
        ;
LAndExp: EqExp {printf("EqExp\n"); }
        | LAndExp AND EqExp {printf("LAndExp && EqExp\n"); }
        ;
LOrExp: LAndExp {printf("LAndExp\n"); }
        | LOrExp OR LAndExp {printf("LOrExp || LAndExp\n"); }
        ;
ConstExp: AddExp {printf("AddExp\n");}
        ;

%%
int main(int argc, const char *args[]){
	extern FILE *yyin;
	if(argc > 1 && (yyin = fopen(args[1], "r")) == NULL) {
		fprintf(stderr, "can not open %s\n", args[1]);
		return 0;
	}
	if(yyparse()) {
		return 0;
	}

    return 0;
}
void yyerror(const char *s){
    fflush(stdout);
}

