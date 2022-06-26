%{
	#include "tree.h"
	#define YYSTYPE TreeNode *

	TreeNode* root = new TreeNode(0, NODE_PROG);
	extern int lineno;

	unsigned char max_scope_id = SCOPT_ID_BASE; //堆栈下一层结点的最大编号
	string presentScope = "1";
	unsigned top = 0;

	extern multimap<string, string> idNameList; // multimap <标识符名称， 作用域> 变量名列表
	extern map<pair<string, string>, TreeNode*> idList; // map <<标识符名称， 作用域>, 结点指针> 变量列表

	int yylex();
	int yyerror( char const * );
%}


%token INT VOID
%token ASSIGN
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token CONST IF_ ELSE WHILE_ BREAK CONTINUE RETURN
%token EQ GE LE NEQ  GRA LES
%token PLUS MINUS MUL DIV MOD AND OR NOT
%token IDENTIFIER INTEGER

%nonassoc IFF
%nonassoc ELSE
%%

ComUnit : Decl {root->Relate($1);}
		| FuncDef {root->Relate($1);}
		| ComUnit Decl {root->Relate($2);}
		| ComUnit FuncDef {root->Relate($2);}
;
Decl : ConstDecl {$$ = $1;}
	 | VarDecl {$$ = $1;}
;
ConstDecl : CONST BasicType ConstDefs SEMICOLON {
				$$ = new TreeNode(lineno, NODE_STMT);
				$$->stype = STMT_CONSTDECL;
				$$->type = TYPE_NONE;
				$$->Relate($2);
				$$->Relate($3);
				TreeNode* p = $3->child;
				while(p != nullptr) {
					p->child->type->copy($2->type);
					p->child->type->constvar = true;
					p = p->sibling;
				}
			}
;


ConstDefs : ConstDef {$$ = new TreeNode(lineno, NODE_VARLIST); $$->Relate($1);}
		  | ConstDefs COMMA ConstDef {$$ = $1; $$->Relate($3);}
;

ConstDef : DeclIdentifier ASSIGN IntConst {
				$$ = new TreeNode(lineno, NODE_OP);
				$$->optype = OP_DECLASSIGN;
				$$->int_val = $3->int_val;
				$1->int_val = $3->int_val;
				$$->Relate($1);
				$$->Relate($3);
			}
		| ArrayDeclIdentifier ASSIGN LBRACE ArrayInitVal RBRACE {
				$$ = new TreeNode(lineno, NODE_OP);
				$$->optype = OP_DECLASSIGN;
				$$->Relate($1);
				$$->Relate($4);
			}
;

BasicType : INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;}
		  | VOID {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_VOID;}
;

IntConst : INTEGER {$$ = new TreeNode(lineno, NODE_EXPR); $$->Relate($1); $$->int_val = $1->int_val; }
;

LVal : Identifier {$$ = new TreeNode($1); $$->int_val = $1->int_val; }
	| ArrayIdentifier {
		$$ = $1;
		$$->child->type->visitDim = 0;
	}
;

Identifier : IDENTIFIER {
				$$ = $1;
				int idNameCount = idNameList.count($$->var_name);
				int DeclCnt = 0;
				int minDefDis = MAX_SCOPE_STACK;
				auto it = idNameList.find($$->var_name);
				int resScoptCmp;
				while (idNameCount--) {
					unsigned plen = presentScope.length();
					unsigned vlen = (it->second).length();
					unsigned minlen = min(plen, vlen);
					if (presentScope.substr(0, minlen) == (it->second).substr(0, minlen)) {
						if (plen >= vlen)
							resScoptCmp = plen - vlen;
						else
							resScoptCmp = -1;
					}
					else	resScoptCmp =  -2;
					if (resScoptCmp >= 0){
						if (resScoptCmp < minDefDis) {
							minDefDis = resScoptCmp;
							$$ = idList[make_pair(it->first, it->second)];
						}
						DeclCnt++;
					}
					it++;
				}
				if (DeclCnt == 0) {
					string t = "UnDeclared Identifier :'" + $1->var_name + "', scope : " + to_string(resScoptCmp);
					yyerror(t.c_str());
				}
			}
;

ArrayIdentifier : Identifier LBRACKET Exp RBRACKET {
						TreeNode*tmp = new TreeNode($1);
						$$ = new TreeNode(lineno, NODE_OP);
						$$->optype = OP_INDEX;
						$$->Relate(tmp);
						// 计算数组偏移量倍数
						int biasRate = 1;
						for (unsigned i = $1->type->visitDim + 1; i < $1->type->dim; i++) {
							biasRate *= $1->type->dimSize[i];
						}
						TreeNode* biasNode;
						if (biasRate == 1)
							biasNode = $3;
						else {
							biasNode = new TreeNode(lineno, NODE_OP);
							biasNode->optype = OP_MUL;
							biasNode->Relate($3);
							TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
							TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
							biasRateConst->type = TYPE_INT;
							biasRateConst->int_val = biasRate;
							biasRateExpr->Relate(biasRateConst);
							biasNode->Relate(biasRateExpr);
						}
						$1->type->visitDim++;

						$$->Relate(biasNode);
					}
				| ArrayIdentifier LBRACKET Exp RBRACKET {
						$$ = $1;
						TreeNode* newBiasNode = new TreeNode(lineno, NODE_OP);
						newBiasNode->optype = OP_ADD;
						newBiasNode->Relate($$->child->sibling);
						$$->child->sibling = newBiasNode;
						// 计算数组偏移量
						int biasRate = 1;
						for (unsigned int i = $$->child->type->visitDim + 1; i < $$->child->type->dim; i++) {
							biasRate *= $$->child->type->dimSize[i];
						}
						TreeNode* biasNode;
						if (biasRate == 1)
							biasNode = $3;
						else {
							biasNode->optype = OP_MUL;
							biasNode->Relate($3);
							TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
							TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
							biasRateConst->type = TYPE_INT;
							biasRateConst->int_val = biasRate;
							biasRateExpr->Relate(biasRateConst);
							biasNode->Relate(biasRateExpr);
						}
						$$->child->type->visitDim++;
						newBiasNode->Relate(biasNode);
				}
;

ArrayDeclIdentifier : DeclIdentifier LBRACKET Exp RBRACKET {
							$$ = $1;
							$$->type->type = VALUE_ARRAY;
							$$->type->dimSize[$$->type->dim] = $3->int_val;
							$$->type->dim++;
						}
					| ArrayDeclIdentifier LBRACKET Exp RBRACKET {
							$$ = $1;
							$$->type->dimSize[$$->type->dim] = $3->int_val;
							$$->type->dim++;
						}
;

DeclIdentifier : IDENTIFIER {
					$$ = $1;
					$1->int_val = $$->int_val;
					$$->var_scope = presentScope;
					$$->type = new Type(NOTYPE);
					if (idList.count(make_pair($$->var_name, $$->var_scope)) != 0) {
						string t = "ReDeclared Identifier : " + $$->var_name;
						yyerror(t.c_str());
					}
					idNameList.insert(make_pair($$->var_name, $$->var_scope));
					idList[make_pair($$->var_name, $$->var_scope)] = $$;
				}
;

ArrayInitVal : IntConst {$$ = new TreeNode(lineno, NODE_VARLIST); $$->Relate($1);}
			 | ArrayInitVal COMMA IntConst {$$ = $1; $$->Relate($3);}
;

VarDecl : BasicType VarDefs SEMICOLON {
				$$ = new TreeNode(lineno, NODE_STMT);
				$$->stype = STMT_DECL;
				$$->type = TYPE_NONE;
				$$->Relate($1);
				$$->Relate($2);
				TreeNode* p = $2->child;
				while(p != nullptr) {
					if (p->nodeType == NODE_OP) {
						p->child->type->copy($1->type);
					}
					else {
						p->type->copy($1->type);
					}
					p = p->sibling;
				}
			}
;

VarDefs : VarDef {$$ = new TreeNode(lineno, NODE_VARLIST); $$->Relate($1);}
		| VarDefs COMMA VarDef {$$ = $1; $$->Relate($3);}
;

VarDef : DeclIdentifier {$$ = $1;}
		| DeclIdentifier ASSIGN Exp {
				$$ = new TreeNode(lineno, NODE_OP);
				$$->optype = OP_DECLASSIGN;
				$$->int_val = $3->int_val;
				$1->int_val = $3->int_val;
				$$->Relate($1);
				$$->Relate($3);
			}
		| ArrayDeclIdentifier {$$ = $1;}
		| ArrayDeclIdentifier ASSIGN LBRACE ArrayInitVal RBRACE {
				$$ = new TreeNode(lineno, NODE_OP);
				$$->optype = OP_DECLASSIGN;
				$$->Relate($1);
				$$->Relate($4);
			}
;

FuncDef : BasicType DeclIdentifier FuncLPAREN FuncFParams RPAREN LBRACE BlockItems RBRACE {
				$$ = new TreeNode(lineno, NODE_STMT);
				$$->stype = STMT_FUNCDECL;
				$2->type->type = COMPOSE_FUNCTION;
				TreeNode* param = $4;
				while (param != nullptr) {
					$2->type->paramType[$2->type->paramNum] = param->child->type;
					$2->type->paramNum++;
					param = param->sibling;
				}
				$2->type->retType = $1->type;
				$$->Relate($1);
				$$->Relate($2);
				TreeNode* params = new TreeNode(lineno, NODE_VARLIST);
				params->Relate($4);
				$$->Relate(params);
				TreeNode* FuncBlock = new TreeNode(lineno, NODE_STMT);
				FuncBlock->stype = STMT_BLOCK;
				FuncBlock->Relate($7);
				$$->Relate(FuncBlock);
				max_scope_id = presentScope[top] + 1;presentScope = presentScope.substr(0, presentScope.length() - 1);top--;
			}
		| BasicType DeclIdentifier FuncLPAREN RPAREN LBRACE BlockItems RBRACE {
			$$ = new TreeNode(lineno, NODE_STMT);
			$$->stype = STMT_FUNCDECL;
			$2->type->type = COMPOSE_FUNCTION;
			$2->type->retType = $1->type;
			$$->Relate($1);
			$$->Relate($2);
			$$->Relate(new TreeNode(lineno, NODE_VARLIST));
			TreeNode* FuncBlock = new TreeNode(lineno, NODE_STMT);
			FuncBlock->stype = STMT_BLOCK;
			FuncBlock->Relate($6);
			$$->Relate(FuncBlock);
			max_scope_id = presentScope[top] + 1;presentScope = presentScope.substr(0, presentScope.length() - 1);top--;
		}
;

FuncLPAREN : LPAREN { presentScope += max_scope_id; max_scope_id = SCOPT_ID_BASE; top++;};

FuncFParams : FuncFParam {$$ = $1;}
			| FuncFParams COMMA FuncFParam {$$ = $1; $$->RelateSib($3);}
;

FuncFParam : BasicType DeclIdentifier {
				$$ = new TreeNode(lineno, NODE_PARAM);
				$$->Relate($1);
				$$->Relate($2);
				$2->type->copy($1->type);
			}
;

Block : BlockLBRACE BlockItems BlockRBRACE {
			$$ = new TreeNode(lineno, NODE_STMT);
			$$->stype = STMT_BLOCK;
			$$->Relate($2);
		}
;

BlockLBRACE : LBRACE { presentScope += max_scope_id; max_scope_id = SCOPT_ID_BASE; top++;} ;
BlockRBRACE : RBRACE { max_scope_id = presentScope[top] + 1;presentScope = presentScope.substr(0, presentScope.length() - 1);top--;} ;

BlockItems : BlockItem {$$ = $1;}
			| BlockItems BlockItem {$$ = $1; $$->RelateSib($2);}
;

BlockItem : Decl {$$ = $1;}
		  | Stmt {$$ = $1;}
;

Stmt : LVal ASSIGN Exp SEMICOLON {
			$$ = new TreeNode(lineno, NODE_OP);
			$$->optype = OP_ASSIGN;
			$$->Relate($1);
			$$->Relate($3);
		}
	| SEMICOLON {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
	| Exp SEMICOLON {$$ = $1;}
	| Block {$$ = $1;}
	| IF LPAREN Cond RPAREN Stmt ELSE Stmt {
		$$ = new TreeNode(lineno, NODE_STMT);
		$$->stype = STMT_IFELSE;
		$$->Relate($3);
		$$->Relate($5);
		$$->Relate($7);
		max_scope_id = presentScope[top] + 1;presentScope = presentScope.substr(0, presentScope.length() - 1);top--;
	}
	| IF LPAREN Cond RPAREN Stmt  %prec IFF{
		$$ = new TreeNode(lineno, NODE_STMT);
		$$->stype = STMT_IF;
		$$->Relate($3);
		$$->Relate($5);
		max_scope_id = presentScope[top] + 1;presentScope = presentScope.substr(0, presentScope.length() - 1);top--;
	}
	| WHILE LPAREN Cond RPAREN Stmt {
		$$ = new TreeNode(lineno, NODE_STMT);
		$$->stype = STMT_WHILE;
		$$->Relate($3);
		$$->Relate($5);
		max_scope_id = presentScope[top] + 1;presentScope = presentScope.substr(0, presentScope.length() - 1);top--;
	}

	| BREAK SEMICOLON {
		$$ = new TreeNode(lineno, NODE_STMT);
		$$->stype = STMT_BREAK;
		$$->type = TYPE_NONE;
	}
	| CONTINUE SEMICOLON{
		$$ = new TreeNode(lineno, NODE_STMT);
		$$->stype = STMT_CONTINUE;
		$$->type = TYPE_NONE;
	}
	| RETURN SEMICOLON {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_RETURN; $$->type = TYPE_NONE;}
	| RETURN Exp SEMICOLON {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_RETURN; $$->Relate($2); $$->type = TYPE_NONE;}

;

IF : IF_ { presentScope += max_scope_id; max_scope_id = SCOPT_ID_BASE; top++;};
WHILE : WHILE_ { presentScope += max_scope_id; max_scope_id = SCOPT_ID_BASE; top++;};

Exp : AddExp {$$ = $1;} ;

Cond : LOrExp {$$ = $1;} ;

PrimaryExp : IntConst {$$ = $1;}
			| LPAREN Exp RPAREN {$$ = $2;}
			| LVal {$$ = new TreeNode(lineno, NODE_EXPR); $$->Relate($1); $$->int_val = $1->int_val;}
;
UnaryExp : PrimaryExp {$$ = $1;}
		| Identifier LPAREN FuncRParams RPAREN {
				$$ = new TreeNode(lineno, NODE_FUNCALL);
				TreeNode *tmp = new TreeNode($1);
				$$->Relate(tmp);
				$$->Relate($3);
			}
		| Identifier LPAREN RPAREN {
				$$ = new TreeNode(lineno, NODE_FUNCALL);
				TreeNode *tmp = new TreeNode($1);
				$$->Relate(tmp);
				$$->Relate(new TreeNode(lineno, NODE_VARLIST));
			}
		| NOT UnaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = !($2->b_val); $$->optype = OP_NOT; $$->Relate($2);}
		| PLUS UnaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = max($2->int_val, -$2->int_val); $$->optype = OP_POS; $$->Relate($2);}
		| MINUS UnaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = 0-$1->int_val; $$->optype = OP_NAG; $$->Relate($2);}
;

FuncRParams : Exp {$$ = new TreeNode(lineno, NODE_VARLIST); $$->Relate($1);}
			| FuncRParams COMMA Exp {$$ = $1; $$->Relate($3);}
;

MulExp : UnaryExp {$$ = $1;}
		| MulExp MUL UnaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = $1->int_val*$3->int_val; $$->optype = OP_MUL; $$->Relate($1); $$->Relate($3);}
		| MulExp DIV UnaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = $1->int_val/$3->int_val; $$->optype = OP_DIV; $$->Relate($1); $$->Relate($3);}
		| MulExp MOD UnaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = $1->int_val%$3->int_val; $$->optype = OP_MOD; $$->Relate($1); $$->Relate($3);}
;

AddExp : MulExp {$$ = $1;}
		| AddExp PLUS MulExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = $1->int_val+$3->int_val;$$->optype = OP_ADD; $$->Relate($1); $$->Relate($3);}
		| AddExp MINUS MulExp {$$ = new TreeNode(lineno, NODE_OP); $$->int_val = $1->int_val-$3->int_val;$$->optype = OP_SUB; $$->Relate($1); $$->Relate($3);}
;

RelExp : Exp {$$ = $1;}
		| RelExp GRA Exp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val > $3->b_val; $$->optype = OP_GRA; $$->Relate($1); $$->Relate($3);}
		| RelExp LES Exp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val < $3->b_val; $$->optype = OP_LES; $$->Relate($1); $$->Relate($3);}
		| RelExp GE Exp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val >= $3->b_val; $$->optype = OP_GE; $$->Relate($1); $$->Relate($3);}
		| RelExp LE Exp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val <= $3->b_val; $$->optype = OP_LE; $$->Relate($1); $$->Relate($3);}
;

EqExp : RelExp {$$ = $1;}
	  | RelExp EQ EqExp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val == $3->b_val; $$->optype = OP_EQ; $$->Relate($1); $$->Relate($3);}
	  | RelExp NEQ EqExp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val != $3->b_val; $$->optype = OP_NEQ; $$->Relate($1); $$->Relate($3);}
;

LAndExp : EqExp {$$ = $1;}
		| EqExp AND LAndExp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val && $3->b_val; $$->optype = OP_AND; $$->Relate($1); $$->Relate($3);}
;

LOrExp : LAndExp {$$ = $1;}
	   | LAndExp OR LOrExp {$$ = new TreeNode(lineno, NODE_OP); $$->b_val = $1->b_val || $3->b_val; $$->optype = OP_OR; $$->Relate($1); $$->Relate($3);}
;

%%
int yyerror(char const * message)
{
	cout << "error: " << message << ", at line " << lineno << endl;
	return 0;
}

void Init() {
    int k = 4;
    Scanf->lineno = -1;
    Scanf->var_name = "scanf";
    Scanf->var_scope = "1";
    Scanf->type = new Type(COMPOSE_FUNCTION);
    Scanf->type->retType = TYPE_VOID;
    Scanf->type->paramType[Scanf->type->paramNum++] = TYPE_INT;
    for (int i = 0; i < k;i++)
        Scanf->type->paramType[Scanf->type->paramNum++] = TYPE_INT;
    idNameList.insert(make_pair("scanf", "1"));
    idList[make_pair("scanf", "1")] = Scanf;
    Printf->lineno = -1;
    Printf->var_name = "printf";
    Printf->var_scope = "1";
    Printf->type = new Type(COMPOSE_FUNCTION);
    Printf->type->retType = TYPE_VOID;
    Printf->type->paramType[Printf->type->paramNum++] = TYPE_INT;
    for (int i = 0; i < k;i++)
        Printf->type->paramType[Printf->type->paramNum++] = TYPE_INT;
    idNameList.insert(make_pair("printf", "1"));
    idList[make_pair("printf", "1")] = Printf;
}
int main() {
    Init();
    yyparse();
    root->genCode();
    return 0;
}
