#include <stdio.h>
#include <stdlib.h>
//while end_while  if then end_if
//else end_else
const char err_op[] =  "NOTE: Write ',' between operands";
//const char err_label_end[] = "NOTE: After every 'goto <label>' expected ';'" ;
const char err_while_do[] = "NOTE: Write 'DO' after WHILE <Expr>";
const char err_while_end[] = "NOTE: Write 'END WHILE' after WHILE <expr> DO <body>";
const char err_goto[] =
	"NOTE: Write label (started by '@') after 'GOTO'" ;
const char err_cmdline[] =
	"NOTE: Parser starts this way: ./parser <filename.txt>\n";
const char err_label[] =
	"NOTE: Write after every <label> expected ':'";
const char err_assign_end[] =
	"NOTE: Write after every assigment ';'" ;
const char err_assign_wait[] =
	"NOTE: Waited for assigment" ;
const char err_sq_brace[] =
	"NOTE: Close ']' after [";
const char err_goto_end[] =
	"NOTE: Write ';' after 'GOTO <label>'" ;

const char err_if[] =
	"NOTE: Write 'THEN' after 'IF <Expr>'" ;
const char err_endif[] =
	"NOTE: Write 'ENDIF' after 'IF <Expr>'" ;
const char err_else[] =
	"NOTE: Write 'ELSE' after every 'IF <Expr>'" ;
const char err_gameop0[] =
	"NOTE: Write ';' after game operator 'ENDTURN'";
const char err_gameop1[] =
	"NOTE: Write ';' after game operator 'PROD/BUILD <operand>'";
const char err_gameop2[] =
	"NOTE: Write ';' after game operator 'BUY/SELL <operand1> <operand2>'" ;
const char err_gameop[] =
	"NOTE: Write BUY/SELL/PROD/BUILD/ENDTURN...";
const char err_print[] =
	"NOTE: Write ',' for print operands, or ';' if it's all operands";
const char err_round_brace_2[] =
	"NOTE: Write ')' after '('";
const char err_round_brace_1[] =
	"NOTE: Write '(' after function with parameters";
const char err_line[] =
	"NOTE: Write line content";
const char err_term[] =
	"NOTE: Write variable/number/expression in term";
const char err_no_func[] =
	"NOTE: Write correct name of game function";
const char err_int_polish[] = 
	"ERROR: Expected INT type";
//const char err_var_def[] = 
	//"ERROR: Expected variable definition";
const char err_label_polish[] = 
	"ERROR: Expected label definition";

enum  lextype{
	label,
	variable,
	function,
	keyword,
	num,
	assign,
	str,
	separator,
	start
};

struct lexlist{
	enum lextype type;
	char *value;
	int pos;
	lexlist *next;
};
int str_cmp(const char *str1, const char *str2)
{
	int i = 0;
	while ((str1[i] != '\0')&&(str2[i] != '\0')&&
			(str1[i] == str2[i]))
		i++;
	return (str1[i] == str2[i]);
}
int str_len(const char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;
}

char* str_add(const char *str1, const char *str2)
{
	int len_res, i, j;
	char *str_res;
	len_res = str_len(str1)+str_len(str2);
	str_res = new char [len_res+1];
	i = 0;
	while (str1[i] != '\0'){
		str_res[i] = str1[i];
		i++;
	}
	j = 0;
	while (str2[j] != '\0'){
		str_res[i] = str2[j];
		i++;
		j++;
	}
	str_res[len_res] = '\0';
	return str_res;
}

char* str_copy(const char* s)
{
	int len = str_len(s);
	char* str = new char[len+1];
	for(int i = 0; i < len; i++)
		str[i] = s[i];
	str[len] = '\0';
	return str;
}

char* reverse (char *str)
{
	int i, j;
	char c;
	for (i = 0, j = str_len(str) - 1; i<j; i++,j--)
	{
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}
	return str;
}

char *int_to_str(int n)
{
	int i, sign;
	char *str;
	str = new char[16];
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		str[i++] = n%10 + '0';
	} while ((n/=10)>0);
	if (sign<0)
		str[i++] = '-';
	str[i] = '\0';
	str = reverse(str);
	return str;
}

int str_to_int(char *str)
{
	int val = 0;
	int i = 0;
	for (i = 0; str[i]; ++i)
	{
		val *= 10;
		val += str[i] - '0';
	}
	return val;
}

const char *enum_to_str(enum lextype type)
{
	switch (type) {
		case label:
			return "label";
		case variable:
			return "variable";
		case function:
			return "function";
		case keyword:
			return "keyword";
		case num:
			return "num";
		case assign:
			return "assign";
		case str:
			return "str";
		case separator:
			return "separator";
		case start:
			return "start";
	}
	return "error";
}
class Lexic
{
	enum states{
		N, H, I, K, A, S, E, C, CL, CB, CBE
	};
	int counter;
	enum states condition;
	char buf;
	int err_pos;
	lexlist *current, *list;
	bool IsSep(const char c);
	bool IsLetter(const char c);
	bool IsNum(const char c);
	void AddSymToList(char c);
	void MakeNewList(char c);
	void BuffStep();
	void NextList(enum states state, char c);
public:
	bool AskForState();
	int  AskForLine();
	Lexic();
	~Lexic(){}
	void Analyse(char c);
	void Print();
	lexlist* GetList(){return list;}
};

struct Error {
	int pos;
	char *value;
};

int Lexic::AskForLine()
{
	return counter;
}
bool Lexic:: AskForState()
{
	if(condition == H || condition == CL || condition == CB || condition == CBE)
		return true;
	else
		return false;
}

bool Lexic::IsSep(const char c)
{
	char mas[] = " \t\n+=-*/%,:><;[]{}()";
	int i = 0;
	while(mas[i] != '\0'){
		if(mas[i] == c)
			return true;
		i++;
	}
	return false;
}
bool Lexic::IsLetter(const char c)
{
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
		return true;
	}
	return false;

}
bool Lexic::IsNum(const char c)
{
	if(c >= '0' && c <= '9')
		return true;
	return false;
}
int str_size(const char *str)
{
	int i = 0;
	while (str[i] != '\0')
		i++;
	return i;

}
void Lexic::NextList(enum states state, char c)
{
	condition = state;
	MakeNewList(c);
}
Lexic::Lexic()
{
	condition = H;
	counter = 1;
	buf = '\0';
	list = NULL;
	current = list;
}
void Lexic::BuffStep()
{
	if(buf != '\0'){
		char tmp = buf;
		buf = '\0';
		Analyse(tmp);
	}
}
void Lexic::AddSymToList(char c)
{
	char *str, *newstr;
	str = current->value;
	int n = str_len(str);
	newstr = new char[n+2];
	for(int i = 0; i < n; i++)
		newstr[i] = str[i];
	newstr[n] = c;
	newstr[n+1] = '\0';
	current->value = newstr;

}
void Lexic::MakeNewList(char c)
{
	lexlist *elem = new lexlist;
	elem->next = NULL;
	elem->value = new char[2];
	elem->value[0] = c;
	elem->value[1] = '\0';
	elem->pos = counter;
	elem->type = start;
	switch(condition){
		case N:
			elem->type = num;
			break;
		case K:
			elem->type = keyword;
			break;
		case S:
			elem->type = str;
			break;
		case I:
			if(c == '$')
				elem->type = variable;
			if(c == '?')
				elem->type = function;
			if(c == '@')
				elem->type  = label;
			break;
		default:
			elem->type = separator;
	}
	if(list == NULL){
		list = elem;
		current = list;
		return;
	}
	current->next = elem;
	current = current->next;
}

void Lexic::Analyse(char c)
{
	switch (condition){
		case H:
			if(c == '/')
				condition = C;
			else if(c == '\n')
				counter++;
			else if(IsNum(c))
				NextList(N, c);
			else if(c == '?' || c == '@' || c == '$')
				NextList(I, c);
			else if(IsLetter(c))
				NextList(K, c);
			else if((c == '=' || c == '>' || c == '<'))
				NextList(A, c);
			else if (c == '"')
				NextList(S, '\0');
			else if ((c == ' ') ||(c == '\t')){
				;
			}
			else
				NextList(H, c);
			break;
		case N:
			if(IsNum(c)){
				AddSymToList(c);
			}
			else if(IsSep(c)){
				condition = H;
				buf = c;
			}
			else{
				//printf("'%c'\n", c);
				condition = E;
				err_pos = counter;
			}
			break;
		case I:
			if(IsLetter(c) || IsNum(c) ||(c == '_'))
				AddSymToList(c);
			else{
				condition = H;
				buf = c;
			}
			break;
		case K:
			if(IsLetter(c))
				AddSymToList(c);
			else{
				condition = H;
				buf = c;
			}
			break;
		case S:
			if(c != '"')
				AddSymToList(c);
			else{
				condition = H;
			}
			break;
		case A:
			if(c == '=')
				AddSymToList(c);
			else{
				current->type = separator;
				condition  = H;
				buf = c;
			}
			break;
		case C:
			if(c == '*'){
				condition = CB;
				break;
			}
			if(c == '/')
				condition = CL;
			else{
				buf = c;
				condition = H;
			}
			break;
		case CB:
			if(c == '*')
				condition = CBE;
			break;
		case CBE:
			if(c == '/'){
				condition = H;
			}
			else
				condition = CB;
			break;
		case CL:
			if(c == '\n'){
				condition = H;
				buf  = c;
			}
			break;
		case E:
			condition = E;
	}
	BuffStep();
}

void Lexic::Print()
{
	lexlist *cur = list;
	while(cur){
		printf("%s  \'%s\'  %d\n",
		cur->value, enum_to_str(cur->type), cur->pos);
		cur = cur->next;
	}
}

/*
class Table{
	int_t* int_tab;
	label_t* label_tab;


public:
	Table(){int_lab = NULL; label_tab = NULL}
	void AddInt(char* var, int val);
	int*c 
}
*/

class PolishElem;



struct PolishItem {
	PolishElem *elem;
	PolishItem *next;
};
struct IntTable{
	int value;
	char *name;
	IntTable *next;

};
class PolishElem {
public:
	virtual ~PolishElem() {};
	virtual void Evaluate(PolishItem **stack,
				PolishItem **cur_cmd,
					IntTable **int_tab) const = 0;
protected:
	static void Push(PolishItem **stack, PolishElem *elem);
	static PolishElem* Pop(PolishItem **stack);
};



struct LabelTable{
	PolishItem *item;
	char *name;
	bool init;
	LabelTable *next;
};

class PolishConst: public PolishElem {
public:
	virtual PolishElem *Clone() const = 0;
	void Evaluate(PolishItem **stack,
			PolishItem **cur_cmd,
				IntTable **int_tab) const;
};

void PolishConst::Evaluate(PolishItem **stack,
				PolishItem **cur_cmd,
					IntTable **int_tab) const
{
	Push(stack, Clone());
	*cur_cmd = (*cur_cmd) -> next;
}

void PolishElem::Push(PolishItem **stack, PolishElem *elem)
{
	PolishItem *p = new PolishItem;
	p -> elem = elem;
	p -> next = (*stack);
	(*stack) = p;
//	printf("PUSHED\n");
}

PolishElem *PolishElem::Pop(PolishItem **stack)
{
	if (*stack) {
		PolishItem *p = (*stack);
		(*stack) = (*stack) -> next;
		return p->elem;
	}
	else {
		throw ">POLISH EVALUATE: empty stack\n";
	}
}

class PolishFunc: public PolishElem {
public:
	IntTable* AddInt(IntTable *t,  int val, char *name) const
	{
		
		printf("name:%s value: %d\n", name, val);
		IntTable *tab = t;
		IntTable *integer = new IntTable;
		integer -> value = val;
		integer -> name = name;
		integer -> next = NULL;	
		if (tab == NULL){
			printf("CREATE NEW INT TABLE WITH FIRST INTEGER\n");
			tab = integer;
		}
		else{
			while(t != NULL){
				if(str_cmp(t->name, name)){
					printf("variable found!\n");
					t->value = val;
					//printf("int name:%s value: %d\n",
							//t->name, t->value);
					return tab;
				}
				if(t->next == NULL)
					break;
				t = t->next;
			}
			printf("ADDING NEW INTEGER TO LABLE TABLE INT\n");
			t->next = integer;	

			printf("******INTEGER TABLE IN ADDINT******\n");
			IntTable *tmp = tab;
			while(tmp){
				printf("int %s = %d\n", tmp->name, tmp->value);
				tmp = tmp->next;
			}
			printf("************************************\n");
			//printf("int name:%s value: %d\n",
					//t->name, t->value);
		}
		return tab;
	}


 	IntTable* FindInt(IntTable *int_tab, char* n) const
	{
		IntTable *t = int_tab;
		while(t != NULL){
			if(str_cmp(n, t->name)){
				return t;
			}
			t = t->next;
		}
		return NULL;
	}
	void Evaluate(PolishItem **stack, PolishItem **cur_cmd,
					IntTable **int_tab) const;
	virtual PolishElem* EvaluateFun(PolishItem **stack, 
					IntTable **int_tab) const = 0;
};

void PolishFunc::Evaluate(PolishItem **stack, 
			PolishItem **cur_cmd, IntTable **int_tab) const
{
	PolishElem *res = EvaluateFun(stack, int_tab);
	if (res) 
		Push(stack, res);
	*cur_cmd = (*cur_cmd)->next;
}

class PolishInt : public PolishConst {
	int value;
public:
	PolishInt(int a) { value = a; printf("Create PolishInt(%d)\n", a); }
	virtual ~PolishInt() {}
	virtual PolishElem* Clone() const
		{return new PolishInt(value); }
	int Get() const {return value;}
};

class PolishString : public PolishConst {
	char *value;
public:
	PolishString(char *a)
	{ 
		value = a; 
		printf("Created PolishString(%s)\n", a); 
	}
	virtual ~PolishString() {}
	virtual PolishElem* Clone() const
		{return new PolishString(str_copy(value)); }
	char *Get() const {return value;}
};

class PolishLabel: public PolishConst {
	PolishItem* addr;
	const char* name;
public:
	PolishLabel(PolishItem* a, const char *n) 
	{ 
		addr = a; 
		name = n; 
		printf("Created PolishLabel(%s)\n", name); 
	}
	void SetAddr(PolishItem* a)
	{
		addr = a;
		printf("Adress for PolishLabel(%s) has been set\n", name);
	}
	virtual ~PolishLabel() {}
	virtual PolishElem* Clone() const
		{return new PolishLabel(addr, name);}
	PolishItem* Get() const {return addr;}
	const char* GetName() const {return name;}
};

/*class PolishEx: public PolishElem{
public:

};

class PolishExNotLabel: public PolishEx{
public:
	PolishExNotLabel(PolishElem *x) {};
};*/


class PolishNoOp: public PolishElem{

public:
	PolishNoOp() {printf("Created PolishNoOp()\n");}
	virtual ~PolishNoOp() {}
	void Evaluate(PolishItem **stack, 
			PolishItem **cur_cmd, IntTable **int_tab) const
	{
		printf("Evaluate NoOp\n");
		*cur_cmd = (*cur_cmd)->next;
	}
};
class PolishOpGo: public PolishElem {
public:
	PolishOpGo() {printf("Created PolishOpGo()\n");}
	virtual ~PolishOpGo() {}
	void Evaluate(PolishItem **stack, 
			PolishItem **cur_cmd, IntTable **int_tab) const
	{
		printf("Evaluate OpGo\n");
		PolishElem *operand1 = Pop(stack);
		PolishLabel *lab = dynamic_cast<PolishLabel*>(operand1);
		if (!lab){
			printf("%s\n", err_label_polish); 
			exit(1);
		}
		*cur_cmd = lab -> Get();
		delete operand1;
	}
};	

class PolishOpGoElse: public PolishElem {
public:
	PolishOpGoElse() {printf("Created PolishOpGoElse()\n");}
	virtual ~PolishOpGoElse() {}
	void Evaluate(PolishItem **stack,
			PolishItem **cur_cmd, IntTable **int_tab) const
	{
		printf("Evaluate OpGoElse\n");
		PolishElem *operand1 = Pop(stack);
		PolishLabel *lab = dynamic_cast<PolishLabel*>(operand1);
		
		PolishElem *operand2 = Pop(stack);
		PolishInt *num = dynamic_cast<PolishInt*>(operand2);

		if(!num){
			printf("%s\n", err_int_polish); 
			exit(1);
		}
		if (!lab){
			printf("%s\n", err_label_polish); 
			exit(1);
		}
		if(num->Get() == 0){
			*cur_cmd = lab -> Get();
		}
		else{
			*cur_cmd = (*cur_cmd) -> next;
		}
		delete operand1;
	}
};
class PolishFunAdd: public PolishFunc {
public:
	PolishFunAdd() {printf("Create PolishFunAdd()\n");}
	virtual ~PolishFunAdd() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate Add\n");
		PolishElem *operand1 = Pop(stack); 
		printf("POP1\n");
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		if(!i1) {
			printf("%s in plus\n", err_int_polish); 
			exit(1);
		}
		PolishElem *operand2 = Pop(stack); 
		printf("POP2\n");
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
		if (!i1){
			printf("%s in plus\n", err_int_polish); 
			exit(1);
		}
		int res = i1->Get() + i2->Get();
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

	
class PolishFunAND: public PolishFunc {
public:
	PolishFunAND() {printf("Created PolishFunAND()\n");}
	virtual ~PolishFunAND() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate AND\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		if(!i1) {
			printf("%s in AND\n", err_int_polish); 
			exit(1);
		}
		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
		if (!i2) {
			printf("%s in AND\n", err_int_polish); 
			exit(1);
		}
		int res = (i1->Get())&&(i2->Get());
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

class PolishFunOR: public PolishFunc {
public:
	PolishFunOR() {printf("Create PolishFunOR()\n");}
	virtual ~PolishFunOR() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunOR\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		if(!i1) {
			printf("%s in OR\n", err_int_polish); 
			exit(1);
		}
		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
		if (!i2) {
			printf("%s in OR\n", err_int_polish);  
			exit(1);
		}
		int res = (i1->Get())||(i2->Get());
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

class PolishFunComp: public PolishFunc {
	char *name;
public:
	PolishFunComp(char *a) 
	{
		printf("Create PolishFunComp(%s)\n", a);  
		name = a;
	}
	virtual ~PolishFunComp() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunComp\n");
		int res;
		PolishElem *operand1 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand1);
		if(!i2){
			printf("%s in Comp\n", err_int_polish);  
			exit(1);
		}
		PolishElem *operand2 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand2);
		if (!i1){
			printf("%s in Comp\n", err_int_polish); 
			exit(1);
		}
		if (str_cmp(name, ">"))
			res = (i1->Get())>(i2->Get());
		if (str_cmp(name, ">="))
			res = (i1->Get())>=(i2->Get());
		if (str_cmp(name, "==")){
			res = (i1->Get())==(i2->Get());
		}
		if (str_cmp(name, "<"))
			res = (i1->Get())<(i2->Get());
		if (str_cmp(name, "<="))
			res = (i1->Get())<=(i2->Get());
		if (str_cmp(name, "!="))
			res = (i1->Get())!=(i2->Get());
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

class PolishFunNOT: public PolishFunc {
public:
	PolishFunNOT() {printf("Create PolishFunNOT()\n");}
	virtual ~PolishFunNOT() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunNot\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		if(!i1){
			printf("%s in NOT\n", err_int_polish); 
			exit(1);
		}
		int res = !(i1->Get());
		delete operand1;
		return new PolishInt(res);
	}
};

class PolishFunNeg: public PolishFunc {
public:
	PolishFunNeg() {printf("Create PolishFunNeg()\n");}
	virtual ~PolishFunNeg() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunNeg\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		if(!i1) {
			printf("%s in PolishFunNeg()\n", err_int_polish); 
			exit(1);
		}
		int res = 0 - (i1->Get());
		delete operand1;
		return new PolishInt(res);
	}
};

class PolishFunSub: public PolishFunc {
public:
	PolishFunSub() {printf("Create PolishFunSub()\n");}
	virtual ~PolishFunSub() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunSub\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
//		if (!i2) throw IPNExNotInt(operand2);
		int res = i2->Get() - i1->Get();
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

class PolishFunMult: public PolishFunc {
public:
	PolishFunMult() {printf("Create PolishFunMult()\n");}
	virtual ~PolishFunMult() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunMult\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
//		if (!i2) throw IPNExNotInt(operand2);
		int res = (i1->Get()) * (i2->Get());
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

class PolishFunDiv: public PolishFunc {
public:
	PolishFunDiv() {printf("Create PolishFunDiv()\n");}
	virtual ~PolishFunDiv() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunDiv\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
//		if (!i2) throw IPNExNotInt(operand2);
		int res = int((i2->Get())/(i1->Get()));
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};

class PolishFunModulo: public PolishFunc {
public:
	PolishFunModulo() {printf("Create PolishFunModulo()\n");}
	virtual ~PolishFunModulo() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunModulo\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);
//		if (!i2) throw IPNExNotInt(operand2);
		int res = int((i2->Get())%(i1->Get()));
		delete operand1;
		delete operand2;
		return new PolishInt(res);
	}
};



class PolishFunGetVar: public PolishFunc {
public:
	PolishFunGetVar() {printf("Create PolishFunGetVar()\n");}
	virtual ~PolishFunGetVar() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunGetVar\n");
		PolishElem *operand1 = Pop(stack);
		IntTable *res1;
		PolishString *i1 = dynamic_cast<PolishString*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
		printf("%s\n", i1->Get());
		res1 = FindInt(*int_tab, i1->Get());
		if (res1 == NULL){

			throw ">POLISH EVALUATE: INTEGER NOT FOUND\n";
		}
		int res = res1->value;
//		if (!i2) throw IPNExNotInt(operand2);
		delete operand1;
		return new PolishInt(res);
	}
};

class PolishFunAssign: public PolishFunc {
public:
	PolishFunAssign() {printf("Create PolishFunAssign()\n");}
	virtual ~PolishFunAssign() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunAssign\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		PolishElem *operand2 = Pop(stack);
		PolishString *i2 = dynamic_cast<PolishString*>(operand2);
		if(!(i1&&i2)){
			printf("EXPECTED INT AND STRING ON STACK\n");
			exit(1);
		}
		printf("%d == ", i1->Get());
//		if(!i1) throw IPNExNotInt(operand1);
		
		printf("%s\n", i2->Get());

		*int_tab = AddInt(*int_tab,  i1->Get(),  i2->Get());
//		if (!i2) throw IPNExNotInt(operand2);
		
		delete operand1;
		delete operand2;
		return NULL;
	}
};

class PolishFunPrintInt: public PolishFunc {
public:
	PolishFunPrintInt() {printf("Create PolishFunPrintInt()\n");}
	virtual ~PolishFunPrintInt() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate PrintInt\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
//		if (!i2) throw IPNExNotInt(operand2);
		printf("%d\n", i1->Get());
		delete operand1;
		return NULL;
	}
};

class PolishFunPrintStr: public PolishFunc {
public:
	PolishFunPrintStr() {printf("Create PolishFunPrintStr()\n");}
	virtual ~PolishFunPrintStr() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunPrintStr\n");
		PolishElem *operand1 = Pop(stack);
		PolishString *i1 = dynamic_cast<PolishString*>(operand1);
//		if(!i1) throw IPNExNotInt(operand1);
//		if (!i2) throw IPNExNotInt(operand2);
		printf("%s\n", i1->Get());
		delete operand1;
		return NULL;
	}
};

class PolishFunMas: public PolishFunc {
public:
	PolishFunMas() {printf("Create PolishFunMas()\n");}
	virtual ~PolishFunMas() {}
	PolishElem* EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate FunMas\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);
		int n = i1->Get();
//		if(!i1) throw IPNExNotInt(operand1);
//		if (!i2) throw IPNExNotInt(operand2);
		PolishElem *operand2 = Pop(stack);
		PolishString *i2 = dynamic_cast<PolishString*>(operand2);
		delete operand1;
		delete operand2;
		//printf("[%s]\n", int_to_str(i1->Get()));
		char *str = str_add(str_add(i2->Get(), "."), 
						int_to_str(n));
		IntTable *find = FindInt(*int_tab, str);
		if(!find){
			AddInt(*int_tab, 0, str);
		}
		return new PolishString(str);
	}
};



class PolishFun0: public PolishFunc {
	const char *func_name;
public:
	PolishFun0(const char *f) {
		func_name = f;
		printf("Create PolishFun0()\n");
	}
	virtual ~PolishFun0() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate Fun0\n");
		printf("%s\n", func_name);
//		Here will be analation of func
//		if(!i1) throw IPNExNotInt(operand1);
//		if (!i2) throw IPNExNotInt(operand2);c
		return NULL;
	}
};

class PolishFun1: public PolishFunc {
	const char *func_name;
public:
	PolishFun1(const char *f) 
	{
		func_name = f;
		printf("Create PolishFun1()\n");
	}
	virtual ~PolishFun1() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate Fun1\n");
		PolishElem *operand2 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand2);
		if(!i1){
			printf("%s\n", err_int_polish);
		}
		printf("%s %d\n", func_name, i1->Get());
//		if(!i1) throw IPNExNotInt(operand1);
//		if (!i2) throw IPNExNotInt(operand2);
//		printf("%s\n", i1->Get_name());
//		delete operand1;
		return NULL;
	}
};

class PolishFun2: public PolishFunc {
	const char *func_name;

public:
	PolishFun2(const char *f) 
	{
		func_name = f;
		printf("Create PolishFun2()\n");
	}
	virtual ~PolishFun2() {}
	PolishElem *EvaluateFun(PolishItem **stack, IntTable **int_tab) const
	{
		printf("Evaluate Fun2\n");
		PolishElem *operand1 = Pop(stack);
		PolishInt *i1 = dynamic_cast<PolishInt*>(operand1);

		PolishElem *operand2 = Pop(stack);
		PolishInt *i2 = dynamic_cast<PolishInt*>(operand2);

		if(!i1){
			printf("%s\n", err_int_polish);
		}
		if(!i2){
			printf("%s\n", err_int_polish);
		}
		printf("%s %d, %d\n", func_name, i1->Get(), i2->Get());
//		if(!i1) throw IPNExNotInt(operand1);
//		if (!i2) throw IPNExNotInt(operand2);
//		printf("%s\n", i1->Get_name());
		delete operand1;
		delete operand2;
		return NULL;
	}
};

class Syntax {
	lexlist *curlex;
	lexlist *list;
	PolishItem *polish_list; 
	PolishItem *polish_cur; 
	LabelTable *label_tab;
	LabelTable *label_tab_cur;
	void START();
	void LABEL();
	void OPERATOR();
	void IF_ELSE();
	void ASSIGN();//assigning variables
	void GAME_OP();//game operator
	void PRINT_ELEM();
	void PRINT_NEXT();
	void EXPRESSION();//expression

	void L1();
	void R1();
	void L2();
	void R2();
	void UNARY();
	void UNARY_MULT_DIV();

	void ADD_SUB();
	void TERM();
	void MULT_DIV();
	void MAS_VAL();
	void GOTO();
	void BODY_IF();
	void BODY_ELSE();
	void BODY_WHILE();
	void WHILE();
	void FUNC();
public:
	Syntax(lexlist *l)
	{
		list = l;
		curlex = l;
		polish_list = NULL;
		polish_cur = polish_list;
		label_tab = NULL;
		label_tab_cur = label_tab;
	}
	~Syntax(){}
	void PrintLabels()
	{
		LabelTable *tmp = label_tab;
		printf("===LABEL LIST===\n");
		while(tmp){
			printf("%s\n", tmp->name);
			tmp = tmp->next;
		}
		printf("=================\n");
	}
	void GetLex();
	void Analyse();
	PolishItem* PutPolish(PolishElem *p);
	void AddLabel(PolishItem *p, char *name);
	LabelTable* FindLabel(const char *name);
	PolishItem* GetPolish(){return polish_list;}
	Error* MakeError(const char *comment,
			const char *exp, const char *cmd);
};

PolishItem* Syntax::PutPolish(PolishElem *p)
{
	if(polish_list == NULL){
		polish_list = new PolishItem;
		polish_list -> elem = p;
		polish_list -> next = NULL;
		polish_cur = polish_list;
	}
	else{
		polish_cur -> next = new PolishItem;
		polish_cur = polish_cur -> next;
		polish_cur -> elem = p;
		polish_cur -> next = NULL;
	}
	return polish_cur;
}
LabelTable* Syntax::FindLabel(const char *name)
{
	LabelTable *tmp = label_tab;
	while(tmp != NULL){
		if(str_cmp(tmp->name, name)){
			printf("lABEL %s FOUND\n", name);
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}
void Syntax::AddLabel(PolishItem *p, char *name)
{
	printf("ADDING NEW LABEL TO LABLE TABLE INT SYNTAX\n");
	LabelTable *find = FindLabel(name);
	if(find){
		if(find->init == true){
			printf("LABEL HAS BEEN ALREADY INIT\n");
			exit(1);
		}
		find->init = true;
		find->item = p;
		return;
	}
	LabelTable *tab = label_tab;
	LabelTable *tab_item = new LabelTable;
	tab_item -> item = p;
	tab_item -> name = name;
	tab_item->next = NULL;
	if(p != NULL){
		printf("NOT UNKNOWN\n");
		tab_item -> init = true;
	}
	else{
		printf("UNKNOWN\n");
		tab_item -> init = false;
	}	
	if (label_tab == NULL){
		label_tab = tab_item;
	}
	else{
		while(tab->next != NULL)
			tab = tab->next;
		tab->next = tab_item;	
	}
}

Error* Syntax::MakeError(const char *comment, 
			const char *exp, const char *cmd)
{
	Error* error = new Error;
	char *res;
	error -> pos = curlex -> pos;
	char *s = str_copy("ERROR: ");
	res = str_add(s, "Expected '");
	res = str_add(res, exp);
	res = str_add(res, "', but have ' ");
	res = str_add(res, curlex->value);
	res = str_add(res,  " ' ");
	res = str_add(res, cmd);
	res = str_add(res, "\n");
	res = str_add(res, comment);
	error->value = res;
	return error;
}

void Syntax::GetLex()
{
	if(curlex->next)
		curlex = curlex->next;
}

void Syntax::Analyse()
{
	//printf("Analyse-->OK!\n");
	try{
		START();
		PolishItem *tmp = polish_list;
		while(tmp != NULL){
			PolishLabel *l = dynamic_cast<PolishLabel*> (tmp->elem);
			if(l && (l->Get() == NULL)){
				LabelTable *t = FindLabel(l->GetName());
				if(t){
					l->SetAddr(t->item);
				}
				else{
					printf("CANNOT FIND ADDR FOR LABEL %s\n", l->GetName());
					exit(1);
				}
			}
			tmp = tmp->next;
		}
		//PolishItem *p = polish_list;
		//p = p->next;
		//PolishString *s = dynamic_cast<PolishString*> (p->elem);
		//printf("%s\n", s->Get());
	}
	catch (Error *e){
		printf("%s at line %d\n", e->value, e->pos);
		exit(1);
	}

}


void Syntax::START()
{
	//printf("%d", curlex->pos);
	//printf("START-->OK!\n");
	WHILE();
	LABEL();
	OPERATOR();
	if(curlex->next == NULL)
		return;
	START();
}
void Syntax::LABEL()
{
	//printf("LABEL-->OK!\n");
	//printf("%s\n", curlex->value);
	if (curlex->type == label){
		char *str = curlex->value;
		GetLex();
		if (str_cmp((curlex->value), ":")){
			PolishItem *addr = PutPolish(new PolishNoOp());//no operand2
			AddLabel(addr, str);
			GetLex();
		}
		else{
			throw MakeError(err_label, ":", " after label");
		}
	}

}

void Syntax::GOTO()
{
	///printf("GOTO-->OK!\n");
	//printf("%s\n", curlex->value);
	if (curlex->type == label){
		char *lab = curlex->value;  
		GetLex();
		if (str_cmp((curlex->value), ";")){
			LabelTable *p = FindLabel(lab);
			if(p != NULL){
				PutPolish(new PolishLabel(p->item, lab));
				PutPolish(new PolishOpGo());
			}
			else{
				AddLabel(NULL, lab);
				PutPolish(new PolishLabel(NULL, lab));
				PutPolish(new PolishOpGo());
			}
			
			GetLex();
		}
		else{
			throw MakeError(err_goto_end, ";", " after label");
		}
	}

}
void Syntax::GAME_OP()
{
	//printf("GAME_OP-->OK!\n");
	//printf("%s\n", curlex->value);
	if (str_cmp(curlex -> value, "ENDTURN")){
		char *str = curlex->value;
		GetLex();
		if (str_cmp(curlex -> value, ";")){
			GetLex();
		}
		else{
			throw MakeError(err_gameop0, ";",
					" after endturn operator");
		}
		PutPolish(new PolishFun0(str));
	}
	else if ((str_cmp(curlex -> value, "BUILD"))||
		(str_cmp(curlex -> value, "PROD"))){
		char *str = curlex->value;
		GetLex();
		EXPRESSION();
		PutPolish(new PolishFun1(str));
		if (str_cmp(curlex -> value, ";")){
			GetLex();
		}
		else{
			throw MakeError(err_gameop1, ";",
					"after operand ");
		}
	}
	else if ((str_cmp(curlex -> value, "BUY"))||
		(str_cmp(curlex -> value, "SELL"))){
		char *str = curlex->value;
		GetLex();
		EXPRESSION();
		if(str_cmp(curlex -> value, ",")){
			GetLex();
		}
		else{
			throw MakeError(err_op, ",",
					"before next operand");
		}
		EXPRESSION();
		PutPolish(new PolishFun2(str));
		if (!str_cmp(curlex -> value, ";")){
			throw MakeError(err_gameop2, ";",
					" after game operator ");
		}
		else{
			GetLex();
		}

	}
	else{
		throw MakeError(err_gameop, "game operator", "");
	}

}
void Syntax::OPERATOR()
{
	//printf("OPERATOR-->OK!\n");
	//printf("[%s]\n", curlex->value);
	if (curlex->type ==  variable){
		PutPolish(new PolishString(curlex->value));
		GetLex();
		ASSIGN();
	}
	else if (str_cmp((curlex -> value), "GOTO")){
		GetLex();
		if(curlex->type != label)
			throw MakeError(err_goto, "<label>", " after goto");
		else{
			GOTO();
		}
	}
	else if (str_cmp((curlex -> value), "IF")){
		GetLex();
		IF_ELSE();
	}
	else if (str_cmp((curlex -> value), "PRINT")){
		GetLex();
		PRINT_ELEM();
		PRINT_NEXT();
	}
	else if(curlex -> type == keyword){
		GAME_OP();
	}
	else if(str_cmp((curlex -> value), ";")){
		GetLex();
	}
	else{
		throw MakeError(err_line, "operator", "in assigment");
	}

}

void Syntax::ASSIGN()
{
	//printf("ASSIGN-->OK!\n");
	//printf("%s\n", curlex->value);
	if (str_cmp((curlex -> value), "=")){
		GetLex();
		MAS_VAL();
		if (str_cmp((curlex -> value), ";")){
			GetLex();
		}
		else{
			throw MakeError(err_assign_end,
				";", " after assigment ");
		}
		PutPolish(new PolishFunAssign());
	}
	else if (str_cmp((curlex -> value), "[")){
		/*
		GetLex();
		EXPRESSION();
		if (str_cmp((curlex -> value), "]")){
			PutPolish(new PolishFunMas());
			GetLex();
		}
		else{
			throw MakeError(err_sq_brace, "]", " in assigment ");
		}
		if (str_cmp((curlex -> value), "=")){
			GetLex();
		}
		else{
			throw MakeError(err_assign, "=", " in assigment ");
		}
		*/
		MAS_VAL();
		if (str_cmp((curlex -> value), "=")){
			GetLex();
			MAS_VAL();
			if (str_cmp((curlex -> value), ";")){
				GetLex();
			}
			else{
				throw MakeError(err_assign_end,
					";", " after assigment ");
			}
			PutPolish(new PolishFunAssign());
		}
	}
	else{
		throw MakeError(err_assign_wait, "= or [Expr]", "");
	}
}

void Syntax::MAS_VAL()
{
	//printf("MAS_VAL-->OK!\n");
	//printf("%s\n", curlex->value);
	if (str_cmp("[", curlex -> value)){
		GetLex();
		EXPRESSION();
		if (!(str_cmp((curlex -> value), "]"))){
			throw MakeError(err_sq_brace, "]",
					" in the mas elem ");
		}
		else{
			PutPolish(new PolishFunMas());
			GetLex();
		}
	}
	else{
		EXPRESSION();
	}

}


void Syntax::PRINT_ELEM()
{
	//printf("P_E-->OK!\n");
	//printf("%s\n", curlex->value);
	if (curlex -> type == str){
		PutPolish(new PolishString(curlex->value));
		GetLex();
	}
	else{
		EXPRESSION();
	}

}

void Syntax::PRINT_NEXT()
{
	//printf("P_N-->OK!\n");
	if (str_cmp(curlex -> value, ",")){
		GetLex();
		PRINT_ELEM();
		PRINT_NEXT();
	}
	else if (str_cmp(curlex -> value, ";")){
		GetLex();
	}
	else {
		throw MakeError(err_print, "; or ,", " in print ");
	}

}

void Syntax::EXPRESSION()
{
	//printf("EXPRESSION-->OK!\n");
	//printf("%s\n", curlex->value);
	L1();
	R1();
}

void Syntax::R1()
{
	if(str_cmp(curlex -> value, "OR")){
		GetLex();
		EXPRESSION();
		PutPolish(new PolishFunOR);
	}
	else if(str_cmp(curlex -> value, "AND")){
		GetLex();
		EXPRESSION();
		PutPolish(new PolishFunAND);
	}
}
void Syntax::L1()
{
	L2();
	R2();
}
void Syntax::R2()
{
	//printf("R2-->OK! pos:");
	//printf("%d\n", curlex->pos);
	//printf("%s\n", curlex->value);
	char *str = curlex->value;
	if ((str_cmp(curlex -> value, "<"))||
		(str_cmp(curlex -> value, ">"))||
		(str_cmp(curlex -> value, "!="))||
		(str_cmp(curlex -> value, ">="))||
		(str_cmp(curlex -> value, "<="))||
		(str_cmp(curlex -> value, "=="))){
		GetLex();
		L2();
		R2();
		PutPolish(new PolishFunComp(str));
	}

}

void Syntax::L2()
{
	UNARY_MULT_DIV();
	ADD_SUB();
}

void Syntax::ADD_SUB()
{
	//printf("ADD_SUB-->OK!\n");
	//printf("%s\n", curlex->value);
	if (str_cmp(curlex -> value, "+")){
		GetLex();
		UNARY_MULT_DIV();
		ADD_SUB();
		PutPolish(new PolishFunAdd);
	}
	else if(str_cmp(curlex -> value, "-")){
		GetLex();
		UNARY_MULT_DIV();
		ADD_SUB();
		PutPolish(new PolishFunSub);
	}
}
void Syntax::UNARY_MULT_DIV()
{
	UNARY();
	MULT_DIV();
}

void Syntax::TERM()
{
	//printf("TERM-->OK!\n");
	//printf("%s\n", curlex->value);
	if (str_cmp(curlex -> value, "(")){
		GetLex();
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, ")",
					" need to close expr ");
	}
	else if (curlex -> type == num){
		PutPolish(new PolishInt(str_to_int(curlex->value)));
		GetLex();

	}
	else if(curlex -> type == function){
		FUNC();
		//PutPolish(new PolishString(curlex->value));
	}
	else if (curlex -> type == variable){
		PutPolish(new PolishString(curlex->value));
		GetLex();
		if (str_cmp(curlex -> value, "[")){
			GetLex();
			EXPRESSION();
			if (str_cmp(curlex -> value, "]")){
				GetLex();
			}
			else{
				throw MakeError(err_sq_brace,"]",
						"need to close [");
			}
			PutPolish(new PolishFunMas);
		}
		PutPolish(new PolishFunGetVar);
	}
	else{
		throw MakeError(err_term,
				"variable, num, function or exp",
				curlex->value);
	}

}

void Syntax::FUNC()
{
	//char *str = curlex->value;
	if(str_cmp(curlex -> value, "?production_price") 
		|| str_cmp(curlex -> value, "?my_id") 
		|| str_cmp(curlex -> value, "?turn") 
		|| str_cmp(curlex -> value, "?players")
		|| str_cmp(curlex -> value, "?active_players")
		|| str_cmp(curlex -> value, "?supply")
		|| str_cmp(curlex -> value, "?raw_price")
		|| str_cmp(curlex -> value, "?demand")){
		GetLex();
	}
	else if(str_cmp(curlex -> value, "?money")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?money' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?money' ");

	}
	else if(str_cmp(curlex -> value, "?raw")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?raw' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?raw' ");
	}
	else if(str_cmp(curlex -> value, "?production")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?production' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?production' ");
	}
	else if(str_cmp(curlex -> value, "?factories")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?factories' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?factories' ");
	}
	else if(str_cmp(curlex -> value, "?auto_factories")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?auto_factories' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?auto_factories' ");
	}
	else if(str_cmp(curlex -> value, "?manufactered")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?manufactered' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?manufactered' ");
	}
	else if(str_cmp(curlex -> value, "?result_raw_sold")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?result_raw_sold' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?result_raw_sold' ");
	}
	else if(str_cmp(curlex -> value, "?result_raw_price")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?result_raw_price' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?result_raw_price' ");
	}
	else if(str_cmp(curlex -> value, "?result_prod_bought")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?result_prod_bought' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?result_prod_bought' ");
	}
	else if(str_cmp(curlex -> value, "?result_prod_price")){
		GetLex();
		if (str_cmp(curlex -> value, "("))
			GetLex();
		else
			throw MakeError(err_round_brace_1, "(",
				" in function '?result_prod_price' ");
		EXPRESSION();
		if (str_cmp(curlex -> value, ")"))
			GetLex();
		else
			throw MakeError(err_round_brace_2, "(",
				" in function '?result_prod_price' ");
	}
	else {
		throw MakeError(err_no_func,
			"function my_id/turn or others", "");
	}
	PutPolish(new PolishInt(117));
}
void Syntax::MULT_DIV()
{
	//printf("MUL_DIV-->OK!\n");
	//printf("%s\n", curlex->value);
	if (str_cmp(curlex -> value, "*")){
		GetLex();
		UNARY();
		MULT_DIV();
		PutPolish(new PolishFunMult);
	}
	else if(str_cmp(curlex -> value, "/")){
		GetLex();
		UNARY();
		MULT_DIV();
		PutPolish(new PolishFunDiv);
	}
	else if(str_cmp(curlex -> value, "%")){
		GetLex();
		UNARY();
		MULT_DIV();
		PutPolish(new PolishFunModulo);
	}
}
void Syntax::UNARY()
{
	//printf("UNARY-->OK!\n");
	//printf("%s\n", curlex->value);
	if(str_cmp(curlex -> value, "-")){

		GetLex();
		TERM();
		PutPolish(new PolishFunNeg);

	}
	else if(str_cmp(curlex -> value, "!")){

		GetLex();
		TERM();
		PutPolish(new PolishFunNOT);
	}
	else{
		TERM();
	}

}


void Syntax::WHILE()
{ 
	//printf("%s\n", curlex->value);
	if(str_cmp(curlex->value, "WHILE")){
		GetLex();
		PolishItem  *noop1 = PutPolish(new PolishNoOp());//noop1
		EXPRESSION();
		PolishLabel *l1 = new PolishLabel(NULL, "empty1");
		PutPolish(l1);// empty label 1
		PutPolish(new PolishOpGoElse());// !F
		if(str_cmp(curlex->value, "DO")){
			GetLex();
			BODY_WHILE();

			PutPolish(new PolishLabel(noop1, "empty2"));//empty label 2
			PutPolish(new PolishOpGo());// !

			PolishItem  *noop2 = PutPolish(new PolishNoOp());//noop2
			l1->SetAddr(noop2);
		}
		else{
			throw MakeError(err_while_do, "DO", " in loop WHILE");
		}
	}
}
void Syntax::BODY_WHILE()
{
	//printf("BODY_WHILE-->OK!\n");
	//printf("%s\n", curlex->value);
	WHILE();
	LABEL();
	OPERATOR();
	if(str_cmp((curlex -> value), "END")){
		GetLex();
		if(str_cmp((curlex -> value), "WHILE")){
			GetLex();
		}
		else{
			throw MakeError(err_while_end,
				"END WHILE",
				" after WHILE <expr> DO <body>");
		}
	}
	else{
		if(curlex->next == NULL){
			throw MakeError(err_while_end,
				"END WHILE",
				" after WHILE <expr> DO <body>");
		}
		BODY_WHILE();
	}

}


void Syntax::IF_ELSE()
{

	//printf("IF_ELSE-->OK!\n");
	//printf("%s\n", curlex->value);
	EXPRESSION();
	PolishLabel *l1 = new PolishLabel(NULL, "empty1");
	PutPolish(l1);// empty label 1
	PutPolish(new PolishOpGoElse());// !F
	if (str_cmp((curlex -> value), "THEN")){
		GetLex();
	}
	else{
		throw MakeError(err_if, "THEN"," after IF ");
	}
	BODY_IF();
	PolishLabel *l2 = new PolishLabel(NULL, "empty2");
	PutPolish(l2);// empty label 2
	PutPolish(new PolishOpGo());// !F

	PolishItem *noop1 = PutPolish(new PolishNoOp());//noop 1
	l1->SetAddr(noop1);// fill lab. 1 
	BODY_ELSE();

	PolishItem *noop2 = PutPolish(new PolishNoOp());//noop2
	l2->SetAddr(noop2);// fill lab. 2
}
void Syntax::BODY_IF()
{
	WHILE();
	LABEL();
	OPERATOR();

	if(str_cmp((curlex -> value), "ELSE")){
		GetLex();
	}
	else{
		if(curlex->next == NULL){
			throw MakeError(err_else,
				"ELSE",
				" after IF <expression> THEN");
		}
		BODY_IF();
	}
}

void Syntax::BODY_ELSE()
{
	WHILE();
	LABEL();
	OPERATOR();
	if(str_cmp((curlex -> value), "END")){
		GetLex();
		if(str_cmp((curlex -> value), "IF")){
			GetLex();
		}
		else{
			throw MakeError(err_endif,
				"END IF",
				" after IF <body> ELSE <body>");
		}
	}
	else{
		if(curlex->next == NULL){
			throw MakeError(err_endif,
				"END IF",
				" after IF <body> ELSE <body>");
		}
		BODY_ELSE();
	}
}

class Environment{
	PolishItem **stack;
	PolishItem *polish_cur;
	PolishItem *polish_list;
	IntTable *int_tab;
	IntTable *int_tab_cur;
public:
	Environment(PolishItem* p)
	{
		polish_list = p;
		polish_cur = p;
		stack = new (PolishItem*);
		(*stack) = NULL;
		int_tab = NULL;

	}
	void Run();
	void PutPolish(PolishElem *p);
	void PrintIntTab()
	{
		printf("******INTEGER TABLE******\n");
		IntTable *tmp = int_tab;
		while(tmp){
			printf("int %s = %d\n", tmp->name, tmp->value);
			tmp = tmp->next;
		}
		printf("*************************\n");

	}
	PolishItem* GetPolish() {return polish_list;}
};

void Environment::Run()
{

	PolishItem *p = polish_list;
	try{
		if(p == NULL){
			throw ">POLISH EVALUATE: Empty Polish List\n";
		}
		while (p != NULL){
			if(!p->elem){
				throw "empty Polish Item Elem\n";
			}
			p->elem->Evaluate(stack, &p, &int_tab);
			PrintIntTab();
		}

		if(*stack != NULL){
			PolishString *i1 = dynamic_cast<PolishString*> 
							((*stack)->elem);
			if(i1)
				printf("%s\n", i1->Get());
			else{
				PolishInt *i2 = dynamic_cast<PolishInt*> 
							((*stack)->elem);
				printf("%d\n", i2->Get());
			}
			throw "STACK IS NOT EMPTY BUT EVALUATION ENDED\n";
		};
	} 
	catch (const char *str)
	{
		printf("%s", str);
		exit(0);
	}
}

int main(int argc, char **argv)
{
	FILE *fd;
	Lexic lexic;
	char c;
	if(argc != 2){
		printf(err_cmdline);
		exit(1);
	}
	fd = fopen(argv[1], "r");
	if(!fd){
		perror("fopen");
		exit(1);
	}
	while((c  = fgetc(fd))!= EOF){
		lexic.Analyse(c);
	}
	fclose(fd);
	if(!lexic.AskForState()){
		printf("Lexical error in line %d\n", lexic.AskForLine());
		exit(1);
	}
	//lexic.Print();
	printf("LEXICAL ANALYSE SUCCESSED\n");
	printf("=========================\n");
	Syntax syntax(lexic.GetList());
	syntax.Analyse();
	printf("=========================\n");
	//PolishItem *p = syntax.GetPolish();
	//p = p->next;
	//PolishString *s = dynamic_cast<PolishString*> (p->elem);
	//("%s\n", s->Get());
	Environment env(syntax.GetPolish());
	env.Run();

	return 0;
}
