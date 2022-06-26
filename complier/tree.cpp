#include "tree.h"

string operator + (string &content, int number) {
    return content + to_string(number);
}

string& operator += (string &content, int number) {
	return content = content + to_string(number);
}

multimap<string, string> idNameList; //<标识符名称， 作用域> 变量名列表
map<pair<string, string>, TreeNode *> idList;// <<标识符名称， 作用域>, 结点指针> 变量列表

// map <作用域+变量名， 变量相对于ebp偏移量> 局部变量表，在每次函数定义前清空
// <"11a", "-12"> 表示第一个函数的栈上第一个分配的局部变量（前3个4字节为bx,cx,dx备份用，始终保留）
map<string, int> LocalVarList;

int stackSize; // 栈上为局部变量分配的空间总大小

TreeNode *pFunction; // 当前所处函数的声明结点指针，return使用
TreeNode *cycleStack[10]; // 循环体栈，为continue与break配对使用
int cycleStackTop = -1;


TreeNode::TreeNode(int lineno, NType type) {
    this->lineno = lineno;
    this->nodeType = type;
}

TreeNode::TreeNode(TreeNode* node) {
    this->lineno = node->lineno;
    this->nodeType = node->nodeType;
    this->optype = node->optype;
	this->stype = node->stype;
	this->type = node->type;
	this->int_val = node->int_val;
	this->b_val = node->b_val;
	this->var_name = node->var_name;
	this->var_scope = node->var_scope;
}

void TreeNode::Relate(TreeNode* child) {
    this->childnum++;
    if (this->child == nullptr)
        this->child = child;
    else
        this->child->RelateSib(child);
}

void TreeNode::RelateSib(TreeNode* sibling) {
    TreeNode* p = this;
    while (p->sibling != nullptr) {
        p = p->sibling;
        p->childnum++;
    }
    p->sibling = sibling;
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
    else if (child->nodeType == NODE_CONST)
        return child->getVal();
    return 0;
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
        p->sibling->child->genCode();
        cout << "\tpushq\t%rax" << endl;
        pSize += this->child->type->paramType[0]->getSize(); //传入参数,只支持一个参数
        /*---------call, scanf printf特殊处理--------*/
        if(child->var_name == string("printf"))
            cout << "\tcall\t" << "printf@PLT" << endl ;//<< "\taddq\t$" << pSize << ",%rsp" << endl;
        else if (child->var_name == string("scanf") )
            cout << "\tcall\t" << "__isoc99_scanf@PLT" << endl;// << "\taddq\t$" << pSize << ",%rsp" << endl;
        else
            cout << "\tcall\t" << child->var_name << endl;// << "\taddq\t$" << pSize << ", %rsp" << endl;
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
            cout << "\tsubq\t$" << -stackSize << ", %rsp" << endl; // 在栈上分配局部变量
            p->sibling->sibling->sibling->genCode(); // 内部代码递归生成
            cout << this->label.next_label << ":" << endl; // 产生返回标签代码
            cout << "\taddq\t$" << -stackSize << ", %rsp" << endl; // 清理局部变量栈空间
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
                    cout << "\tmovl\t%eax, " << LocalVarList[p->child->var_scope + p->child->var_name] << "(%rbp)" << endl;
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
            cout << "\tjmp\t" << label.next_label << endl;
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
            cout << "\tjmp\t" << label.next_label << endl;
            cout << label.false_label << ":" << endl;
            cycleStackTop--;
            break;
        case STMT_BREAK:
            cout << "\tjmp\t" << cycleStack[cycleStackTop]->label.false_label << endl;
            break;
        case STMT_CONTINUE:
            cout << "\tjmp\t" << cycleStack[cycleStackTop]->label.next_label << endl;
            break;
        case STMT_RETURN:
            if (p)
                p->genCode();
            cout << "\tjmp\t" << pFunction->label.next_label << endl;
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
        if (child->nodeType == NODE_VAR) { // 内存变量（全局/局部）
            string varCode = getVarNameCode(this->child);
            cout << "\tmovl\t" << varCode << ", %eax" << endl;
        }
        else if (child->nodeType == NODE_OP && child->optype == OP_INDEX) // 数组
            child->genCode();
        else // 立即数
            cout << "\tmovl\t$" << child->getVal() << ", %eax" << endl;
        break;
    case NODE_OP:
        switch (optype)
        {
        case OP_EQ:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsete\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tje\t" << label.true_label << endl
                     << "\tjmp\t" << label.false_label << endl;
            }
            break;
        case OP_NEQ:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetne\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjne\t" << label.true_label << endl
                     << "\tjmp\t" << label.false_label << endl;
            }
            break;
        case OP_GRA:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetg\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjg\t" << label.true_label << endl
                     << "\tjmp\t" << label.false_label << endl;
            }
            break;
        case OP_LES:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetl\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjl\t" << label.true_label << endl
                     << "\tjmp\t" << label.false_label << endl;
            }
            break;
        case OP_GE:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetge\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjge\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_LE:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tcmpl\t%eax, %ebx" << endl
                 << "\tsetle\t%al" << endl;
            if (label.true_label != "") {
                cout << "\tjle\t\t" << label.true_label << endl
                     << "\tjmp\t\t" << label.false_label << endl;
            }
            break;
        case OP_NOT:
            get_label();
            p->genCode();
            break;
        case OP_AND:
            get_label();
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            cout << child->label.true_label << ":" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl
                 << "\tandl\t%eax, %ebx" << endl
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
                cout << "\tmovl\t%eax, " << getVarNameCode(p) << endl;
            else {
                cout << "\tpushq\t%rax" << endl;
                p->child->sibling->genCode();
                cout << "\tpopq\t%rbx" << endl
                     << "\tmovl\t%ebx, " << getVarNameCode(p) << endl;
            }
            break;
        case OP_ADD:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tpopq\t%rbx" << endl;
            cout << "\taddl\t%ebx, %eax" << endl;
            break;
        case OP_SUB:
            p->genCode();
            cout << "\tpushq\t%rax" << endl;
            p->sibling->genCode();
            cout << "\tmovq\t%rax, %rbx" << endl
                 << "\tpopq\t%rax" << endl
                 << "\tsubl\t%ebx, %eax" << endl;
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
    if (nodeType == NODE_PROG) { //root: deal the extern var
        TreeNode *p = child;
        bool print_data = false;
        while(p) { // 发现了p为定义语句，LeftChild为类型，RightChild为声明表
            if (p->stype == STMT_DECL || p->stype == STMT_CONSTDECL) {
                TreeNode* q = p->child->sibling->child;
                while (q) {
                    if (!print_data) { // first: print the header
                        print_data = true;
                        cout << "\t.text" << endl
                            << "\t.data" << endl
                            << "\t.align\t4" << endl;
                    }
                    TreeNode *t = q;
                    if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN)
                        t = q->child;
                    int varsize =  t->type->getSize();
                    if(t->type->dim > 0)
                        t->type->type = VALUE_ARRAY;
                    cout << "\t.globl\t" << t->var_name << endl
                         << "\t.type\t" << t->var_name << ", @object" << endl
                         << "\t.size\t" << t->var_name << ", " << varsize << endl
                         << t->var_name << ":" << endl;
                    if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN) { //with init
                        if (t->type->dim == 0)
                            cout << "\t.long\t" << t->sibling->getVal() << endl;
                        else
                            for (TreeNode *pe = t->sibling->child; pe != nullptr; pe = pe->sibling)
                                cout << "\t.long\t" << 4 * pe->getVal() << endl;
                    }
                    else { //without init
                        if (t->type->dim == 0)
                            cout << "\t.long\t0" << endl;
                        else {
                            int size = t->type->getSize();
                            cout << "\t.zero\t" << size << endl;
                        }
                    }
                    q = q->sibling;
                }
            }
            p = p->sibling;
        }
    }
    else if (nodeType == NODE_STMT && stype == STMT_FUNCDECL) { //function
        LocalVarList.clear();
        stackSize = -12;
        int paramSize = 8;
        TreeNode *p = child->sibling->sibling->child;
        while (p) { // scan the param , 4b for one
            LocalVarList[p->child->sibling->var_scope + p->child->sibling->var_name] = paramSize;
            paramSize += 4;
            p = p->sibling;
        }
        p = child->sibling->sibling->sibling->child;
        while (p) { //find local var in func
            p->gen_var_decl();
            p = p->sibling;
        }

    }
    else if (nodeType == NODE_STMT && (stype == STMT_DECL || stype == STMT_CONSTDECL)) { //local var: only change the stack
        TreeNode* q = child->sibling->child;
        while (q) {
            TreeNode *t = q;
            if (q->nodeType == NODE_OP && q->optype == OP_DECLASSIGN)
                t = q->child;
            int varsize = t->type->getSize();
            LocalVarList[t->var_scope + t->var_name] = stackSize;
            stackSize -= varsize;
            q = q->sibling;
        }
    } 
    else { //recursively
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
