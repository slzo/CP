#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <unordered_map>
using namespace std;

/*----------struct to store the value information------------*/
#define MAX_PARAM 16
#define MAX_ARRAY_DIM 8
#define MAX_SCOPE_STACK 32
#define SCOPT_ID_BASE '1'
enum ValueType
{
    NOTYPE,
    VALUE_BOOL,
    VALUE_INT,
    VALUE_VOID,
    VALUE_ARRAY,
    COMPOSE_FUNCTION
};
class Type {
public:
    bool constvar;
    ValueType type;
    Type(ValueType valueType){
        this->type = valueType;
        this->paramNum = 0;
        this->constvar = false;
        this->retType = nullptr;
        this->dim = 0;
        this->visitDim = 0;
    };

    void copy(Type* a){
        this->type = a->type;
        this->constvar = a->constvar;
        if (a->paramNum) {
            this->paramNum = a->paramNum;
            for (unsigned short i=0;i<a->paramNum;i++)
                this->paramType[i] = a->paramType[i];
            this->retType = a->retType;
        }
        if (a->dim) {
            this->dim = a->dim;
            this->elementType = a->elementType;
            for (unsigned int i=0;i<a->dim;i++)
                this->dimSize[i] = a->dimSize[i];
        }
    };

    int pointLevel = 0;

    unsigned short paramNum; // for function
    Type* paramType[MAX_PARAM];
    Type* retType;

    unsigned int dim;   // for array
    ValueType elementType;
    int dimSize[MAX_ARRAY_DIM];
    // 下一次使用下标运算符会访问的维度
    unsigned int visitDim = 0;

    int getSize(){
        int size = 1;
        switch (type)
        {
        case VALUE_BOOL:
        case VALUE_INT:
            return 4;
        case VALUE_ARRAY:
            for (unsigned int i = 0; i < dim; i++)
                size *= dimSize[i];
            return 4 * size;
        default:
            return 0;
        }
    };

};
/*-----------------------------------------------------------*/
static Type* TYPE_INT = new Type(VALUE_INT);
static Type* TYPE_BOOL = new Type(VALUE_BOOL);
static Type* TYPE_VOID = new Type(VALUE_VOID);
static Type* TYPE_NONE = new Type(NOTYPE);


enum NodeType
{
	NODE_OP,
	NODE_CONST, 
	NODE_VAR,
	NODE_FUNCALL,
	NODE_PROG,
	NODE_STMT,
	NODE_EXPR,
	NODE_TYPE,
	NODE_VARLIST,
	NODE_PARAM,
};

enum OperatorType
{
	OP_EQ,  	// ==
	OP_NEQ, 	// !=
	OP_GRAEQ,	// >=
	OP_LESEQ,	// <=
	OP_DECLASSIGN,	// = (仅在常变量声明赋值时使用)
	OP_ASSIGN,	// =
	OP_GRA,		// >
	OP_LES,		// <
	OP_ADD,		// +
	OP_SUB,		// -
	OP_POS,		// + (一元运算符)
	OP_NAG,		// - (一元运算符)
	OP_MUL,		// *
	OP_DIV,		// /
	OP_MOD,		// %
	OP_NOT,		// !
	OP_AND, 	// &&
	OP_OR,		// ||
	OP_INDEX,	// [] 下标运算符
};

enum StmtType {
	STMT_SKIP,
	STMT_BLOCK,
	STMT_DECL,
	STMT_CONSTDECL,
	STMT_FUNCDECL,
	STMT_IFELSE,
	STMT_IF,
	STMT_WHILE,
	STMT_RETURN,
	STMT_CONTINUE,
	STMT_BREAK,
};

struct Label {
	string true_label;
	string false_label;
	string begin_label;
	string next_label;
};

/*---------------struct tree node-------------------*/
struct TreeNode {
	int lineno;

	// -------------- 语法树构造 ----------------
	TreeNode* child = nullptr;
	TreeNode* sibling = nullptr;

	NodeType nodeType;
	OperatorType optype;// 运算符类型
	StmtType stype;		// 表达式类型
	Type* type;			// 变量、类型、表达式结点，有类型。
	int int_val;
	char ch_val;
	bool b_val;
	string str_val;
	string var_name;
	string var_scope;	// 变量作用域标识符
    int pointLevel;

	TreeNode(int lineno, NodeType type);
	TreeNode(TreeNode* node);	// 仅用于叶节点拷贝，函数不复制子节点，也不复制子节点指针
	void addChild(TreeNode*);
	void addSibling(TreeNode*);
	int getChildNum();
	int getVal();

	int nodeID;
	void genNodeId();


	// -------------- 类型检查 ----------------
	void typeCheck();
	void findReturn(vector<TreeNode *> &retList);

	// ------------- asm 代码生成 -------------
	int node_seq = 0;
	int temp_var_seq = 0;
	Label label;

	void gen_var_decl();
	void gen_str();

	string new_label();
	void get_label();

	void genCode();

	// 传入标识符结点或OP_INDEX数组下标结点指针，
	// 返回它在汇编中的变量表示。
	// 注意：如果传入下标运算符结点，务必保证
	// 此时的eax寄存器已经完成了偏移量的计算，
	// 即先执行了OP_INDEX结点的右子树的genCode()
	string getVarNameCode(TreeNode* p);

};

void InitIOFunctionNode();
static TreeNode *nodeScanf = new TreeNode(0, NODE_VAR);
static TreeNode* nodePrintf = new TreeNode(0, NODE_VAR);

extern bool typeError;

#endif
