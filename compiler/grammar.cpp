#include "stdafx.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include <string>
using namespace std;
void sub_program();
void defconsts_part();
void defvars_part();
void defproce();
void deffunc();
void combosentence();
void constnum();
void defvars();
void procedurehead();
void formal_para_chart();
void formal_para_section();
void functionhead();
void sentence();
void expr();
void rela_operator();
void becomesentence();
void callproceduresentence(bool para);
void real_para_chart(arg* p1);
void term();
void factor();
void callfunction(arg* p1,bool para);
int look_up(char name_in[], int obj_in,int searchtype);


extern void equal(arg* save, arg* give);
extern arg* newlab();
extern arg* newplace();
extern arg* bdarg(char name[], int inde, int type);
extern void emit(char op[], arg* name1, arg* name2, arg* name3, int level);
extern arg* genarg();
extern arg* new_profunclab(int inde);
extern arg* genfuncprolab(int i);

extern int lab;
arrayinfo atab[200];//数组表
int atabindex = 0;//数组表从1开始算
btabFrame btab[200];//btab
int tabindex = 1;//符号表指针 从1开始
int btabindex = 1;//分程序表指针 从1开始
int display[200] = { 0 };//分程序索引表
//int displayindex = 1;//分程序索引表指针 从1开始
info tab[200];//符号表 由info组成的结构组
int last= 0;//最后一个标识符
int lpar = 0;//最后一个参数
int lev = 1;//层次从1开始
int varnum = 0;//统计tab表中输入了当前分程序的几个iden
int localadr = 0;
char id[30];//用于临时存储
int tbt[200] = { 0 };

int obj;
int typ;
int normal;
int ref = 0;
int adr = 0;

extern int number;
int tempnum=0;//var一次声明几个
int temptab = 0;//暂时记录函数在tab中的下标

char op[30];
arg* var1 = genarg();
arg* var2 = genarg();
arg* var3 = genarg();
arg* var4=genarg();
char funcproname[30];

int stringnum = 0;
int strlength[100] = { 0 };
int strinum[100] = { 0 };
char str[1000];
int paranum = 0;
//int paranums=0;//统计参数个数的临时变量
//char tempname[30];//临时寄存名字
extern int symbol;//最后一个读到的symbol 
extern void getsym();
extern void error(int error_num);
extern char tokenBuffer[82];//读到的字符串 最大为81

void insert_temp_var(char name_in[])
{
	tab[tabindex].ref = 0;
	tab[tabindex].adr = 0;
	//index为指向符号表的栈顶指针
	if (tabindex > 199)
	{
		error(4);//符号表越界
		return;
	}

	
	if (tab[btab[display[lev]].lastvar].obj == parakind || tab[btab[display[lev]].lastvar].obj == varkind || tab[btab[display[lev]].lastvar].obj==tempvar)
	{
		if (tab[btab[display[lev]].lastvar].obj == parakind)
		{
			tab[tabindex].adr =0;
		}
		else if (tab[btab[display[lev]].lastvar].typ != arrayint && tab[btab[display[lev]].lastvar].typ != arraychar)
			tab[tabindex].adr = tab[btab[display[lev]].lastvar].adr + 4;
		else
			tab[tabindex].adr = atab[tab[btab[display[lev]].lastvar].ref].length * 4 + tab[btab[display[lev]].lastvar].adr;
		tab[tabindex].link = btab[display[lev]].lastvar;//也连接了其他的玩意儿！
	}
	else{
		tab[tabindex].adr = 0;
		tab[tabindex].link = btab[display[lev]].lastvar;//
	}
	btab[display[lev]].last = tabindex;
	btab[display[lev]].lastvar = tabindex;
	//varnum++;
	//localadr = localadr + 4;


	strcpy_s(tab[tabindex].name, name_in);
	//*********tab[tabindex].link =//*********
	tab[tabindex].obj = tempvar;
	//[tabindex].typ = typ_in;
	//***********tab[tabindex].ref
	tab[tabindex].lev = lev;
	tab[tabindex].normal = 1;
	tabindex++;

}
////符号表设计：填写符号表 符号表最多装200个
void insertTab(char name_in[], int obj_in, int typ_in, int normal_in)
{
	tab[tabindex].ref = 0;
	tab[tabindex].adr = 0;
	//index为指向符号表的栈顶指针
	if (tabindex > 199)
	{
		error(4);//符号表越界
		return;
	}
	/*
	
	*/
	//如果是从main开始的第一个
	if (tabindex == 1){
		tab[tabindex].link = 0;
		//新建btab表第一项
		btab[btabindex].last = 0;
		btab[btabindex].lpar = 0;
		btab[btabindex].psze = 0;
		btab[btabindex].vsze = 0;
		btab[btabindex].lastvar = 0;
		display[1] = 1;//level[1]现在登记在btab表的第一项
	}
	if (obj_in == funckind || obj_in == prokind)//如果是function或者是procedure
	{//检查有无重名 跟所有历来的pro和func的名字相比
		if (look_up(name_in, obj_in,1) > 0)//找到了 返回tab所在地
		{
			error(5);//重复定义
			return;
		}
		else if (look_up(name_in, obj_in,1) == 0)//没有找到 可以填表
		{
			int i = lev; 
				btab[display[i]].last = tabindex;

//			btab[btabindex].last = tabindex;
			btabindex++;
			tbt[tabindex] = btabindex;
			btab[btabindex].last = 0;
			btab[btabindex].lpar = 0;
			btab[btabindex].psze = 0;
			btab[btabindex].vsze = 0;
			btab[btabindex].lastvar = 0;
			varnum = 0;
			localadr = 0;
			tab[tabindex].link = tabindex - 1;
			tab[tabindex].ref = btabindex;
			paranum = 0;
		}
	}
	else//参数或是变量或是常量
	{
		if (obj_in == parakind )//是参数可以建
		{
			if (varnum == 0){
				tab[tabindex].link = 0;
				//display[lev] = btabindex;
				//tab[tabindex].adr = 0;
			}
			else{
				//tab[tabindex].adr = localadr;
				tab[tabindex].link = tabindex - 1;

			}
			btab[display[lev]].lpar = tabindex;
			paranum++;
			btab[display[lev]].last = tabindex;
			if (obj_in != constkind)
			btab[display[lev]].lastvar=tabindex;
			varnum++;
			//if (obj_in != constkind)
			//	localadr = localadr + 4;
		}
		else if (look_up(name_in, 0, 1) == 0 )//查表 没查到 可以填表 不是参数
		{
			if (varnum == 0){
				tab[tabindex].link = 0;
				//display[lev] = btabindex;
				//if (obj_in != constkind)
					tab[tabindex].adr = 0;

			}
			else{
				//if(obj_in != constkind)
					tab[tabindex].adr = localadr;
				tab[tabindex].link = tabindex - 1;
			}
			if (obj_in==varkind)
				btab[display[lev]].lastvar = tabindex;
			btab[display[lev]].last = tabindex;

			varnum++;
			if (obj_in != constkind)
				localadr=localadr+4;
			/*if (typ_in == arraykind){
				//atabindex++;
				tab[tabindex].ref = atabindex;
			}*/
		}
		else if (look_up(name_in, 0,1) >0)//查到了(返回了符号表所在地址)
		{
			error(5);//重复定义
			return;
		}

	}
	strcpy_s(tab[tabindex].name, name_in);
	//*********tab[tabindex].link =//*********
	tab[tabindex].obj = obj_in;
	tab[tabindex].typ = typ_in;
	//***********tab[tabindex].ref
	tab[tabindex].lev = lev;
	tab[tabindex].normal = normal_in;
	tabindex++;
}


//查表函数
void insertArray(int i,int type,int length){
	atabindex++;
	tab[i].ref = atabindex;//i是tabindex
	atab[atabindex].eltyp = type;//数组元素类型
	atab[atabindex].length = length;//数组长度
	localadr = localadr + 4 * (length-1);
}

int look_up(char name_in[],int obj_in,int searchtype)//isproc为1表示为查找的是procedure或者是function
//searchtype 为1 建表时的查找 为0 查找标识符时
{
	int a, b;
	b = 0;
	if (obj_in == prokind || obj_in==funckind){//是procedure 或者是 function
		for (a = tabindex - 1; a >= 1; a--){
			if (strcmp(tab[a].name, name_in) == 0){
				break;
			}
		}
		if (a ==0){//没找到
			if (searchtype==0){
				error(7);//没找到这个函数或是这个过程 过程函数 未定义
				printf("%s\n", name_in);
			}
			return 0;
		}
		return a;//查找成功就返回该pro或是func在符号表中的位置
	}
	else{//查找到的是个常量或者变量
		//先在当前层超找
		int level = lev;
		for (; level >= 1; level--){
			a = display[level];
			if (btab[a].last == 0)
				continue;
			for (b = btab[a].last;; b--){
				if (strcmp(tab[b].name, name_in) == 0)//找到了
					return b;
				if (tab[b].link == 0)
					break;
			}
		}
		if (searchtype == 0){
			error(8);
			printf("%s\n", name_in);
		}
		return 0;//没找到
	}
}

int look_tab(char name_in[]){//函数 过程 和标识符不能重名
	int a, b;
	b = 0;
		int level = lev;
		for (; level >= 1; level--){
			a = display[level];
			if (btab[a].last == 0)
				continue;
			for (b = btab[a].last;; b--){
				if (strcmp(tab[b].name, name_in) == 0 && tab[b].lev==level)//找到了
					return b;
				if (tab[b].link == 0 && tab[b].lev==level)
					break;
			}
		}
			error(8);
			printf("%s\n", name_in);
		return 0;//没找到
}

//添加kind种类
void insertkind(int type,int index)
{
	tab[index].typ =type;
	return;
}
//添加数组大小



//<程序>                   :: =   <分程序>.
void main_program()
{
	getsym();
	lev = 1;
	sub_program();
	lev--;
	//最后看是否为.
	if (symbol == PERIOD)
		cout << "It is a legal program.这是个合法的完整程序" << endl;
	else
		error(23);//缺少.
	//TODO!!!!!!!!!!!最后最好再检查一下还有没有字符。.后面不许有字符
}

//< 分程序>               :: = [<常量说明部分>][<变量说明部分>]{[<过程说明部分>] | [<函数说明部分>]}<复合语句>
//最后不多读 复合语句多读
void sub_program()
{
	if (symbol == CONSTSYM)
	{
		//常量说明部分
		getsym();
		defconsts_part();

	}
	if (symbol==VARSYM)
	{
		//变量说明部分
		getsym();
		defvars_part();
	}

	while (symbol == PROSYM || symbol==FUNCSYM )//设置标志 LAB id；
	{
		normal = 1;
		//过程说明部分或者是函数说明部分
		if (symbol == PROSYM)
		{
			obj = prokind;
			//过程说明部分
			getsym();
			defproce();
			
		}
		else 
		{
			//函数说明部分
			obj = funckind;
			getsym();
			deffunc();
		}

	}
	if (symbol == BEGINSYM)
	{
		//是复合语句
		if (lev - 1 > 0){

			arg *lab1 = new_profunclab(btab[display[lev - 1]].last);//
			//addr应为lab-1;
			tab[btab[display[lev - 1]].last].adr = lab - 1;//lab标号
			emit((char*)"LAB", lab1, NULL, NULL, lev);
		}
		else{
			arg *newarg = (arg *)malloc(sizeof(arg));
			strcpy_s(newarg->word, "entry");
			newarg->type = entrylab;
			newarg->ident = 0;
		emit((char*)"LAB", newarg, NULL, NULL, lev);
		}
		getsym();
		combosentence();
	}
	else
	{
		error(22);//没有复合语句 语法错误
		getsym();
	}
	//lev--;
	cout << "It is a sub procedure.这是个分程序"<<endl;
	

}

//< 常量说明部分>  ::=   const<常量定义>{,<常量定义>};
//< 常量定义>           :: = <标识符>＝ <常量>
//多读了一个
void defconsts_part()
{
	obj = constkind;
	if (symbol == IDEN)
	{
		strcpy_s(id, tokenBuffer);
		getsym();
		if (symbol == EQL)
		{
			getsym();
			constnum();			
		}
		else{
			error(10);//肯定是等号
			while (symbol != COMMA && symbol != SEMICOLON) { /////跳读
				if (symbol == EOFSYM) {
					printf("EOFSYM!\n");
					return;
				}
				getsym();
			}
			return;
		}
			
		cout << "It is a constnumber definition.这是个常量定义" << endl;
	}
	else
	{
		error(11);
		while (symbol != COMMA && symbol != SEMICOLON) { /////跳读
			if (symbol == EOFSYM) {
				printf("EOFSYM!\n");
				return;
			}
			getsym();
		}
		return;
	}
	while (symbol == COMMA)
	{
		getsym();
		if (symbol == IDEN)
		{
			strcpy_s(id, tokenBuffer);
			getsym();
			if (symbol == EQL)
			{
				getsym();
				constnum();

			}
			else
			{
				error(10);//肯定是等号
				while (symbol != COMMA && symbol != SEMICOLON) { /////跳读
					if (symbol == EOFSYM) {
						printf("EOFSYM!\n");
						return;
					}
					getsym();
				}
				cout << "It is a constnumber definition.这是个常量定义" << endl;
				getsym();
			}
		}
		
	}
	if (symbol == SEMICOLON)
	{		
		getsym();
	}
	else
	{
		error(15);//缺少分号
		while (symbol != VARSYM && symbol !=PROSYM && symbol !=FUNCSYM && symbol != BEGINSYM && symbol!=CONSTSYM){////////缺分号时的跳读，即下一个明确的合法后继
			if (symbol == EOFSYM) { printf("EOF!\n"); return; }
			getsym();
		}
		return;
	}
	cout << "It is a consts definition part.这是个常量说明部分" << endl;

}

//< 常量>   ::=    [+| -] <无符号整数>|<字符>
//多读了一个
void constnum()
{
	if (symbol == PLUS || symbol == MINUS || symbol == NUMBER)
	{
		if (symbol == PLUS)
		{
			getsym();
			if (symbol == NUMBER)
			{
				//是带加号的整数耶！
				cout << "It is a positive number.这是个正数" << endl;
				typ = intkind;
				insertTab(id, obj, typ, 0);
				
				//***未完
				//_itoa_s(number, var2, 10, 10);
				tab[tabindex - 1].adr = number;
				//emit((char*)"ASG",id,var2,(char*)"",lev);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!存表不写四元式
			}
			else
				error(29);//加号后面接数字

		}
		else if (symbol == MINUS)
		{
			getsym();
			if (symbol == NUMBER)
			{
				//是负数！
				cout << "It is a negative number.这是个负数" << endl;
				typ = intkind;
				insertTab(id, obj, typ, 0);
				number = 0 - number;
				tab[tabindex - 1].adr = number;
				//_itoa_s(number, var2, 10, 10);
				//emit((char*)"ASG", id, var2, (char*)"",lev);////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!存表不写四元式
				//***未完
			}
			else
				error(29);//减号后面接数字
		}
		else
		{
			cout << "It is a number.这是个数字（正）" << endl;
			typ = intkind;
			insertTab(id, obj, typ, 0);
			//_itoa_s(number, var2, 10, 10);
			tab[tabindex - 1].adr = number;
			//emit((char*)"ASG", id, var2, (char*)"",lev);////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!存表不写四元式
			//***未完
		}
		getsym();
	}
	else if (symbol == APOSCHARAC)
	{
		//是字符
		cout << "It is a Apos Chracter.这是个字符" << endl;
		typ = charkind;
		insertTab(id, obj, typ, 0);
		tab[tabindex - 1].adr = (int)tokenBuffer[1];//转换成ascii
		//emit((char*)"ASG", id,tokenBuffer, (char*)"",lev);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!存表不写四元式
		//***未完
		getsym();
	}
	else
	{
		error(14);//就是非法字符 赋值不合法
		while (symbol != COMMA && symbol != SEMICOLON) { /////跳读
			if (symbol == EOFSYM) {
				printf("EOFSYM!\n");
				return;
			}
			getsym();
		}
		return;
	}
}



//< 变量说明部分>  :: = var <变量说明>; {<变量说明>; }
//多读了一个
void defvars_part()
{
	obj = varkind;
	tempnum = 0;
	normal = 1;
	if (symbol == IDEN)
	{
		strcpy_s(id, tokenBuffer);
		normal = 1;//都传值
		insertTab(id, obj,typ,normal);
		tempnum++;
		//未完
		getsym();
		defvars();
	}
	else
	{
		error(28);//必须是标识符
		while (symbol != SEMICOLON) {
			if (symbol == EOFSYM) { printf("EOF!\n"); return; }
			getsym();
		}
		getsym();
	}
	if (symbol == SEMICOLON)
	{
		getsym();
	}
	else
	{
		error(15);//缺少分号 这种类型不往后面读了！！！！！！！！！！！！！！
		while (symbol != IDEN && symbol != VARSYM && symbol != FUNCSYM && symbol!=PROSYM && symbol!=BEGINSYM) {
			if (symbol == EOFSYM) { printf("EOF!\n"); return; }
			getsym();
		}
	}
	while (symbol==IDEN)
	{
		strcpy_s(id, tokenBuffer);
		normal = 1;//都传值
		insertTab(id, obj, typ, normal);
		tempnum++;
		getsym();
		defvars();
		if (symbol == SEMICOLON)
		{
			getsym();
		}
		else
		{
			error(15);//缺少分号 这种类型不往后面读了！！！！！！！！！！！！！！
		}
	}
	cout << "It is a variables definiton part.这是个变量说明部分" << endl;

	
}

//< 变量说明>           :: = <标识符>{, <标识符>} : <类型>
//多读了一个
void defvars()
{
	while (symbol == COMMA)
	{
		
		getsym();
		if (symbol == IDEN)
		{
			strcpy_s(id, tokenBuffer);
			normal = 1;//都传值
			insertTab(id, obj, typ, normal);
			tempnum++;
			//未完
			getsym();
		}
		else
		{
			error(0);//逗号后面必定是标识符
			while (symbol != SEMICOLON && symbol!=COMMA) {
				if (symbol == EOFSYM) return;
				getsym();
			}
			if (symbol == SEMICOLON){
				tempnum = 0;
				return;
			}
				
		}
	}
	if (symbol == COLON)
	{
		getsym();
	}
	else
	{
		error(25);//必须是冒号
		getsym();
	}
	if (symbol == INTSYM || symbol == CHARSYM || symbol == ARRAYSYM)
	{
		if (symbol == INTSYM)
		{
			for (; tempnum > 0; tempnum--)
			{
				insertkind(intkind, tabindex - tempnum);
				
			}
			tempnum = 0;
			getsym();
		}
		else if (symbol == CHARSYM)
		{
			for (; tempnum > 0; tempnum--)
			{
				insertkind(charkind, tabindex - tempnum);

			}
			tempnum = 0;
			getsym();
		}
		else//数组
		{
			int num;
			int eltype;
			getsym();
			if (symbol == LBRACKET)
			{
				getsym();
			}
			else
			{
				error(17);//必须是左中括号
				getsym();
			}
			if (symbol == NUMBER)
			{
				num = number;
				getsym();//数组容量
			}
			else
			{
				error(29);//必须是无符号整数
				getsym();
			}
			if (symbol == RBRACKET)
			{
				getsym();
			}
			else
			{
				error(17);//必须是右中括号
				getsym();
			}
			if (symbol == OFSYM)
			{
				getsym();
			}
			else
			{
				error(24);//必须是of
				getsym();
			}
			if (symbol == INTSYM || symbol == CHARSYM)
			{
				if (symbol == INTSYM)
				{
					//整型数组
					eltype = intkind;
					for (int i=tempnum; i>0; i--)
					{
						insertkind(arrayint, tabindex - i);
					}
					for (int i = tempnum; i > 0; i--){
						insertArray(tabindex - i, eltype, num);
					}
					tempnum = 0;
					getsym();
				}
				else
				{
					//字符型数组
					eltype = charkind;
					for (int i = tempnum; i>0; i--)
					{
						insertkind(arraychar, tabindex - i);
					}
					for (int i = tempnum; i > 0; i--){
						insertArray(tabindex - i, eltype, num);
					}
					tempnum = 0;
					getsym();
				}
			}
			else
			{
				error(16);//只有这两种类型的数组
				getsym();
			}
		}
	}
	else
	{
		error(16);//就是语法错了嘛
		getsym();
	}
	cout << "It is a variables definition.这是个类型说明";
}

//< 过程说明部分>  :: = <过程首部><分程序>{; <过程首部><分程序>};
//多读了一个
void defproce()
{
	procedurehead();
	
	sub_program();
	
	lev--;
	arg *newarg = (arg *)malloc(sizeof(arg));
	strcpy_s(newarg->word, (char*)tab[btab[display[lev]].last].name);
	newarg->ident = btab[display[lev]].last;
	newarg->type = retarg;
	emit((char*)"RET",newarg, NULL, NULL, lev+1);
	//clean_table_proc();
	while (symbol == SEMICOLON)
	{
		getsym();
		if (symbol != PROSYM)
			return;
		obj = prokind;
		getsym();
		procedurehead();
		sub_program();
		lev--;
		arg *newarg1 = (arg *)malloc(sizeof(arg));
		strcpy_s(newarg1->word, (char*)tab[btab[display[lev]].last].name);
		newarg1->ident = btab[display[lev]].last;
		newarg1->type = retarg;
		emit((char*)"RET", newarg1, NULL, NULL, lev + 1);
		//clean_table_proc();
	}
	if (symbol == SEMICOLON)
	{
		getsym();
	}
	else
	{
		error(15);//缺少分号！！！！！！！！！！！！！！！！
	}
	cout << "This is a procedure difenition.这是一个过程说明部分" << endl;
}

//< 过程首部>           ::=   procedure<标识符>[<形式参数表>];
//多读了一个
void procedurehead()
{
	if (symbol == IDEN)
	{
		strcpy_s(id, tokenBuffer);
		
		strcpy_s(funcproname, tokenBuffer);
		insertTab(id, obj,notype,normal);
		
		getsym();
		lev++;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		display[lev] = btabindex;
		if (symbol == LPAREN)
		{
			formal_para_chart();
		}
		btab[display[lev]].psze = 4 * paranum;
		//返填adr
		for (int i = 1; i <= paranum;i++)
		{
			tab[tabindex - i].adr = (lev - 1 + 2 + i) * 4 * (-1);
		}
		paranum = 0;
		if (symbol == SEMICOLON)
		{
			getsym();
		}
		else
		{
			error(15);//缺少分号！！！！！！！！！！！！！！！！！！
		}
	}
	else
	{
		error(28);//此处需要是标识符
		getsym();
	}
	cout << "This is a proedure head.这是一个过程首部" << endl;
}

//< 形式参数表>       ::= '('<形式参数段>{; <形式参数段>}')'
//多读了一个
void formal_para_chart()
{
	tempnum = 0;
	obj = parakind;
	if (symbol == LPAREN)
	{
		getsym();
		formal_para_section();
	}
	else
	{
		error(18);//必须是左括号
		getsym();
	}
	while (symbol == SEMICOLON)
	{
		getsym();
		formal_para_section();
	}
	if (symbol == RPAREN)
	{
		getsym();
	}
	
	else
	{
		error(18);//此处必须是）
		getsym();
	}
	cout << "It is a formal parameters chart.这是个形式参数表" << endl;
}

//< 形式参数段>       :: = [var]<标识符>{, <标识符>}: <基本类型>
//多读了一个
void formal_para_section()
{
	tempnum = 0;
	if (symbol == VARSYM){
		normal = 0;
		getsym();
	}
	else
		normal = 1;	
	if (symbol == IDEN){
		tempnum = 0;//////////////////////////////////////////////////////////

		strcpy_s(id, tokenBuffer);
		insertTab(id, obj, typ, normal);
		tempnum++;
		getsym();
	}
	else
	{
		error(28);//此处应该是个标识符
		getsym();

	}
	while (symbol == COMMA)
	{
		getsym();
		if (symbol == IDEN)
		{
			strcpy_s(id, tokenBuffer);
			insertTab(id, obj, typ, normal);
			tempnum++;
			getsym();
		}
		else
		{
			error(28);//此处应该是标识符
			getsym();
		}
	}
	if (symbol == COLON)
	{
		getsym();
		if (symbol == INTSYM || symbol == CHARSYM)
		{
			if (symbol == INTSYM) //int类型
			{
				for (; tempnum > 0; tempnum--)
				{
					insertkind(intkind, tabindex - tempnum);

				}
				tempnum = 0;
				getsym();
			}
			else //char类型
			{
				for (; tempnum > 0; tempnum--)
				{
					insertkind(charkind, tabindex - tempnum);

				}
				tempnum = 0;
				getsym();
			}
		}
		else
		{
			error(16);//只能是int或者是char
			getsym();
		}
	}
	else
	{
		error(25);//此处应该是冒号
		getsym();
	}
	cout << "It is a formal parameters section.这是形式参数段" << endl;
}

//< 函数说明部分>  :: =  <函数首部><分程序>{; <函数首部><分程序>};
void deffunc()
{
	functionhead();
	sub_program();
	lev--;
	arg *newarg = (arg *)malloc(sizeof(arg));
	strcpy_s(newarg->word, (char*)tab[btab[display[lev]].last].name);
	newarg->ident = btab[display[lev]].last;
	newarg->type = retarg;
	emit((char*)"RET", newarg,NULL, NULL, lev + 1);
	//clean_table_proc();
	while (symbol == SEMICOLON)
	{
		getsym();
		if (symbol != FUNCSYM)
			return;
		obj = funckind;
		getsym();
		functionhead();
		sub_program();
		lev--;

		arg *newarg1 = (arg *)malloc(sizeof(arg));
		strcpy_s(newarg1->word, (char*)tab[btab[display[lev]].last].name);
		newarg1->ident = btab[display[lev]].last;
		newarg1->type = retarg;

		emit((char*)"RET", newarg1, NULL, NULL, lev + 1);
		//clean_table_proc();
	}
	if (symbol == SEMICOLON)
	{
		getsym();
	}
	else
	{
		error(15);//缺少分号！！！！！！！！！！！！！！！！
	}
	cout << "This is a function difenition.这是一个函数说明部分" << endl;
}

//< 函数首部>         :: =   function <标识符>[<形式参数表>]: <基本类型>;
//多读了一个
void functionhead()
{
	if (symbol == IDEN)
	{
		strcpy_s(id, tokenBuffer);
		strcpy_s(funcproname, tokenBuffer);
		insertTab(id, obj, notype, normal);//暂时定为notype
		//emit((char*)"LAB", id, (char*)"", (char*)"",lev);
		temptab = tabindex - 1;
		getsym();
		lev++;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		display[lev] = btabindex;
	}
	else
	{
		error(28);//必须是标识符
		getsym();		
	}
	if (symbol == LPAREN)
	{
		formal_para_chart();
	}
	btab[display[lev]].psze = 4 * paranum;
	for (int i = 1; i <= paranum; i++)
	{
		tab[tabindex - i].adr = (lev - 1 + 2 + i) * 4 * (-1);
	}
	paranum = 0;
	if (symbol == COLON)
	{
		getsym();
	}
	else
	{
		error(25);//必须是冒号
		getsym();
	}
	if (symbol == INTSYM || symbol == CHARSYM)
	{
		if (symbol == INTSYM)
		{
			tab[temptab].typ = intkind;
			getsym();
		}
		else
		{
			tab[temptab].typ = charkind;
			getsym();
		}
	}
	else
	{
		error(16);//只有两种参数类型
		getsym();
	}
	if (symbol == SEMICOLON)
		getsym();
	else
	{
		error(15);//必须有分号
	}
	cout << "It is a function head.这是一个函数首部" << endl;
}

//< 复合语句>   ::=   begin<语句>{; <语句>}end
//多读了一个
void combosentence()
{
	if (symbol == IDEN || symbol == IFSYM || symbol == DOSYM || symbol == BEGINSYM || symbol == READSYM || symbol == WRITESYM || symbol == FORSYM)
	{
		sentence();
	}

	while (symbol == SEMICOLON)
	{
		getsym();
		if (symbol == IDEN || symbol == IFSYM || symbol == DOSYM || symbol == BEGINSYM || symbol == READSYM || symbol == WRITESYM || symbol == FORSYM)
		{
			sentence();
		}
	}
	if (symbol==ENDSYM)
	{
		getsym();
	}	
	else
	{
		error(0);//非法字符嘛
		getsym();
	}
	cout << "This is a combo sentence.这是一个复合语句" << endl;
}

//< 条件> :: = <表达式><关系运算符><表达式>
//多读了一个//传进来var1
void condition()//传进来var4
{
	arg* place1=genarg();//有实体
	
	expr();//var3
	
	equal(place1, var3);


	rela_operator();//op
	expr();//var3


	emit(op, var4, place1, var3,lev);
	cout << "This is a condition.这是一个条件" << endl;
}

//< 关系运算符>      ::=   <|<=|>|>= |=|<>
//多读了一个
void rela_operator()
{
	if (symbol == LSS)
	{
		getsym();//  <
		strcpy_s(op,30,"LEQ");
	}
	else if (symbol == LEQ)
	{
		getsym();//  <=
		strcpy_s(op, 30, "BGT");
	}
	else if (symbol == GTR)
	{
		getsym();//  >
		strcpy_s(op, 30, "SEQ");
	}
	else if (symbol == GEQ)
	{
		getsym();//  >=
		strcpy_s(op, 30, "SMT");
	}
	else if (symbol == EQL)
	{
		getsym();//  =
		strcpy_s(op, 30, "NQL");
	}
	else if (symbol == NEQ)
	{
		getsym();// <>
		strcpy_s(op, 30, "EQL");
	}
	else
	{
		error(0);//关系运算符不太对
		getsym();
	}

}

//< 条件语句> ::=   if<条件>then<语句> | if<条件>then<语句>else<语句>
//多读了一个
void ifsentence()
{
	arg* lab1=newlab();
	arg* lab2=newlab();
	//arg* templab = newlab();
//	arg* templab2;
	//equal(lab1,templab);

	//templab2 = newlab();
	//equal(lab2,templab2);
	//emit((char*)"test1", lab1, (char*)"", (char*)"", lev);
	var4 = genarg();
	equal(var4,lab1);////////、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、、
	//emit((char*)"test2", var1, (char*)"", (char*)"", lev);//!
	condition();//传进去var4
	if (symbol == THENSYM)
	{
		getsym();
		sentence();
		emit((char*)"JMP",lab2,NULL,NULL,lev);
		emit((char*)"LAB", lab1, NULL, NULL,lev);
	}
	else
	{
		error(0);//必须是then
		getsym();
	}
	if (symbol == ELSESYM)
	{
		getsym();
		sentence();
		
	}
	emit((char*)"LAB", lab2, NULL, NULL,lev);
	cout << "This is a if sentence.这是一个if语句" << endl;
}

//< 当循环语句>      ::=   do<语句> while<条件>
void dosentence()
{
	arg* place1=genarg();
	var1 = genarg();
	equal(var1,newlab());
	emit((char*)"LAB", var1, NULL, NULL,lev);
	equal(place1, var1);
	sentence();
	var4 = genarg();
	equal(var4,place1);
	if (symbol == WHILESYM)
	{
		getsym();
		condition();
	}
	else
	{
		error(0);//必须是while
		getsym();
	}
	cout << "This is a do sentence.这是一个do语句" << endl;
}

//< 读语句> ::=   read'('<标识符>{,<标识符>}')'
void readsentence()
{
	int inde;
	arg* temp;
	if (symbol == LPAREN)
	{
		getsym();
		if (symbol == IDEN)
		{
			strcpy_s(id, tokenBuffer);
			inde = look_tab(id);
			temp = bdarg(id, inde, argType);
			
			if (inde > 0){
				emit((char*)"RED", temp, NULL, NULL,lev);
			}
			else
			{
				error(8);
			}
			getsym();
		}
		else
		{
			error(28);//此处必须是标识符
			getsym();
		}
	}
	else
	{
		error(18);//此处必须是（
		getsym();
	}
	while (symbol == COMMA)
	{
		getsym();
		if (symbol == IDEN)
		{
			strcpy_s(id, tokenBuffer);
			inde = look_tab(id);
			if (inde > 0){
				temp = bdarg(id, inde, argType);
				emit((char*)"RED", temp, NULL, NULL,lev);
			}
			else
			{
				error(8);
			}
			getsym();
		}
		else
		{
			error(28);//此处必须是标识符
			getsym();
		}
	}
	if (symbol == RPAREN)
	{
		getsym();
	}
	else
	{
		error(18);//此处必须是）
		getsym();
	}
	cout << "This is a read sentence.这是一个读语句" << endl;
}


//< 写语句> ::=   write'('<字符串>,<表达式>')'|write'('<字符串>')'|write'('<表达式>')'
//多读了一个
void writesentence()
{
	if (symbol == LPAREN)
	{
		getsym();
		
		if (symbol == QUOSTRING)//是字符串
		{
			strlength[stringnum] = strlen(tokenBuffer);//长度
			
			strcat_s(str,1000,tokenBuffer);//拼接
			stringnum++;
			strinum[stringnum] = strinum[stringnum - 1] + strlen(tokenBuffer);

			var1 = bdarg(tokenBuffer,stringnum-1,chuanType);
			
			
			/////////////////////////
			///////字符串长度不能超过30
			emit((char*)"WRT", var1, NULL, NULL, lev);
			getsym();
			if (symbol == COMMA)
			{
				getsym();
				expr();//var3
				emit((char*)"WRT", var3, NULL, NULL,lev);
			}
		}
		else
		{
			expr();
			emit((char*)"WRT", var3, NULL, NULL,lev);
		}

	}
	else
	{
		error(18);//此处必须为（
		getsym();
	}
	if (symbol == RPAREN)
	{
		getsym();
	}
	else
	{
		error(18);//此处必须为）
		getsym();
	}
	cout << "This is a write sentence.这是一个写语句" << endl;
}

//< for循环语句>  ::=   for <标识符>  := <表达式> （downto | to） <表达式> do <语句> //步长为1
void forsentence()
{
	int inde;
	arg* lab1;
	arg* lab2;
	arg* place1;
	arg* temp;
	arg* place2;
	lab1=newlab();
	lab2=newlab();
	place1 = genarg();
	strcpy_s(place1->word, (char*)"1");
	place1->type = justnumber;
	//_itoa_s(1,place1,10,10);
	
	if (symbol == IDEN)
	{
		
		
		inde = look_tab(tokenBuffer);
		place2 = bdarg(tokenBuffer, inde, argType);
		if (inde > 0){
			
		}
		else
		{
			error(8);//未被定义
		}
		getsym();
		if (symbol == BECOMES)
		{
			getsym();
			expr();
			emit((char*)"ASG", place2, var3, NULL, lev);
			emit((char*)"LAB", lab1, NULL, NULL, lev);
			if (symbol == DOWNTOSYM || symbol == TOSYM)
			{
				getsym();
				expr();//var3
				if (symbol == TOSYM){
					emit((char*)"BGT", lab2, place2, var3, lev);
					if (symbol == DOSYM)
					{
						getsym();
						sentence();
						temp = genarg();
						equal(temp, place2);
						emit((char*)"ADD", place2, temp, place1, lev);
						emit((char*)"JMP",lab1,NULL,NULL,NULL);
					}
					else
					{
						error(0);//for循环缺少do
						getsym();
					}
				}
				else				
				{
					emit((char*)"SMT", lab2, place2, var3, lev);
					if (symbol == DOSYM)
					{
						getsym();
						sentence();
						temp = genarg();
						equal(temp, place2);
					
						emit((char*)"SUB", place2, temp, place1, lev);
						emit((char*)"JMP", lab1, NULL, NULL,lev);
					}
					else
					{
						error(0);//for循环缺少do
						getsym();
					}

				}
				emit((char*)"LAB", lab2, NULL, NULL,lev);
			}
			else
			{
				error(0);//只能为downto 或者 to
				getsym();
			}
		}
		else
		{
			error(30);//只能为:=
			getsym();
		}
	}
	else
	{
		error(28);//只能为标识符
		getsym();
	}
	cout << "This is a for sentence.这是一个for语句" << endl;
}

//< 语句>::=   <赋值语句>|<条件语句>|<当循环语句>|<过程调用语句>|<复合语句>|<读语句>|<写语句>|<for循环语句>|<空>
//多读了一个
void sentence()
{
	if (symbol == IFSYM)
	{
		getsym();
		ifsentence();
		strcpy_s(op,10,"");
	}
	else if (symbol == DOSYM)
	{
		getsym();
		dosentence();
		strcpy_s(op, 10, "");
	}
	else if (symbol == BEGINSYM)//复合语句
	{
		getsym();
		combosentence();
		strcpy_s(op, 10, "");
	}
	else if (symbol == READSYM)
	{
		getsym();
		readsentence();
		strcpy_s(op, 10, "");
	}
	else if (symbol == WRITESYM)
	{
		getsym();
		writesentence();
	}
	else if (symbol == FORSYM)
	{
		getsym();
		forsentence();
		strcpy_s(op, 10, "");
	}
	else if (symbol == IDEN)//有可能是赋值语句/过程调用语句/
	{
		int inde;
		strcpy_s(id, tokenBuffer);
		getsym();
		inde = look_tab(id);
		if (symbol == BECOMES || symbol == LBRACKET)//赋值语句
		{
			becomesentence();
		}
		else if (tab[inde].obj == prokind)//过程调用语句
		{
			bool para;
			if (symbol == LPAREN){
				para = true;
				getsym();
			}
			else
				para = false;
			//getsym();
			callproceduresentence(para);
			strcpy_s(op, 10, "");
		}
		else
			error(0);
	}
	else
	{
		error(0);//这个错误分支应该是不会进来的
		getsym();
	}
	cout << "This is a sentence.这是一个语句" << endl;
}

//< 赋值语句> ::=  <标识符> := <表达式>| <函数标识符> := <表达式> | <标识符>'['<表达式>']':= <表达式>
//多读了一个
void becomesentence()
{
	int inde;
	arg* place1;
	arg* temp;
	if (symbol == BECOMES)//标识符或者是函数表达式
	{
		inde=look_tab(id);
		temp = bdarg(id, inde, argType);
		temp->ident = inde;
		if (tab[inde].obj == constkind)
		{
			error(14);
			//常量不能赋值
			return;
		}
		if (inde > 0){
			if (tab[inde].obj == funckind){//
				//函数赋值语句
				//两种情况 自己给自己赋值 其他种类赋值
				if (strcmp(id, tab[btab[display[lev-1]].last].name) == 0){
					getsym();
					expr();//返回var3
					emit((char*)"RVAL", var3, NULL, NULL, lev);
				}
				else
				{
					getsym();
					expr();//返回var3
					emit((char*)"ASG", temp, var3, NULL, lev);//函数也有值///////////////////////////////////////////////////////////////////////////////////////////////////////////////

				}
			}
			else{
				getsym();
				expr();
				temp->type = var3->type;
				emit((char*)"ASG", temp, var3, NULL, lev);
				//标识符赋值语句 注意const、prokind不能被赋值
			}
		}
		else
		{
			error(8);//标识符未被定义 查表没查到
		}
	}
	else if (symbol == LBRACKET)//数组元素
	{
		inde = look_tab(id);
		temp = bdarg(id, inde, argType);
		if (inde == 0){
			error(8);
		}
		
		if (tab[inde].typ != arraychar && tab[inde].typ != arrayint){
			error(8);//不是数组
			return;//待处理
		}
		getsym();
		expr();
		if (var3->type == justnumber){
			int a = atoi(var3->word);
			if (a >= atab[tab[temp->ident].ref].length){
				error(27);/////数组越界
				return;
			}
			
			
		}

		place1 = newplace();
		insert_temp_var(place1->word);
		place1->ident = tabindex - 1;
		tab[tabindex - 1].normal = 0;
		if (tab[place1->ident].typ == arrayint)
			tab[tabindex - 1].typ = intkind;
		else
			tab[tabindex - 1].typ = charkind;
		



		emit((char*)"OFF", place1, temp, var3, lev);
		
		if (symbol == RBRACKET)
		{
			getsym();
		}
		else
		{
			error(17);//此处必须为右中括号
			getsym();
		}
		if (symbol == BECOMES)
		{
			getsym();
		}
		else
		{
			error(30);//此处必须为赋值符号
			getsym();
		}
		expr();
		emit((char*)"ASG", place1, var3, NULL, lev);
	}
	else
	{
		error(1);//这里应该不会进来
		getsym();
	}
	cout << "This is a becomes sentence.这是一个赋值语句" << endl;
}

//< 过程调用语句>  ::=  <标识符>[<实在参数表>]
//多读了一个
void callproceduresentence(bool para)
{
	int inde;
	int labnum;
	arg* temp;
	arg* a;
	inde = look_tab(id);
	
	labnum = tab[inde].adr;
	temp=bdarg(id, inde, LabType);
	emit((char*)"SAVE",NULL, NULL,NULL, lev);

	a = genfuncprolab(labnum);
	a->ident = inde;
	if (inde > 0){
		if (para)
			real_para_chart(temp);
		else
		{
			if ((btab[tbt[temp->ident]].psze) / 4 != 0){
				error(9);
				getsym();
			}
		}
		emit((char*)"CALP", a, NULL, NULL, lev);//存在var1里了记得记得！
	}
	else
	{
		error(6);
	}
	cout << "This is a call procedure sentence.这是一个过程调用语句" << endl;
}

//< 实在参数表> ::= '(' <实在参数> {, <实在参数>}')'
//< 实在参数>           :: = <表达式>
//多读了一个
void real_para_chart(arg* p1)//函数名称
{
	int i = 1;
	//char place[30];//
	char n[30];
	int temp = 0;
	expr();//返回var3
	_itoa_s(i,n,10,10);

	arg *newarg = (arg *)malloc(sizeof(arg));
	newarg->ident = 0;
	strcpy_s(newarg->word, n);

	emit((char*)"PUSH", p1, newarg, var3, lev);
	i++;
	while (symbol == COMMA)
	{
		getsym();
		expr();//返回var3
		_itoa_s(i, n, 10, 10);

		arg *newarg1 = (arg *)malloc(sizeof(arg));
		newarg1->ident = 0;
		strcpy_s(newarg1->word, n);

		emit((char*)"PUSH", p1, newarg1, var3, lev);
		i++;
	}
	if ((btab[tbt[p1->ident]].psze)/4 != i - 1){
		error(9);
		getsym();
	}
	if (symbol == RPAREN)
	{
		getsym();
	}
	else
	{
		error(18);//此处必须为） 
		getsym();
	}

	cout << "This is a real character chart.这是个实在参数表" << endl;
}

//< 表达式> ::=   [+|-]<项>{<加法运算符><项>}//加法运算符可以使加号或者减号
//多读了一个
void expr()//返回var3
{
	arg* place1;// place2[30], place3[30];
	arg* place2;
	arg* a;
	char tempop[30];
	bool k = false;
	if (symbol == PLUS)
	{
		strcpy_s(op,10,"ADD");
		getsym();
		//k = true;
	}
	else if (symbol == MINUS)
	{
		strcpy_s(op, 10, "SUB");
		getsym();
		k = true;
	}
	if (symbol == IDEN || symbol == NUMBER || symbol == LPAREN)//是项
	{		
		/*if (k==false)
			strcpy_s(op, 10, "");*/
		if (symbol == IDEN)
			strcpy_s(id, tokenBuffer);
		term();//返回var2
		//写点啥
		if (strcmp(op,(char*)"SUB")==0 && k==true){
			var3=newplace();
			insert_temp_var(var3->word);
			var3->ident = tabindex - 1;
			tab[var3->ident].typ = tab[var2->ident].typ;
			emit((char*)"NEQ", var3, var2,NULL, lev);//
		}
		else
		{
			var3 = genarg();
			equal(var3,var2);
		}
	}
	else
	{
		error(0);//此处必须有项
		getsym();
	}
	while (symbol == PLUS || symbol==MINUS)//加法运算符可以是加号或者减号
	{
		place1 = genarg();
		equal(place1, var3);
		if (symbol == PLUS){
			strcpy_s(op, 10, "ADD");
			strcpy_s(tempop, 10, "ADD");
		}
		else
		{
			strcpy_s(op, 10, "SUB");
			strcpy_s(tempop, 10, "SUB");
		}
		getsym();
		term();//项
		place2 = genarg();
		equal(place2,var2);
		var3 = genarg();
		a = newplace();
		equal(var3,a);
		insert_temp_var(var3->word);		
		var3->ident = tabindex - 1;
		if (place1->type != justnumber)
			tab[var3->ident].typ = tab[place1->ident].typ;
		else
			tab[var3->ident].typ = intkind;

		
		emit(tempop, var3, place1, place2, lev);//返回var3
	}
	cout << "This is a expression.这是一个表达式" << endl;
}

//< 项> ::=   <因子>{<乘法运算符><因子>}
//多读了一个
void term()//返回var2
{

	arg* place1=genarg();
	factor();//factor返回var1
	equal(place1,var1);
	var2 = genarg();
	equal(var2,var1);
	while (symbol == TIMES || symbol==SLASH)
	{
		if (symbol == TIMES){
			getsym();

			factor();
			var2 = genarg();
			equal(var2,newplace());
			insert_temp_var(var2->word);
			var2->ident = tabindex - 1;
			if (place1->type != justnumber)
				tab[var2->ident].typ = tab[place1->ident].typ;
			else
				tab[var2->ident].typ = intkind;
			emit((char*)"MUL", var2, place1, var1, lev);
		}
		else{
			getsym();
			factor();
			var2 = genarg();
			equal(var2, newplace());
			insert_temp_var(var2->word);
			var2->ident = tabindex - 1;
			if (place1->type != justnumber)
			tab[var2->ident].typ = tab[place1->ident].typ;
			else
				tab[var2->ident].typ = intkind;
			emit((char*)"DIV", var2, place1, var1, lev);
		}
	}
	cout << "This is a term.这是一个项" << endl;
}

//< 因子>  :: = <标识符> | <标识符>'['<表达式>']' | <无符号整数> | '('<表达式>')' | <函数调用语句>
//多读了一个
void factor()//返回var1
{
	char temp[30];
	arg* place1;
	int inde;
	if (symbol == IDEN)//变量或者数组元素或者函数调用语句
	{
		
		//strcpy_s(place1,10, tokenBuffer);
		
		inde = look_tab(tokenBuffer);
		place1 = bdarg(tokenBuffer, inde, argType);

		getsym();
		if (inde == 0){
			error(8);//没找到
		}
		var1 = genarg();
		equal(var1,place1);
		if (symbol == LBRACKET)//数组元素
		{
			getsym();
			expr();//expr返回来var2


			var1 = newplace();
			var1->type = tempargshuyuan;//数组元素

			insert_temp_var(var1->word);
			var1->ident = tabindex - 1;


			if (var3->type == justnumber){
				int a = atoi(var3->word);
				if (a >= atab[tab[place1->ident].ref].length){
					error(27);/////数组越界
					return;
				}


			}
			if (tab[place1->ident].typ == arrayint)
				tab[var1->ident].typ = intkind;
			else
				tab[var1->ident].typ = charkind;


			//strcpy_s(var1, 30, newplace());
			emit((char*)"SEL", var1, place1, var3, lev);
			if (symbol == RBRACKET)
			{
				getsym();
			}
			else
			{
				error(17);//缺少右中括号
				getsym();
			}
		}
		else if (tab[inde].obj==funckind)//函数调用语句
		{
			bool para;
			if (symbol == LPAREN){
				para = true;
				getsym();
			}
			else
				para = false;
			
			callfunction(place1,para);

		}
	}
	else if (symbol == NUMBER)
	{

		_itoa_s(number, temp, 10, 10);
		var1 = genarg();
		strcpy_s(var1->word, 30, temp);
		var1->type = justnumber;

		getsym();
	}
	else if (symbol == LPAREN)
	{
		getsym();
		expr();
		var1 = genarg();
		equal(var1,var3);
		if (symbol == RPAREN)
		{
			getsym();
		}
		else
		{
			error(18);//必须是右括号
			getsym();
		}
	}
	else
	{
		error(0);//语法错误
		getsym();
	}
	cout << "这是个因子" << endl;
}

//< 函数调用语句>      ::=   <标识符>[<实在参数表>]
//多读一个
void callfunction(arg* p1,bool para)//返回var1
{
	int inde;
	arg* temp;
	arg* a;
	int labnum;
	inde = look_tab(p1->word);
	{
		if (inde <= 0){
			error(7);
			getsym();
		}
	}
	labnum = tab[p1->ident].adr;
	a = genfuncprolab(labnum);
	a->ident = p1->ident;


	emit((char*)"SAVE", NULL, NULL, NULL, lev);
	if (para)
		real_para_chart(p1);
	else
		if ((btab[tbt[p1->ident]].psze) / 4 != 0){
		error(9);
		getsym();
		}
	

	temp = newplace();
	insert_temp_var(temp->word);
	temp->ident = tabindex - 1;

	var1 = genarg();
	equal(var1, temp);
	emit((char*)"CALF", a, var1, NULL, lev);//存在var1里了记得记得！

}
