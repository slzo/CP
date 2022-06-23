#include "tree.h"

string operator + (string &content, int number) {
    return content + to_string(number);
}

string& operator += (string &content, int number) {
	return content = content + to_string(number);
}

// multimap <标识符名称， 作用域> 变量名列表
multimap<string, string> idNameList;
// map <<标识符名称， 作用域>, 结点指针> 变量列表
map<pair<string, string>, TreeNode *> idList;

// map <字符串， 标签序列号> 字符串表
map<string, int> strList;

// map <作用域+变量名， 变量相对于ebp偏移量> 局部变量表，在每次函数定义前清空
// <"11a", "-12"> 表示第一个函数的栈上第一个分配的局部变量（前3个4字节为bx,cx,dx备份用，始终保留）
map<string, int> LocalVarList;
// 栈上为局部变量分配的空间总大小，在return时进行清理
int stackSize;

// 当前所处函数的声明结点指针，return使用
TreeNode *pFunction;
// 循环体栈，为continue与break配对使用
TreeNode *cycleStack[10];
int cycleStackTop = -1;


TreeNode::TreeNode(int lineno, NodeType type) {
    this->lineno = lineno;
    this->nodeType = type;
    this->pointLevel = 0;
}

TreeNode::TreeNode(TreeNode* node) {
    this->lineno = node->lineno;
    this->nodeType = node->nodeType;
    this->optype = node->optype;
	this->stype = node->stype;
	this->type = node->type;
	this->int_val = node->int_val;
	this->ch_val = node->ch_val;
	this->b_val = node->b_val;
	this->str_val = node->str_val;
	this->var_name = node->var_name;
	this->var_scope = node->var_scope;
    this->pointLevel = node->pointLevel;
}

void TreeNode::addChild(TreeNode* child) {
    if (this->child == nullptr) {
        this->child = child;
    }
    else {
        this->child->addSibling(child);
    }
}

void TreeNode::addSibling(TreeNode* sibling) {
    TreeNode* p = this;
    while (p->sibling != nullptr) {
        p = p->sibling;
    }
    p->sibling = sibling;
}

int TreeNode::getChildNum() {
    int num = 0;
    for (TreeNode *p = child; p != nullptr; p = p->sibling)
        num++;
    return num;
}

int TreeNode::getVal() {
    if (nodeType == NODE_CONST) {
        switch (type->type)
        {
        case VALUE_BOOL:
            return (b_val ? 1 : 0);
        case VALUE_INT:
            return int_val;
        default:
            return 0;
        }
    }
    else if (child->nodeType == NODE_CONST) {
        return child->getVal();
    }
    return 0;
}

void TreeNode::genNodeId() {
    static unsigned int maxid = 0;
    this->nodeID = maxid++;
    if (this->child)
        this->child->genNodeId();
    if (this->sibling)
        this->sibling->genNodeId();
}

void TreeNode::typeCheck() {
    // 类型检查时记录循环层数，为continue和break提供循环外错误检查
    if (nodeType == NODE_STMT && stype == STMT_WHILE) {
        cycleStackTop++;
    }
    // 先遍历子节点进行type计算
    TreeNode *p = this->child;
    while (p != nullptr) {
        p->typeCheck();
        p = p->sibling;
    }
    // 分情况检查类型错误并对部分情况作强制类型转换
    switch (this->nodeType)
    {
    case NODE_FUNCALL:
        // 函数调用要求标识符是一个函数，且形参表与函数定义一致
        if (child->type->type == COMPOSE_FUNCTION) {
            if (child->var_name == "printf" || child->var_name == "scanf") {
                if (child->sibling->child->type->type != VALUE_INT) {
                    cout << "Wrong at line " << lineno << endl;
                    typeError = true;
                }
                break;
            }
            if (child->sibling->getChildNum() == child->type->paramNum) {
                int paracnt = 0;
                TreeNode *param = child->sibling->child;
                while (param!=nullptr) {
                    if (child->type->paramType[paracnt] != TYPE_NONE // 无类型表示支持任意类型，在scanf和printf上使用
                        && child->type->paramType[paracnt]->type != param->type->type) {
                        cout << "Wrong at line " << lineno << endl;
                        typeError = true;
                    }
                    paracnt++;
                    param = param->sibling;
                }
            }
            else {
                cout << "Wrong type: paramater num doesn`t fit function " << child->var_name << " , at line " << lineno << endl;
                typeError = true;
            }
        }
        else {
            cout << "Wrong type: identifier " << child->var_name << " isn`t a function, at line " << lineno << endl;
            typeError = true;
        }
        if (!type)
            this->type = new Type(NOTYPE);
        this->type->copy(child->type->retType);
        break;
    case NODE_STMT:
        // statement无类型
        this->type = TYPE_NONE;
        switch (stype) {
        case STMT_FUNCDECL: {
            vector<TreeNode *> retList;
            findReturn(retList);
            int size = retList.size();
            if (child->sibling->type->retType->type == VALUE_VOID) {
                // void类型函数无return或return无子节点
                for (int i = 0; i < size; i++) {
                    if (retList[i]->child) {
                        cout << "Wrong return: none void return in void function, at line " << retList[i]->lineno << endl;
                        typeError = true;
                    }
                }
            }
            else {
                // 其它函数必须return且类型一致
                if (size == 0) {
                    cout << "Wrong return: none void function without any return statement, function decl at line " << child->sibling->lineno << endl;
                    typeError = true;
                }
                else {
                    for (int i = 0; i < size; i++) {
                        if (retList[i]->child) {
                            if (retList[i]->child->type->type != child->type->type) {
                                cout << "Wrong type: return type can`t fit function return type, at line " 
                                << retList[i]->lineno << endl;
                            typeError = true;
                            }
                        }
                        else {
                            cout << "Wrong return: return nothing in none void function, at line " << retList[i]->lineno << endl;
                            typeError = true;
                        }
                    }
                }
            }
            break;
        }
        case STMT_IF:
        case STMT_IFELSE:
        case STMT_WHILE:
            if (child->type->type != VALUE_BOOL) {
                if (child->type->type == VALUE_INT) {
                    // 强制类型转换，添加一个"!=0"运算过程
                    TreeNode *eq = new TreeNode(child->lineno, NODE_OP);
                    eq->sibling = child->sibling;
                    eq->child = child;
                    eq->type = TYPE_BOOL;
                    eq->optype = OP_NEQ;
                    child->sibling = new TreeNode(child->lineno, NODE_CONST);
                    child->sibling->type = TYPE_INT;
                    child->sibling->int_val = 0;
                    child = eq;
                    cout << "Cast at line " << child->lineno << endl;
                }
                else {
                    cout << "Wrong at line " << child->lineno << endl;
                    typeError = true;
                }
            }
            if (stype == STMT_WHILE)
                cycleStackTop--;
            break;
        case STMT_BREAK:
        case STMT_CONTINUE:
            if (cycleStackTop < 0) {
                cout << "Error at line " << lineno << endl;
                typeError = true;
            }
            break;
        case STMT_RETURN:

            break;
        default:
            break;
        }
        break;
    case NODE_EXPR:
        this->type = this->child->type;
        this->pointLevel = this->child->pointLevel;
        break;
    case NODE_OP:
        if ( optype == OP_POS || optype == OP_NAG) {
            // 一元运算符，输入int，输出int，+(一元),-(一元)
            if (this->child->type->type != VALUE_INT) {
                cout << "Wrong at line " << lineno << endl;
                typeError = true;
            }
            this->type = TYPE_INT;
        } 
        else if (optype == OP_EQ || optype == OP_NEQ || optype == OP_ASSIGN || optype == OP_DECLASSIGN) {
            // 二元运算符，输入无限制，两侧同类型，==,!=,=
            // ！！！不要在指针声明时候初始化！！！
            if (this->child->type->type != this->child->sibling->type->type) {
                cout << "Wrong at line " << lineno << endl;
                typeError = true;
            }
            if (optype == OP_ASSIGN && child->type->constvar) {
                cout << "Wrong assign: assign to a const varable, at line " << lineno;
                typeError = true;
            }
            if (optype == OP_ASSIGN || optype == OP_DECLASSIGN)
                this->type = this->child->type;
            else
                this->type = TYPE_BOOL;
        }
        else if (optype == OP_GRA || optype == OP_LES || optype == OP_GRAEQ || optype == OP_LESEQ) {
            // 二元运算符，输入int，输出bool，>,<,>=,<=
            if (this->child->type->type != this->child->sibling->type->type || this->child->type->type != VALUE_INT) {
                cout << "Wrong at line " << lineno << endl;
                typeError = true;
            }
            this->type = TYPE_BOOL;
        }
        else if (optype == OP_NOT || optype == OP_AND || optype == OP_OR) {
            // 二元运算符，输入bool，输出bool，!,&&,||
            if (this->child->type->type != VALUE_BOOL 
                || (optype != OP_NOT 
                && this->child->type->type != this->child->sibling->type->type)) {
                cout << "Wrong at line " << lineno << endl;
                typeError = true;
            }
            this->type = TYPE_BOOL;
        }
        else if (optype == OP_INDEX) {
            // 二元运算符，输入int，输出左值类型，[]下标运算符
            if (this->child->sibling->type->type != VALUE_INT) {
                cout << "Wrong at line " << lineno << endl;
                typeError = true;
            }
            this->type = this->child->type;
        }
        else {
            // 二元运算符，输入int，输出int，+,-,*,/,%
            if (this->child->type->type != this->child->sibling->type->type || this->child->type->type != VALUE_INT) {
                cout << "Wrong at line " << lineno << endl;
                typeError = true;
            }
            this->type = TYPE_INT;
        }
        break;
    case NODE_PROG:
        this->type = TYPE_NONE;
        break;
    case NODE_VARLIST:
    case NODE_PARAM:
        if (this->child)
            this->type = this->child->type;
        break;
    default:
        break;
    }
    

}

void TreeNode::findReturn(vector<TreeNode *> &retList) {
    if (nodeType == NODE_STMT && stype == STMT_RETURN)
        retList.push_back(this);
    else {
        TreeNode *p = child;
        while (p) {
            p->findReturn(retList);
            p = p->sibling;
        }
    }
}

void TreeNode::genCode() {
    TreeNode *p = child;
    TreeNode **q;
    int N = 0, n = 1, pSize = 0;
    string varCode = "";
    switch (nodeType)
    {
    case NODE_PROG:
        gen_var_decl();
        cout << "\t.text" << endl;
        while (p) {
            if (p->nodeType == NODE_STMT && p->stype == STMT_FUNCDECL)
                p->genCode();
            p = p->sibling;
        }
        break;
    case NODE_FUNCALL:
        // 反转链表
        N = p->sibling->getChildNum();
        q = new TreeNode *[N];
        p = p->sibling->child;
        while (p) {
            q[N - n++] = p;
            p = p->sibling;
        }
        // 从右向左压栈
        for (int i = 0; i < N; i++) {
            q[i]->genCode();
            cout << "\tpushq\t%rax" << endl;
            pSize += this->child->type->paramType[i]->getSize();
        }
        // call和参数栈清理
        if(child->var_name == string("printf"))
            cout << "\tcall\t" << "printf@PLT" << endl << "\taddq\t$" << pSize << ",%rsp" << endl;
        else if (child->var_name == string("scanf") )
            cout << "\tcall\t" << "__isoc99_scanf@PLT" << endl << "\taddq\t$" << pSize << ",%rsp" << endl;
        else
            cout << "\tcall\t" << child->var_name << endl << "\taddq\t$" << pSize << ", %rsp" << endl;
        break;
    case NODE_STMT:
        switch (stype)
        {
        case STMT_FUNCDECL:
            cycleStackTop = -1;
            pFunction = this;
            get_label();
            cout << "\t.globl\t" << p->sibling->var_name << endl
                 << "\t.type\t" << p->sibling->var_name << ", @function" << endl
                 << p->sibling->var_name << ":" << endl;
            gen_var_decl();
            cout << "\tpushq\t%rbp" << endl
                 << "\tmovq\t%rsp, %rbp" << endl;
            // 在栈上分配局部变量
            cout << "\tsubq\t$" << -stackSize << ", %rsp" << endl;
            // 内部代码递归生成
            p->sibling->sibling->sibling->genCode();
            // 产生返回标签代码
            cout << this->label.next_label << ":" << endl;
            // 清理局部变量栈空间
            cout << "\taddq\t$" << -stackSize << ", %rsp" << endl;
            cout << "\tpopq\t%rbp" << endl
                 << "\tret" << endl;
            pFunction = nullptr;
            break;
        case STMT_DECL:
        case STMT_CONSTDECL:
            p = p->sibling->child;
            while (p) {
                if (p->nodeType == NODE_OP) {
                    p->child->sibling->genCode();
                    // 这里也很蠢，可以通过三地址码优化一下
                    cout << "\tmovq\t%rax, " << LocalVarList[p->child->var_scope + p->child->var_name] << "(%rbp)" << endl;
                }
                p = p->sibling;
            }
            break;
        case STMT_IF:
            get_label();
            cout << label.begin_label << ":" << endl;
            this->child->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            cout << label.false_label << ":" << endl;
            break;
        case STMT_IFELSE:
            get_label();
            cout << label.begin_label << ":" << endl;
            this->child->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            cout << "\tjmp\t\t" << label.next_label << endl;
            cout << label.false_label << ":" << endl;
            this->child->sibling->sibling->genCode();
            cout << label.next_label << ":" << endl;
            break;
        case STMT_WHILE:
            get_label();
            cycleStack[++cycleStackTop] = this;
            cout << label.next_label << ":" << endl;
            this->child->genCode();
            cout << label.true_label << ":" << endl;
            this->child->sibling->genCode();
            cout << "\tjmp\t\t" << label.next_label << endl;
            cout << label.false_label << ":" << endl;
            cycleStackTop--;
            break;
        case STMT_BREAK:
            cout << "\tjmp\t\t" << cycleStack[cycleStackTop]->label.false_label << endl;
            break;
        case STMT_CONTINUE:
            cout << "\tjmp\t\t" << cycleStack[cycleStackTop]->label.next_label << endl;
            break;
        case STMT_RETURN:
            if (p) {
                p->genCode();
            }
            cout << "\tjmp\t\t" << pFunction->label.next_label << endl;
            break;
        case STMT_BLOCK:
            while (p) {
                p->genCode();
                p = p->sibling;
            }
            break;
        default:
            break;
        }
        break;
    case NODE_EXPR:
        if (child->nodeType == NODE_VAR) {
            // 内存变量（全局/局部）
            string varCode = getVarNameCode(this->child);
            if (child->pointLevel == 0)
                cout << "\tmovq\t" << varCode << ", %rax" << endl;
            else if (child->pointLevel < 0) { // &前缀的变量
                cout << "\tleal\t" << varCode << ", %rax" << endl;
            }
            else {
                cout << "\tmovq\t" << varCode << ", %rax" << endl;
                for (int i = 0; i < child->pointLevel; i++) {
                    cout << "\tmovq\t(%rax), %rax" << endl;
                }
            }
        }
        else if (child->nodeType == NODE_OP && child->optype == OP_INDEX) {
            // 数组
            child->genCode();
        }
        else {
            // 立即数
                cout << "\tmovq\t$" << child->getVal() << ", %rax" << endl;
        }
        break;
    case NODE_OP:
        switch (optype)
        {
        case OP_EQ:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%rax, %rbx" << endl
                 << "\tsete\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tje\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_NEQ:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpq\t%rax, %rbx" << endl
                 << "\tsetne\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjne\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_GRA:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpq\t%rax, %rbx" << endl
                 << "\tsetg\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjg\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_LES:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpq\t%rax, %rbx" << endl
                 << "\tsetl\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjl\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_GRAEQ:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpq\t%rax, %rbx" << endl
                 << "\tsetge\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjge\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_LESEQ:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpq\t%rax, %rbx" << endl
                 << "\tsetle\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjle\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_NOT:
            get_label();
            p->genCode();
            // cout << "\tandl\t%rax, $eax" << endl
            //      << "\tsete\t%al" << endl;
            break;
        case OP_AND:
            get_label();
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            cout << child->label.true_label << ":" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tandq\t%rax, %rbx" << endl
                 << "\tsetne\t%al" << endl;
            break;
        case OP_OR:
            get_label();
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            cout << child->label.false_label << ":" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\torb\t%al, %bl" << endl
                 << "\tsetne\t%al" << endl;
            break;
        case OP_DECLASSIGN:
        case OP_ASSIGN:
            p->sibling->genCode();
            if (p->nodeType == NODE_VAR)
                cout << "\tmovq\t%rax, " << getVarNameCode(p) << endl;
            else {  // 左值是数组
                cout << "\tpushq\t%rax" << endl;
                // 计算偏移量到%rax
                p->child->sibling->genCode();
                cout << "\tpopq\t%rbx" << endl
                     << "\tmovq\t%rbx, " << getVarNameCode(p) << endl;
            }
            break;
        case OP_ADD:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl;
            cout << "\taddq\t%rbx, %rax" << endl;
            break;
        case OP_SUB:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tmovq\t%rax, %rbx" << endl
                 << "\tpopq\t%rax" << endl
                 << "\tsubq\t%rbx, %rax" << endl;
            break;
        case OP_POS:
            p->genCode();
            break;
        case OP_NAG:
            p->genCode();
            cout << "\tnegl\t%eax" << endl;
            break;
        case OP_MUL:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl;
            cout << "\timull\t%ebx, %eax" << endl;
            break;
        case OP_DIV:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tmovl\t%eax, %ebx" << endl
                 << "\tpopq\t%rax" << endl
                 << "\tcltd" << endl
                 << "\tidivl\t%ebx" << endl;
            break;
        case OP_MOD:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tmovl\t%eax, %ebx" << endl
                 << "\tpopq\t%rax" << endl
                 << "\tcltd" << endl
                 << "\tidivl\t%ebx" << endl
                 << "\tmovl\t%edx, %eax" << endl;
            break;
        case OP_INDEX:
            // 这里只生成下标运算在右值时的代码（即按下标取数值）
            p->sibling->genCode();
            cout << "\tmovq\t" << getVarNameCode(this) << ", %rax" << endl;
            break;
        default:
            break;
        }
    default:
        break;
    }
}


void TreeNode::gen_var_decl() {
    if (nodeType == NODE_PROG) {
        // 根节点下只处理全局变量声明
        TreeNode *p = child;
        bool print_data = false;
        while(p) {
            // 发现了p为定义语句，LeftChild为类型，RightChild为声明表
            if (p->stype == STMT_DECL || p->stype == STMT_CONSTDECL) {
                TreeNode* q = p->child->sibling->child;
                // q为变量表语句，可能为标识符或者赋值声明运算符
                while (q) {
                    if (!print_data) {
                        // 第一次遇到全局变量的时候输出
                        print_data = true;
                        cout << "\t.text" << endl
                            << "\t.data" << endl
                            << "\t.align\t4" << endl;
                    }
                    TreeNode *t = q;
                    if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN) {
                        t = q->child;
                    }
                    // 遍历常变量列表，指针类型视为4字节int
                    int varsize = ((t->type->pointLevel == 0) ? t->type->getSize() : 4);
                    if (t->type->dim > 0) {
                        t->type->elementType = p->child->type->type;
                        t->type->type = VALUE_ARRAY;
                        varsize = t->type->getSize();
                    }
                    cout << "\t.globl\t" << t->var_name << endl
                         << "\t.type\t" << t->var_name << ", @object" << endl
                         << "\t.size\t" << t->var_name << ", " << varsize << endl
                         << t->var_name << ":" << endl;
                    if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN) {
                        // 声明时赋值
                        // 只处理字面量初始化值
                        if (t->type->dim == 0) {    // 单个值                    
                            cout << "\t.long\t" << t->sibling->getVal() << endl;
                        }
                        else {    // 数组                    
                            for (TreeNode *pe = t->sibling->child; pe != nullptr; pe = pe->sibling)
                                cout << "\t.long\t" << 4 * pe->getVal() << endl;
                        }
                    }
                    else {
                        // 声明时未赋值，默认初始化值为0
                        // 只处理字面量初始化值
                        if (t->type->dim == 0) { // 单个值
                            cout << "\t.long\t0" << endl;
                        }
                        else {  // 数组
                            int size = 1;
                            for (unsigned int i = 0; i < t->type->dim; i++)
                                size *= t->type->dimSize[i];
                            cout << "\t.zero\t" << size << endl;
                        }
                    }
                    q = q->sibling;
                }
            }
            p = p->sibling;
        }
    }
    else if (nodeType == NODE_STMT && stype == STMT_FUNCDECL) {
        // 对于函数声明语句，递归查找局部变量声明
        LocalVarList.clear();
        stackSize = -12;
        int paramSize = 8;
        // 遍历参数定义列表
        TreeNode *p = child->sibling->sibling->child;
        while (p) {
            // 只能是基本数据类型，简便起见一律分配4字节
            LocalVarList[p->child->sibling->var_scope + p->child->sibling->var_name] = paramSize;
            paramSize += 4;
            p = p->sibling;
        }

        // 遍历代码段，查找函数内声明的局部变量
        p = child->sibling->sibling->sibling->child;
        while (p) {
            p->gen_var_decl();
            p = p->sibling;
        }

    }
    else if (nodeType == NODE_STMT && (stype == STMT_DECL || stype == STMT_CONSTDECL)) {
        // 找到了局部变量定义
        TreeNode* q = child->sibling->child;
        while (q) {
            // 遍历常变量列表，指针类型视为4字节int, q为标识符或声明赋值运算符
            TreeNode *t = q;
            // 声明时赋值
            if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN)
                t = q->child;
            int varsize = ((t->type->pointLevel == 0) ? t->type->getSize() : 4);
            if (t->type->dim > 0) {
                t->type->type = VALUE_ARRAY;
                varsize = t->type->getSize();
            }
            LocalVarList[t->var_scope + t->var_name] = stackSize;
            stackSize -= varsize;
            q = q->sibling;
        }
    } 
    else {
        // 在函数定义语句块内部递归查找局部变量声明
        TreeNode *p = child;
        while (p) {
            p->gen_var_decl();
            p = p->sibling;
        }
    }
}

string TreeNode::new_label() {
    static int label_seq = 0;
    string labelStr = ".L";
    labelStr += label_seq++;
    return labelStr;
}

void TreeNode::get_label() {
    string temp;
    switch (nodeType)
    {
    case NODE_STMT:
        switch (stype)
        {
        case STMT_FUNCDECL:
            this->label.begin_label = this->child->sibling->var_name;
            // next为return和局部变量清理
            this->label.next_label = ".LRET_" + this->child->sibling->var_name;
            break;
        case STMT_IF:
            this->label.begin_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = this->label.next_label = new_label();
            this->child->label.true_label = this->label.true_label;
            this->child->label.false_label = this->label.false_label;
            break;
        case STMT_IFELSE:
            this->label.begin_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = new_label();
            this->label.next_label = new_label();
            this->child->label.true_label = this->label.true_label;
            this->child->label.false_label = this->label.false_label;
            break;
        case STMT_WHILE:
            this->label.begin_label = this->label.next_label = new_label();
            this->label.true_label = new_label();
            this->label.false_label = new_label();
            this->child->label.true_label = this->label.true_label;
            this->child->label.false_label = this->label.false_label;
            break;
        default:
            break;
        }
        break;
    case NODE_OP:
        switch (optype)
        {
        case OP_AND:
            child->label.true_label = new_label();
            child->sibling->label.true_label = label.true_label;
            child->label.false_label = child->sibling->label.false_label = label.false_label;
            break;
        case OP_OR:
            child->label.true_label = child->sibling->label.true_label = label.true_label;
            child->label.false_label = new_label();
            child->sibling->label.false_label = label.false_label;
            break;
        case OP_NOT:
            child->label.true_label = label.false_label;
            child->label.false_label = label.true_label;
            break;
        default:
            break;
        }
        break;   
    default:
        break;
    }
}

string TreeNode::getVarNameCode(TreeNode* p) {
    string varCode = "";
    if (p->nodeType == NODE_VAR) { // 标识符
        if (p->var_scope == "1") // 全局变量
            varCode = p->var_name;
        else { // 局部变量（不要跨定义域访问）
            varCode += LocalVarList[p->var_scope + p->var_name];
            varCode += "(%rbp)";
        }
    }
    else { // 数组
        if (p->child->var_scope == "1")
            varCode = p->child->var_name + "(,%rax,4)";
        else {
            varCode += LocalVarList[p->child->var_scope + p->child->var_name];
            varCode += "(%rbp,%rax,4)";
        }
    }
    return varCode;
}

void InitIOFunctionNode() {
    int k = 4;
    nodeScanf->lineno = -1;
    nodeScanf->var_name = "scanf";
    nodeScanf->var_scope = "1";
    nodeScanf->type = new Type(COMPOSE_FUNCTION);
    nodeScanf->type->retType = TYPE_VOID;
    nodeScanf->type->paramType[nodeScanf->type->paramNum++] = TYPE_INT;
    for (int i = 0; i < k;i++)
        nodeScanf->type->paramType[nodeScanf->type->paramNum++] = TYPE_INT;
    idNameList.insert(make_pair("scanf", "1"));
    idList[make_pair("scanf", "1")] = nodeScanf;
    nodePrintf->lineno = -1;
    nodePrintf->var_name = "printf";
    nodePrintf->var_scope = "1";
    nodePrintf->type = new Type(COMPOSE_FUNCTION);
    nodePrintf->type->retType = TYPE_VOID;
    nodePrintf->type->paramType[nodePrintf->type->paramNum++] = TYPE_INT;
    for (int i = 0; i < k;i++)
        nodePrintf->type->paramType[nodePrintf->type->paramNum++] = TYPE_INT;
    idNameList.insert(make_pair("printf", "1"));
    idList[make_pair("printf", "1")] = nodePrintf;
}
