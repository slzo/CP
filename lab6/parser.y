%{
	#include "tree.h"
	#define YYSTYPE TreeNode *

	TreeNode* root = new TreeNode(0, NODE_PROG);
	extern int lineno;
	extern bool parserError;

	// max_scope_id 是堆栈下一层结点的最大编号
	unsigned char max_scope_id = SCOPT_ID_BASE;
	string presentScope = "1";
	unsigned int top = 0;

	// multimap <标识符名称， 作用域> 变量名列表
	extern multimap<string, string> idNameList;
	// map <<标识符名称， 作用域>, 结点指针> 变量列表
	extern map<pair<string, string>, TreeNode*> idList;


	int yylex();
	int yyerror( char const * );
	int scopeCmp(string preScope, string varScope);
	void scopePush();
	void scopePop();
%}


%token T_INT T_VOID
%token ASSIGN
%token SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token CONST IF_ ELSE WHILE_ BREAK CONTINUE RETURN
%token EQ GRAEQ LESEQ NEQ  GRA LES
%token PLUS MINUS MUL DIV MOD AND OR NOT
%token IDENTIFIER INTEGER

%left EQ

%%

ComUnit : Decl {root->addChild($1);}
		| FuncDef {root->addChild($1);}
		| ComUnit Decl {root->addChild($2);}
		| ComUnit FuncDef {root->addChild($2);}
;
Decl : ConstDecl {$$ = $1;}
	 | VarDecl {$$ = $1;}
;
ConstDecl : CONST BasicType ConstDefs SEMICOLON {
				$$ = new TreeNode(lineno, NODE_STMT);
				$$->stype = STMT_CONSTDECL;
				$$->type = TYPE_NONE;
				$$->addChild($2);
				$$->addChild($3);
				TreeNode* p = $3->child;
				while(p != nullptr) {
					p->child->type->copy($2->type);
					p->child->type->constvar = true;
					p = p->sibling;
				}
			}
;


ConstDefs : ConstDef {$$ = new TreeNode(lineno, NODE_VARLIST); $$->addChild($1);}
		  | ConstDefs COMMA ConstDef {$$ = $1; $$->addChild($3);}
;

ConstDef : pDeclIdentifier ASSIGN IntConst {
				$$ = new TreeNode(lineno, NODE_OP);
				$$->optype = OP_DECLASSIGN;
				$$->addChild($1);
				$$->addChild($3);
			}
		| ArrayDeclIdentifier ASSIGN LBRACE ArrayInitVal RBRACE {
				$$ = new TreeNode(lineno, NODE_OP);
				$$->optype = OP_DECLASSIGN;
				$$->addChild($1);
				$$->addChild($4);
			}
;

BasicType : T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;}
		  | T_VOID {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_VOID;}
;

IntConst : INTEGER {$$ = new TreeNode(lineno, NODE_EXPR); $$->addChild($1);}
;

CompIdentifier : Identifier {$$ = new TreeNode($1);}
			   | ArrayIdentifier {
					$$ = $1;
					$$->child->type->visitDim = 0;
				}
;


ArrayIdentifier : Identifier LBRACKET Exp RBRACKET {
						TreeNode*tmp = new TreeNode($1);
						$$ = new TreeNode(lineno, NODE_OP);
						$$->optype = OP_INDEX;
						$$->addChild(tmp);
						// 计算数组偏移量倍数
						int biasRate = 1;
						for (unsigned int i = $1->type->visitDim + 1; i < $1->type->dim; i++) {
							biasRate *= $1->type->dimSize[i];
						}
						TreeNode* biasNode;
						if (biasRate == 1) {
							// 偏移倍数为1时省略乘法结点
							biasNode = $3;
						}
						else {
							biasNode = new TreeNode(lineno, NODE_OP);
							biasNode->optype = OP_MUL;
							biasNode->addChild($3);
							TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
							TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
							biasRateConst->type = TYPE_INT;
							biasRateConst->int_val = biasRate;
							biasRateExpr->addChild(biasRateConst);
							biasNode->addChild(biasRateExpr);
						}
						$1->type->visitDim++;

						$$->addChild(biasNode);
					}
				| ArrayIdentifier LBRACKET Exp RBRACKET {
						$$ = $1;
						TreeNode* newBiasNode = new TreeNode(lineno, NODE_OP);
						newBiasNode->optype = OP_ADD;
						newBiasNode->addChild($$->child->sibling);
						$$->child->sibling = newBiasNode;
						// 计算数组偏移量倍数
						int biasRate = 1;
						for (unsigned int i = $$->child->type->visitDim + 1; i < $$->child->type->dim; i++) {
							biasRate *= $$->child->type->dimSize[i];
						}
						TreeNode* biasNode;
						if (biasRate == 1) {
							// 偏移倍数为1时省略乘法结点
							biasNode = $3;
						}
						else {
							biasNode->optype = OP_MUL;
							biasNode->addChild($3);
							TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
							TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
							biasRateConst->type = TYPE_INT;
							biasRateConst->int_val = biasRate;
							biasRateExpr->addChild(biasRateConst);
							biasNode->addChild(biasRateExpr);
						}
						$$->child->type->visitDim++;
						newBiasNode->addChild(biasNode);
				}
;

Identifier : IDENTIFIER {
				$$ = $1;
				int idNameCount = idNameList.count($$->var_name);
				int DeclCnt = 0;
				int minDefDis = MAX_SCOPE_STACK;
				// 搜索变量是否已经声明
				auto it = idNameList.find($$->var_name);
				int resScoptCmp;
				while (idNameCount--) {
					resScoptCmp = scopeCmp(presentScope, it->second);
					if (resScoptCmp >= 0){
						// 寻找最近的定义
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

DeclCompIdentifier
: pDeclIdentifier {$$ = $1;}
| ArrayDeclIdentifier {$$ = $1;}
;

pDeclIdentifier
: DeclIdentifier {$$ = $1;}
;

// 常量数组标识符（仅供声明使用）
ArrayDeclIdentifier
: pDeclIdentifier LBRACKET INTEGER RBRACKET {
  $$ = $1;
  $$->type->type = VALUE_ARRAY;
  $$->type->elementType = $1->type->type;
  $$->type->dimSize[$$->type->dim] = $3->int_val;
  $$->type->dim++;
}
| ArrayDeclIdentifier LBRACKET INTEGER RBRACKET {
  $$ = $1;
  $$->type->dimSize[$$->type->dim] = $3->int_val;
  $$->type->dim++;
}
;

DeclIdentifier
: IDENTIFIER {
	$$ = $1;
	$$->var_scope = presentScope;
	$$->type = new Type(NOTYPE);
	#ifdef ID_REDUCE_DEBUG
		cout<<"# $ reduce DeclIdentifier : "<<$$->var_name<<", at scope :"<<presentScope<<endl;
	#endif
	if (idList.count(make_pair($$->var_name, $$->var_scope)) != 0) {
		string t = "ReDeclared Identifier : " + $$->var_name;
		yyerror(t.c_str());
	}
	idNameList.insert(make_pair($$->var_name, $$->var_scope));
	idList[make_pair($$->var_name, $$->var_scope)] = $$;
}
;

// ---------------- 常变量声明 -------------------



// 数组初始化值
ArrayInitVal
: IntConst {$$ = new TreeNode(lineno, NODE_VARLIST); $$->addChild($1);}
| ArrayInitVal COMMA IntConst {$$ = $1; $$->addChild($3);}
;

VarDecl
: BasicType VarDefs SEMICOLON {
  $$ = new TreeNode(lineno, NODE_STMT);
  $$->stype = STMT_DECL;
  $$->type = TYPE_NONE;
  $$->addChild($1);
  $$->addChild($2);
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
  #ifdef DECL_DEBUG
	cout << "# $ reduce VarDecl type = " << $1->type->getTypeInfo() << endl;
	// $$->printAST();
  #endif
}
;

// 连续变量定义
VarDefs
: VarDef {$$ = new TreeNode(lineno, NODE_VARLIST); $$->addChild($1);}
| VarDefs COMMA VarDef {$$ = $1; $$->addChild($3);}
;

VarDef
: DeclCompIdentifier {$$ = $1;}
| DeclCompIdentifier ASSIGN Exp {
	$$ = new TreeNode(lineno, NODE_OP);
	$$->optype = OP_DECLASSIGN;
	$$->addChild($1);
	$$->addChild($3);
  }
| ArrayDeclIdentifier ASSIGN LBRACE ArrayInitVal RBRACE {
	$$ = new TreeNode(lineno, NODE_OP);
	$$->optype = OP_DECLASSIGN;
	$$->addChild($1);
	$$->addChild($4);
  }
;

// ---------------- 函数声明 -------------------

FuncDef
: BasicType pDeclIdentifier FuncLPAREN FuncFParams RPAREN LBRACE BlockItems RBRACE {
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
	$$->addChild($1);
	$$->addChild($2);
	TreeNode* params = new TreeNode(lineno, NODE_VARLIST);
	params->addChild($4);
	$$->addChild(params);
	TreeNode* FuncBlock = new TreeNode(lineno, NODE_STMT);
	FuncBlock->stype = STMT_BLOCK;
	FuncBlock->addChild($7);
	$$->addChild(FuncBlock);
	scopePop();
  }
| BasicType pDeclIdentifier FuncLPAREN RPAREN LBRACE BlockItems RBRACE {
	$$ = new TreeNode(lineno, NODE_STMT);
	$$->stype = STMT_FUNCDECL;
	$2->type->type = COMPOSE_FUNCTION;
	$2->type->retType = $1->type;
	$$->addChild($1);
	$$->addChild($2);
	$$->addChild(new TreeNode(lineno, NODE_VARLIST));
	TreeNode* FuncBlock = new TreeNode(lineno, NODE_STMT);
	FuncBlock->stype = STMT_BLOCK;
	FuncBlock->addChild($6);
	$$->addChild(FuncBlock);
	scopePop();
  }
;

FuncLPAREN : LPAREN {scopePush();};

FuncFParams
: FuncFParam {$$ = $1;}
| FuncFParams COMMA FuncFParam {$$ = $1; $$->addSibling($3);}
;

FuncFParam
: BasicType pDeclIdentifier {
	$$ = new TreeNode(lineno, NODE_PARAM); 
	$$->addChild($1); 
	$$->addChild($2);
	$2->type->copy($1->type);
  }
;

// ---------------- 语句块 -------------------

Block
: BlockLBRACE BlockItems BlockRBRACE {
	$$ = new TreeNode(lineno, NODE_STMT);
	$$->stype = STMT_BLOCK;
	$$->addChild($2);
};

BlockLBRACE : LBRACE {scopePush();}
BlockRBRACE : RBRACE {scopePop();}

BlockItems
: BlockItem {$$ = $1;}
| BlockItems BlockItem {$$ = $1; $$->addSibling($2);}
;

BlockItem
: Decl {$$ = $1;}
| Stmt {$$ = $1;}
;

Stmt_
: LBRACE BlockItems RBRACE {
	$$ = new TreeNode(lineno, NODE_STMT);
	$$->stype = STMT_BLOCK;
	$$->addChild($2);
  }
| Stmt {$$ = $1;}
;

Stmt
: SEMICOLON {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| Exp SEMICOLON {$$ = $1;}
| Block {$$ = $1;}
| IF LPAREN Cond RPAREN Stmt_ ELSE Stmt_ {
	$$ = new TreeNode(lineno, NODE_STMT);
	$$->stype = STMT_IFELSE;
	$$->addChild($3);
	$$->addChild($5);
	$$->addChild($7);
	scopePop();
	#ifdef IFELSE_DEBUG
		cout << "# $ reduce IF-ELSE at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
| IF LPAREN Cond RPAREN Stmt_ {
	$$ = new TreeNode(lineno, NODE_STMT);
	$$->stype = STMT_IF;
	$$->addChild($3);
	$$->addChild($5);
	scopePop();
	#ifdef IF_DEBUG
		cout << "% # $ reduce IF at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
| WHILE LPAREN Cond RPAREN Stmt_ {
	$$ = new TreeNode(lineno, NODE_STMT);
	$$->stype = STMT_WHILE;
	$$->addChild($3);
	$$->addChild($5);
	scopePop();
	#ifdef WHILE
		cout << "# $ reduce WHILE at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
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
| RETURN Exp SEMICOLON {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_RETURN; $$->addChild($2); $$->type = TYPE_NONE;}
;

IF : IF_ {scopePush();};
WHILE : WHILE_ { scopePush();};

// ---------------- 表达式 -------------------

Exp
: addExp {$$ = $1;}
| CompIdentifier ASSIGN Exp {
	$$ = new TreeNode(lineno, NODE_OP);
	$$->optype = OP_ASSIGN;
	$$->addChild($1);
	$$->addChild($3);
  }
;

Cond
: LOrExp {$$ = $1;}
;

addExp
: mulExp {$$ = $1;}
| addExp PLUS mulExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_ADD; $$->addChild($1); $$->addChild($3);}
| addExp MINUS mulExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_SUB; $$->addChild($1); $$->addChild($3);}
;

// factor
mulExp
: unaryExp {$$ = $1;}
| mulExp MUL unaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_MUL; $$->addChild($1); $$->addChild($3);}
| mulExp DIV unaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_DIV; $$->addChild($1); $$->addChild($3);}
| mulExp MOD unaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_MOD; $$->addChild($1); $$->addChild($3);}
;

// 一元表达式
unaryExp
: primaryExp {$$ = $1;}
| NOT Cond {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_NOT; $$->addChild($2);}
| PLUS primaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_POS; $$->addChild($2);}
| MINUS primaryExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_NAG; $$->addChild($2);}
;

// 基本表达式
primaryExp
: IntConst {$$ = $1;}
| LPAREN Exp RPAREN {$$ = $2;}
| LPAREN Cond RPAREN {$$ = $2;}
| Identifier LPAREN FuncRParams RPAREN {
	$$ = new TreeNode(lineno, NODE_FUNCALL);
	TreeNode *tmp = new TreeNode($1);
	$$->addChild(tmp);
	$$->addChild($3);
  }
| Identifier LPAREN RPAREN {
	$$ = new TreeNode(lineno, NODE_FUNCALL);
	TreeNode *tmp = new TreeNode($1);
	$$->addChild(tmp);
	$$->addChild(new TreeNode(lineno, NODE_VARLIST));
  }
| CompIdentifier {$$ = new TreeNode(lineno, NODE_EXPR); $$->addChild($1);}
;

// 函数实参列表
FuncRParams
: Exp {$$ = new TreeNode(lineno, NODE_VARLIST); $$->addChild($1);}
| FuncRParams COMMA Exp {$$ = $1; $$->addChild($3);}
;

// 或表达式
LOrExp
: LAndExp {$$ = $1;}
| LAndExp OR LOrExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_OR; $$->addChild($1); $$->addChild($3);}
;

// 与
LAndExp
: eqExp {$$ = $1;}
| eqExp AND LAndExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_AND; $$->addChild($1); $$->addChild($3);}
;

// 相等关系
eqExp
: relExp {$$ = $1;}
| relExp EQ eqExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_EQ; $$->addChild($1); $$->addChild($3);}
| relExp NEQ eqExp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_NEQ; $$->addChild($1); $$->addChild($3);}
;

// 相对关系
relExp
: Exp {$$ = $1;}
| relExp GRA Exp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_GRA; $$->addChild($1); $$->addChild($3);}
| relExp LES Exp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_LES; $$->addChild($1); $$->addChild($3);}
| relExp GRAEQ Exp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_GRAEQ; $$->addChild($1); $$->addChild($3);}
| relExp LESEQ Exp {$$ = new TreeNode(lineno, NODE_OP); $$->optype = OP_LESEQ; $$->addChild($1); $$->addChild($3);}
;

%%

int yyerror(char const * message)
{
	cout << "error: " << message << ", at line " << lineno << endl;
	parserError = true;
	return 0;
}

int scopeCmp(string presScope, string varScope) {
	unsigned int plen = presScope.length(), vlen = varScope.length();
	unsigned int minlen = min(plen, vlen);
	if (presScope.substr(0, minlen) == varScope.substr(0, minlen)) {
		if (plen >= vlen)
			return plen - vlen;
		else
			return -1;
	}
	return -2;
}

void scopePush() {
	presentScope += max_scope_id;
	max_scope_id = SCOPT_ID_BASE;
	top++;
#ifdef SCOPE_DEBUG
	cout << "* push -> " << presentScope << ", at line " << lineno << endl;
#endif
}

void scopePop() {
	max_scope_id = presentScope[top] + 1;
	presentScope = presentScope.substr(0, presentScope.length() - 1);
	top--;
#ifdef SCOPE_DEBUG
	cout << "* pop -> " << presentScope << ", at line " << lineno << endl;
#endif
}

