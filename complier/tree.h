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
/*--------------------struct type of var----------------*/
struct Type {
    bool constvar;
    ValueType type;
    Type(ValueType valueType){ //创建变量类型值
        this->type = valueType;
        this->paramNum = 0;
        this->constvar = false;
        this->retType = nullptr;
        this->dim = 0;
        this->visitDim = 0;
    };
    void copy(Type* a){ //复制类型值
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
            for (unsigned int i=0;i<a->dim;i++)
                this->dimSize[i] = a->dimSize[i];
        }
    };
    unsigned short paramNum; //如果变量是函数,记录参数个数
    Type* paramType[MAX_PARAM]; //参数列表
    Type* retType; //返回值类型
    unsigned int dim;  // 如果变量是数组,记录维度
    int dimSize[MAX_ARRAY_DIM]; //记录维度值
    unsigned int visitDim = 0; //记录正在访问的维度
    int getSize(){ //返回变量的大小
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


enum NType
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

enum OType
{
	OP_EQ,  	// ==
	OP_NEQ, 	// !=
	OP_GE,	// >=
	OP_LE,	// <=
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
	TreeNode* child = nullptr;
	TreeNode* sibling = nullptr; //采用child_link方式记录孩子,方便后续语义分析

	NType nodeType; //记录节点类型: 变量/常量/运算符/表达式/函数参数......
	OType optype; //如果节点是运算符, 记录运算符类型
	StmtType stype;	//如果节点是表达式状态, 表达式类型
	Type* type;	//如果节点是变量/常量, 记录变量类型:数字/数组/函数
	int int_val; //如果节点是int量, 记录值
	bool b_val; //如果节点是bool量, 记录值
	string var_name; //节点名称
	string var_scope; //变量作用域标识符

	TreeNode(int lineno, NType type); //创建建立值节点
	TreeNode(TreeNode* node);	//节点的复制
	void Relate(TreeNode*); //增加第一个孩子节点
	void RelateSib(TreeNode*); //增加孩子节点
	int getVal(); //如果节点是int/bool,返回节点的值
	int childnum = 0; //孩子节点个数
	void findReturn(vector<TreeNode *> &retList); //找到return所在节点,将其添加到retlist中
	/* -------------asm------------- */
	Label label; //记录label, if_else while bool语句使用
	void gen_var_decl(); //检查并生成变量的汇编代码
	void genCode(); //生成汇编代码函数入口
	string new_label(); //创建新的label
	void get_label(); //为if_else while bool语句分配label
	string getVarNameCode(TreeNode* p); //生成取值操作的汇编代码
};
static TreeNode* Scanf = new TreeNode(0, NODE_VAR);
static TreeNode* Printf = new TreeNode(0, NODE_VAR);
