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
%type <node> St CompUnit Decl ConstDecl ConstDef ConstArr ConstInitVals ConstInitVal VarDecl VarDefs VarDef InitVals InitVal FuncDef FuncFParams FuncFParam Block BlockItem Stmt Exp Cond LVal VarArr PrimaryExp UnaryExp UnaryOp FuncRParams MulExp AddExp RelExp EqExp LAndExp LOrExp ConstExp ConstPrimaryExp ConstUnaryExp ConstMulExp ConstAddExp
%nonassoc IFF
%nonassoc ELSE
%%
St: CompUnit {  char type[]="CompUnit"; $$=Make1(type); root = $$; Relate(root, $1);}
        ;
CompUnit: Decl { char type[]="CompUnit"; $$=Make1(type); Relate($$,$1); }
        | FuncDef { char type[]="CompUnit"; $$=Make1(type); Relate($$,$1);}
        | CompUnit Decl { char type[]="CompUnit"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        | CompUnit FuncDef { char type[]="CompUnit"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        ;
Decl: ConstDecl { char type[]="Decl"; $$=Make1(type); Relate($$,$1); }
        | VarDecl { char type[]="Decl"; $$=Make1(type); Relate($$,$1); }
        ;
ConstDecl: CONST INT ConstDef ';' { char type[]="ConstDecl"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); }
            ;

ConstDef:ID '=' ConstInitVal { printf("ConstDef -> Ident = ConstInitVal\n");
                                char type[]="ConstDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        | ID ConstArr '=' ConstInitVal { printf("ConstDef -> Ident ConstArr = ConstInitVal\n");
                                        char type[]="ConstDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); }
        | ConstDef ',' ID '=' ConstInitVal  {   printf("ConstDef -> ConstDef , Ident = ConstInitVal\n");
                                                char type[]="ConstDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4);Relate($$,$5);}
        | ConstDef ',' ID ConstArr '=' ConstInitVal { printf("ConstDef -> ConstDef , Ident ConstArr = ConstInitVal\n");
                                                        char type[]="ConstDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5); Relate($$,$6);}
        ;

ConstArr:'['ConstExp']' { printf("ConstArr -> [ ConstExp ]\n");
                                char type[]="ConstArr"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        | ConstArr '['ConstExp']' { printf("ConstArr -> ConstArr [ ConstExp ]\n");
                                        char type[]="ConstArr"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); }
        ;

ConstInitVals: ConstInitVal {printf("ConstInitVals -> ConstInitVal\n");
                                char type[]="ConstInitVals"; $$=Make1(type); Relate($$,$1); }
                | ConstInitVals ',' ConstInitVal { printf("ConstInitVals -> ConstInitVals, ConstInitVal");
                                                        char type[]="ConstInitVals"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
                ;
ConstInitVal : ConstExp { printf("ConstInitVal -> ConstExp\n");
                        char type[]="ConstInitVal"; $$=Make1(type); Relate($$,$1); }
                | '{' '}' { printf("ConstInitVal -> { }\n");
                                char type[]="ConstInitVal"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
                | '{'ConstInitVals'}' { printf("ConstInitVal -> { ConstInitVals }\n");
                                        char type[]="ConstInitVal"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
                ;
VarDecl: INT VarDefs ';' { printf("VarDecl -> int VarDefs ;\n");
                                char type[]="VarDecl"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        ;
VarDefs: VarDef { printf("VarDefs -> VarDef\n");
                        char type[]="VarDefs"; $$=Make1(type); Relate($$,$1);  }
        | VarDefs ',' VarDef { printf("VarDefs -> VarDefs , VarDef\n");
                                char type[]="VarDefs"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        ;
VarDef:   ID {  printf("VarDef -> Ident\n");
                char type[]="VarDef"; $$=Make1(type); Relate($$,$1); }
        | ID '=' InitVal { printf("VarDef -> Ident = InitVal\n");
                                char type[]="VarDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        | ID ConstArr { printf("VarDef -> Ident ConstArr\n");
                        char type[]="VarDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        | ID ConstArr '=' InitVal { printf("VarDef -> Ident ConstArr = InitVal\n");
                                        char type[]="VarDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); }
        ;
InitVals: InitVal { printf("InitVals -> InitVal\n");
                        char type[]="InitVals"; $$=Make1(type); Relate($$,$1);  }
        | InitVals ',' InitVal { printf("InitVals -> InitVals , InitVal\n");
                                char type[]="InitVals"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        ;
InitVal: Exp  { printf("InitVal -> Exp\n");
                char type[]="InitVal"; $$=Make1(type); Relate($$,$1); }
        | '{' '}' { printf("InitVal -> { }\n");
                        char type[]="InitVal"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        |'{'InitVals'}' { printf("InitVal -> { InitVals }\n");
                        char type[]="InitVal"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
        ;
FuncDef:INT ID '('  ')' Block { printf("FuncDef -> int Ident () Block\n");
                                char type[]="FuncDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5);}
        |INT ID '(' FuncFParams ')' Block { printf("FuncDef -> int Ident (FuncFParams) Block\n");
                                                char type[]="FuncDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);Relate($$,$4); Relate($$,$5); Relate($$,$6);}
        |VOID ID '('  ')' Block { printf("FuncDef -> void Ident () Block\n");
                                        char type[]="FuncDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5);}
        |VOID ID '(' FuncFParams ')' Block { printf("FuncDef -> void Ident (FuncFParams) Block\n");
                                                char type[]="FuncDef"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);Relate($$,$4);Relate($$,$5);Relate($$,$6);}
        ;
FuncFParams:  FuncFParam { printf("FuncFParams -> FuncFParam\n");
                                char type[]="FuncFParams"; $$=Make1(type); Relate($$,$1); }
            | FuncFParams ',' FuncFParam { printf("FuncFParams -> FuncFParams , FuncFParam\n");
                                                char type[]="FuncFParams"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); }
            ;
FuncFParam:  INT ID  { printf("FuncFParam -> int Ident\n");
                        char type[]="FuncFParam"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
            | INT ID '['']' { printf("FuncFParam -> int Ident[]\n");
                                char type[]="FuncFParam"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); }
            | INT ID '['']' VarArr{ printf("FuncFParam -> int Ident[]VarArr\n");
                                char type[]="FuncFParam"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5); }
            ;

Block: '{''}' { printf("Block -> { }\n");
                char type[]="Block"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        |'{'BlockItem'}' { printf("Block -> { BlockItem }\n");
                                char type[]="Block"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
BlockItem:  Decl { printf("BlockItem -> Decl\n");
                        char type[]="BlockItem"; $$=Make1(type); Relate($$,$1);  }
            | Stmt { printf("BlockItem -> Stmt\n");
                        char type[]="BlockItem"; $$=Make1(type); Relate($$,$1);  }
            |  BlockItem  Decl { printf("BlockItem -> BlockItem Decl\n");
                                char type[]="BlockItem"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
            | BlockItem  Stmt { printf("BlockItem -> BlockItem Stmt\n");
                                char type[]="BlockItem"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
            ;
Stmt: LVal'='Exp';' { printf("Stmt -> LVal = Exp \n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4);}
    | Exp ';' { printf("Stmt -> Exp;\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
    | Block { printf("Stmt -> Block\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); }
    | IF'(' Cond ')' Stmt %prec IFF { printf("Stmt -> if (Cond) Stmt\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5);}
    | IF'(' Cond ')' Stmt ELSE Stmt { printf("Stmt -> if (Cond) Stmt else Stmt\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5); Relate($$,$6); Relate($$,$7);}
    | WHILE '(' Cond ')' Stmt { printf("Stmt -> while (Cond) Stmt\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); Relate($$,$5);}
    | BREAK ';' { printf("Stmt -> break;\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2);}
    | CONTINUE ';' { printf("Stmt -> continue;\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2);}
    | RETURN Exp ';' { printf("Stmt -> return Exp;\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
    | RETURN ';' { printf("Stmt -> return;\n");
                         char type[]="Stmt"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
    ;

Exp: AddExp { printf("Exp -> AddExp\n");
                 char type[]="Exp"; $$=Make1(type); Relate($$,$1); }
    ;
Cond: LOrExp { printf("Cond -> LOrExp\n");
                 char type[]="Cond"; $$=Make1(type); Relate($$,$1); }
    ;
LVal: ID  { printf("LVal -> Ident\n");
                 char type[]="LVal"; $$=Make1(type); Relate($$,$1); }
    | ID VarArr { printf("LVal -> Ident VarArr\n");
                 char type[]="LVal"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
    ;
VarArr: '['Exp']' { printf("VarArr -> [ Exp ]\n");
                                 char type[]="VarArr"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        |VarArr '['Exp']' { printf("VarArr -> VarArr [ Exp ]\n");
                                 char type[]="VarArr"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3); Relate($$,$4); }
        ;
PrimaryExp: '('Exp')'{ printf("PrimaryExp -> ( Exp )\n");
                         char type[]="PrimaryExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
            | LVal { printf("PrimaryExp -> LVal\n");
                         char type[]="PrimaryExp"; $$=Make1(type); Relate($$,$1); }
            | Number { printf("PrimaryExp -> Number\n");
                         char type[]="PrimaryExp"; $$=Make1(type); Relate($$,$1); }
            ;
UnaryExp: PrimaryExp { printf("UnaryExp -> PrimaryExp\n");
                         char type[]="UnaryExp"; $$=Make1(type); Relate($$,$1); }
        | ID '('')' { printf("UnaryExp -> Ident ()\n");
                         char type[]="UnaryExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | ID '('FuncRParams')' { printf("UnaryExp -> Ident (FuncFParams)\n");
                                 char type[]="UnaryExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);Relate($$,$4);}
        | UnaryOp UnaryExp { printf("UnaryExp -> UnaryOp UnaryExp\n");
                             char type[]="UnaryExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        ;
UnaryOp: '+' { printf("UnaryOp -> +\n");
                 char type[]="UnaryOp"; $$=Make1(type); Relate($$,$1); }
        |'-' { printf("UnaryOp -> -\n");
                 char type[]="UnaryOp"; $$=Make1(type); Relate($$,$1); }
        |'!' { printf("UnaryOp -> !\n");
                 char type[]="UnaryOp"; $$=Make1(type); Relate($$,$1); }
        ;
FuncRParams: Exp { printf("FuncRParams -> Exp\n");
                   char type[]="FuncRParams"; $$=Make1(type); Relate($$,$1);}
            | FuncRParams ',' Exp { printf("FuncRParams -> FuncRParams , Exp\n");
                                  char type[]="FuncRParams"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
            ;
MulExp: UnaryExp { printf("MulExp -> UnaryExp\n");
                   char type[]="MulExp"; $$=Make1(type); Relate($$,$1);}
        | MulExp'*'UnaryExp { printf("MulExp -> MulExp * UnaryExp\n");
                             char type[]="MulExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | MulExp'/'UnaryExp { printf("MulExp -> MulExp / UnaryExp\n");
                             char type[]="MulExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | MulExp'%'UnaryExp { printf("MulExp -> MulExp % UnaryExp\n");
                             char type[]="MulExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
AddExp: MulExp { printf("AddExp -> MulExp\n");
                char type[]="AddExp"; $$=Make1(type); Relate($$,$1); }
        | AddExp'+'MulExp { printf("AddExp -> AddExp + MulExp\n");
                           char type[]="AddExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | AddExp'-'MulExp { printf("AddExp -> AddExp - MulExp\n");
                           char type[]="AddExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
RelExp: AddExp { printf("RelExp -> AddExp\n");
                char type[]="RelExp"; $$=Make1(type); Relate($$,$1); }
        | RelExp'>'AddExp { printf("RelExp -> RelExp > AddExp\n");
                           char type[]="RelExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | RelExp'<'AddExp { printf("RelExp -> RelExp < AddExp\n");
                           char type[]="RelExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | RelExp GE AddExp { printf("RelExp -> RelExp >= AddExp\n");
                            char type[]="RelExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | RelExp LE AddExp { printf("RelExp -> RelExp <= AddExp\n");
                            char type[]="RelExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
EqExp: RelExp { printf("EqExp -> RelExp\n");
               char type[]="EqExp"; $$=Make1(type); Relate($$,$1); }
        | EqExp EQ RelExp { printf("EqExp -> EqExp == RelExp\n");
                           char type[]="EqExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | EqExp NE RelExp { printf("EqExp -> EqExp != RelExp\n");
                           char type[]="EqExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
LAndExp: EqExp { printf("LAndExp -> EqExp\n");
                char type[]="LAndExp"; $$=Make1(type); Relate($$,$1); }
        | LAndExp AND EqExp { printf("LAndExp -> LAndExp && EqExp\n");
                char type[]="LAndExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
LOrExp: LAndExp { printf("LOrExp -> LAndExp\n");
                 char type[]="LOrExp"; $$=Make1(type); Relate($$,$1); }
        | LOrExp OR LAndExp { printf("LOrExp -> LOrExp || LAndExp");
                             char type[]="LOrExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
ConstExp: ConstAddExp { printf("ConstExp -> ConstAddExp\n");
                  char type[]="ConstExp"; $$=Make1(type); Relate($$,$1); }
        ;
ConstPrimaryExp: Number { printf("ConstPrimaryExp -> Number\n");
                         char type[]="ConstPrimaryExp"; $$=Make1(type); Relate($$,$1); }
            ;
ConstUnaryExp: ConstPrimaryExp { printf("ConstUnaryExp -> ConstPrimaryExp\n");
                         char type[]="ConstUnaryExp"; $$=Make1(type); Relate($$,$1); }
        | UnaryOp ConstUnaryExp { printf("ConstUnaryExp -> UnaryOp ConstUnaryExp\n");
                             char type[]="ConstUnaryExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); }
        ;
ConstMulExp: ConstUnaryExp { printf("ConstMulExp -> ConstUnaryExp\n");
                   char type[]="ConstMulExp"; $$=Make1(type); Relate($$,$1);}
        | ConstMulExp'*'ConstUnaryExp { printf("ConstMulExp -> ConstMulExp * ConstUnaryExp\n");
                             char type[]="ConstMulExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | ConstMulExp'/'ConstUnaryExp { printf("ConstMulExp -> ConstMulExp / ConstUnaryExp\n");
                             char type[]="ConstMulExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | ConstMulExp'%'ConstUnaryExp { printf("ConstMulExp -> ConstMulExp % ConstUnaryExp\n");
                             char type[]="ConstMulExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
ConstAddExp: ConstMulExp { printf("ConstAddExp -> ConstMulExp\n");
                char type[]="ConstAddExp"; $$=Make1(type); Relate($$,$1); }
        | ConstAddExp'+'ConstMulExp { printf("ConstAddExp -> ConstAddExp + ConstMulExp\n");
                           char type[]="ConstAddExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        | ConstAddExp'-'ConstMulExp { printf("ConstAddExp -> ConstAddExp - COnstAddExp\n");
                           char type[]="ConstAddExp"; $$=Make1(type); Relate($$,$1); Relate($$,$2); Relate($$,$3);}
        ;
%%
void yyerror(const char *s) {
	printf("%s\n", s);
}

int main() {
    yyparse();
    Create(root);
    return 0;
}

