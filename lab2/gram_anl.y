%{
    #include "Tree.h"
    int yylex();
    void yyerror(const char*);
    extern char*yytext;
    struct Node*root;
%}
%union {
    struct Node* node;
}
%token <node> INT VOID //基本类型
%token <node> IF ELSE WHILE BREAK CONTINUE RETURN CONST//关键字
%token <node> ID //标识符
%token <node> AND OR EQ NE LE GE  //长度为2运算 && || == != <= >=
%token <node> '+' '-' '*' '/' '%' '[' ']' '(' ')' '{' '}' ',' ';' '>' '<' '=' '!'
%token <node> Number
%type <node> St CompUnit Decl ConstDecl ConstDef ConstArr ConstInitVals ConstInitVal VarDecl VarDefs VarDef InitVals InitVal FuncDef FuncFParams FuncFParam FPArr Block BlockItem Stmt Exp Cond LVal VarArr PrimaryExp UnaryExp UnaryOp FuncRParams MulExp AddExp RelExp EqExp LAndExp LOrExp ConstExp
%%
St: CompUnit {  char type[]="CompUnit"; root=$$=Make1(type); Insert(root, $1);}
        ;
CompUnit: Decl {printf("CompUnit -> Decl\n");
                char type[]="CompUnit"; $$=Make1(type); Insert($$,$1); }
        | FuncDef {printf("CompUnit -> FuncDef\n");
                   char type[]="CompUnit"; $$=Make1(type); Insert($$,$1);}
        | CompUnit Decl {printf("CompUnit -> CompUnit {CompUnit}\n");
                         char type[]="CompUnit"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        | CompUnit FuncDef {printf("CompUnit | FuncDef\n");
                            char type[]="CompUnit"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        ;
Decl: ConstDecl {printf("ConstDecl\n");
                 char type[]="Decl"; $$=Make1(type); Insert($$,$1); }
        | VarDecl {printf("VarDecl\n");
                   char type[]="Decl"; $$=Make1(type); Insert($$,$1); }
        ;
ConstDecl: CONST INT ConstDef ';' {printf("const int ConstDef\n");
                                        char type[]="ConstDecl"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); }
            ;

ConstDef:ID '=' ConstInitVal { printf("%s = ConstInitVal\n",$ID);
                                char type[]="ConstDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        | ID ConstArr '=' ConstInitVal { printf("%s  ConstArraryIndex\n",$1);
                                        char type[]="ConstDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); }
        | ConstDef ',' ID '=' ConstInitVal  { printf("ConstDef , %s = ConstInitVal\n",$ID);
                                                char type[]="ConstDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4);Insert($$,$5);}
        | ConstDef ',' ID ConstArr '=' ConstInitVal { printf("ConstDef , %s ConstArraryIndex\n",$ID);
                                                        char type[]="ConstDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); Insert($$,$5); Insert($$,$6);}
        ;

ConstArr:'['ConstExp']' { printf("[ConstExp]\n");
                                char type[]="ConstArr"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        | ConstArr '['ConstExp']' {
                                        char type[]="ConstArr"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); }
        ;

ConstInitVals: ConstInitVal
                | ConstInitVals ',' ConstInitVal
                ;
ConstInitVal : ConstExp {printf("ConstExp\n");
                        char type[]="ConstInitVal"; $$=Make1(type); Insert($$,$1); }
                | '{' '}' {
                                char type[]="ConstInitVal"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
                | '{'ConstInitVals'}' {printf("{ConstInitVal, ConstInitVal}\n");
                                        char type[]="ConstInitVal"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
                ;
VarDecl: INT VarDefs ';' {printf("int VarDefs;\n");
                                char type[]="VarDecl"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        ;
VarDefs: VarDef {
                        char type[]="VarDefs"; $$=Make1(type); Insert($$,$1);  }
        | VarDefs ',' VarDef {
                                char type[]="VarDefs"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        ;
VarDef:   ID { printf("\n");
                char type[]="VarDef"; $$=Make1(type); Insert($$,$1); }
        | ID '=' InitVal { printf(" InitVal\n");
                                char type[]="VarDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        | ID ConstArr { printf("ArraryIndex\n");
                        char type[]="VarDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        | ID ConstArr '=' InitVal { printf("ArraryIndex = InitVal\n");
                                        char type[]="VarDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); }
        ;
InitVals: InitVal {
                        char type[]="InitVals"; $$=Make1(type); Insert($$,$1);  }
        | InitVals ',' InitVal {
                                char type[]="InitVals"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        ;
InitVal: Exp  {printf("Exp\n");
                char type[]="InitVal"; $$=Make1(type); Insert($$,$1); }
        | '{' '}' {
                        char type[]="InitVal"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        |'{'InitVals'}' {printf("{InitVals, InitVal}\n");
                        char type[]="InitVal"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
        ;
FuncDef:INT ID '(' FuncFParams ')' Block { printf("int  (FuncFParams) Block\n");
                                                char type[]="FuncDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);Insert($$,$4); Insert($$,$5); Insert($$,$6);}
        |INT ID '('  ')' Block { printf("int  () Block\n");
                                char type[]="FuncDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$3); Insert($$,$3);}
        |VOID ID '(' FuncFParams ')' Block { printf("void  (FuncFParams) Block\n");//function declare
                                                char type[]="FuncDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);Insert($$,$4);Insert($$,$5);Insert($$,$6);}
        |VOID ID '('  ')' Block { printf("void () Block\n");
                                        char type[]="FuncDef"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); Insert($$,$5);}
        ;
FuncFParams:  FuncFParam {
                                char type[]="FuncFParams"; $$=Make1(type); Insert($$,$1); }
            | FuncFParams ',' FuncFParam {
                                                char type[]="FuncFParams"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }
            ;
FuncFParam:  INT ID  { printf("int %s\n",$2);
                        char type[]="FuncFParam"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
            | INT ID FPArr {
                                char type[]="FuncFParam"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); }

            ;
FPArr: '['']' {
                char type[]="FPArr"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        |FPArr VarArr {
                        char type[]="FPArr"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        ;
Block: '{''}' {
                char type[]="Block"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        |'{'BlockItem'}' {printf("{BlockItem}\n");
                                char type[]="Block"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
BlockItem:  Decl {printf("Decl\n");
                        char type[]="BlockItem"; $$=Make1(type); Insert($$,$1);  }
            | Stmt {printf("Stmt\n");
                        char type[]="BlockItem"; $$=Make1(type); Insert($$,$1);  }
            |  BlockItem  Decl {printf("BlockItem  Decl\n");
                                char type[]="BlockItem"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
            | BlockItem  Stmt {printf("BlockItem  Stmt\n");
                                char type[]="BlockItem"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
            ;
Stmt: LVal'='Exp';' {printf("LVal = Exp\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4);}
    | Exp ';' {printf("Exp\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
    | Block {printf("Block\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); }
    | IF'(' Cond ')' Stmt ELSE Stmt {printf("if (Cond) Stmt else Stmt\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); Insert($$,$5); Insert($$,$6); Insert($$,$7);}
    | IF'(' Cond ')' Stmt {printf("if (Cond) Stmt\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); Insert($$,$5);}
    | WHILE '(' Cond ')' Stmt {printf("while (Cond) Stmt\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); Insert($$,$5);}
    | BREAK ';' {printf("break;\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2);}
    | CONTINUE ';' {printf("continue;\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2);}
    | RETURN Exp ';' {printf("return Exp;\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
    | RETURN ';' {printf("return empty;\n");
                         char type[]="Stmt"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
    ;
Exp: AddExp {printf("AddExp\n");
                 char type[]="Exp"; $$=Make1(type); Insert($$,$1); }
    ;
Cond: LOrExp {printf("LOrExp\n");
                 char type[]="Cond"; $$=Make1(type); Insert($$,$1); }
    ;
LVal: ID  { printf("%s\n",$ID); //left of = must be var
                 char type[]="LVal"; $$=Make1(type); Insert($$,$1); }
    | ID VarArr {
                 char type[]="LVal"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
    ;
VarArr: '['Exp']' {
                                 char type[]="VarArr"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        |VarArr '['Exp']' {
                                 char type[]="VarArr"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3); Insert($$,$4); }
        ;
PrimaryExp: '('Exp')'{
                         char type[]="PrimaryExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
            | LVal {printf("LVal\n");
                         char type[]="PrimaryExp"; $$=Make1(type); Insert($$,$1); }
            | Number {printf("%d\n",$Number);
                         char type[]="PrimaryExp"; $$=Make1(type); Insert($$,$1); }
            ;
UnaryExp: PrimaryExp { printf("PrimaryExp\n");
                         char type[]="UnaryExp"; $$=Make1(type); Insert($$,$1); }
        | ID '('')' {//function call
                         char type[]="UnaryExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | ID '('FuncRParams')' {//function call
                                 char type[]="UnaryExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);Insert($$,$4);}
        | UnaryOp UnaryExp {
                             char type[]="UnaryExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); }
        ;
UnaryOp: '+' {
                 char type[]="UnaryOp"; $$=Make1(type); Insert($$,$1); }
        |'-' {
                 char type[]="UnaryOp"; $$=Make1(type); Insert($$,$1); }
        |'!' {
                 char type[]="UnaryOp"; $$=Make1(type); Insert($$,$1); }
        ;
FuncRParams: FuncRParams ',' Exp {
                                  char type[]="FuncRParams"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
            | Exp {
                   char type[]="FuncRParams"; $$=Make1(type); Insert($$,$1);}
            ;
MulExp: UnaryExp { printf("UnaryExp\n");
                   char type[]="MulExp"; $$=Make1(type); Insert($$,$1);}
        | MulExp'*'UnaryExp {printf("MulExp * UnaryExp\n");
                             char type[]="MulExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | MulExp'/'UnaryExp {printf("MulExp / UnaryExp\n");
                             char type[]="MulExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | MulExp'%'UnaryExp {printf("MulExp % UnaryExp\n");
                             char type[]="MulExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
AddExp: MulExp { printf("MulExp\n");
                char type[]="AddExp"; $$=Make1(type); Insert($$,$1); }
        | AddExp'+'MulExp {printf("AddExp + MulExp\n");
                           char type[]="MulExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | AddExp'-'MulExp {printf("AddExp - MulExp\n");
                           char type[]="MulExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
RelExp: AddExp {printf("AddExp\n");
                char type[]="RelExp"; $$=Make1(type); Insert($$,$1); }
        | RelExp'>'AddExp {printf("RelExp > AddExp\n");
                           char type[]="RelExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | RelExp'<'AddExp {printf("RelExp < AddExp\n");
                           char type[]="RelExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | RelExp GE AddExp {printf("RelExp >= AddExp\n");
                            char type[]="RelExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | RelExp LE AddExp {printf("RelExp <= AddExp\n");
                            char type[]="RelExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
EqExp: RelExp {printf("RelExp\n");
               char type[]="EqExp"; $$=Make1(type); Insert($$,$1); }
        | EqExp EQ RelExp {printf("EqExp == RelExp\n");
                           char type[]="EqExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        | EqExp NE RelExp {printf("EqExp != RelExp\n");
                           char type[]="EqExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
LAndExp: EqExp {printf("EqExp\n");
                char type[]="LAndExp"; $$=Make1(type); Insert($$,$1); }
        | LAndExp AND EqExp {printf("LAndExp && EqExp\n");
                char type[]="LAndExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
LOrExp: LAndExp {printf("LAndExp\n");
                 char type[]="LOrExp"; $$=Make1(type); Insert($$,$1); }
        | LOrExp OR LAndExp {printf("LOrExp || LAndExp\n");
                             char type[]="LOrExp"; $$=Make1(type); Insert($$,$1); Insert($$,$2); Insert($$,$3);}
        ;
ConstExp: AddExp {printf("AddExp\n");
                  char type[]="ConstExp"; $$=Make1(type); Insert($$,$1); }
        ;

%%
void yyerror(const char *s) {
	printf("%s\n", s);
}

int main(int argc, char **argv) {
    extern FILE* yyin;
    if(argc==2)
        if((yyin=fopen(argv[1],"r"))==NULL) {
                puts("WRONG!");
                return 0;
        }
    yyparse();
    Create(root);
    if(argc==2) fclose(yyin);
    return 0;
}

