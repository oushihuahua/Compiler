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
void rela_operator(bool val);
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
arrayinfo atab[200];//�����
int atabindex = 0;//������1��ʼ��
btabFrame btab[200];//btab
int tabindex = 1;//���ű�ָ�� ��1��ʼ
int btabindex = 1;//�ֳ����ָ�� ��1��ʼ
int display[200] = { 0 };//�ֳ���������
//int displayindex = 1;//�ֳ���������ָ�� ��1��ʼ
info tab[200];//���ű� ��info��ɵĽṹ��
int last= 0;//���һ����ʶ��
int lpar = 0;//���һ������
int lev = 1;//��δ�1��ʼ
int varnum = 0;//ͳ��tab���������˵�ǰ�ֳ���ļ���iden
int localadr = 0;
char id[30];//������ʱ�洢
int tbt[200] = { 0 };

int obj;
int typ;
int normal;
int ref = 0;
int adr = 0;

extern int number;
int tempnum=0;//varһ����������
int temptab = 0;//��ʱ��¼������tab�е��±�

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
//int paranums=0;//ͳ�Ʋ�����������ʱ����
//char tempname[30];//��ʱ�Ĵ�����
extern int symbol;//���һ��������symbol 
extern void getsym();
extern void error(int error_num);
extern char tokenBuffer[82];//�������ַ��� ���Ϊ81

void insert_temp_var(char name_in[])
{
	tab[tabindex].ref = 0;
	tab[tabindex].adr = 0;
	//indexΪָ����ű��ջ��ָ��
	if (tabindex > 199)
	{
		error(1);//���ű�Խ��
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
		tab[tabindex].link = btab[display[lev]].lastvar;//Ҳ�������������������
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
////���ű���ƣ���д���ű� ���ű����װ200��
void insertTab(char name_in[], int obj_in, int typ_in, int normal_in)
{
	tab[tabindex].ref = 0;
	tab[tabindex].adr = 0;
	//indexΪָ����ű��ջ��ָ��
	if (tabindex > 199)
	{
		error(1);//���ű�Խ��
		return;
	}
	/*
	
	*/
	//����Ǵ�main��ʼ�ĵ�һ��
	if (tabindex == 1){
		tab[tabindex].link = 0;
		//�½�btab���һ��
		btab[btabindex].last = 0;
		btab[btabindex].lpar = 0;
		btab[btabindex].psze = 0;
		btab[btabindex].vsze = 0;
		btab[btabindex].lastvar = 0;
		display[1] = 1;//level[1]���ڵǼ���btab��ĵ�һ��
	}
	if (obj_in == funckind || obj_in == prokind)//�����function������procedure
	{//����������� ������������pro��func���������
		if (look_up(name_in, obj_in,1) > 0)//�ҵ��� ����tab���ڵ�
		{
			error(5);//�ظ�����
			return;
		}
		else if (look_up(name_in, obj_in,1) == 0)//û���ҵ� �������
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
	else//�������Ǳ������ǳ���
	{
		if (obj_in == parakind )//�ǲ������Խ�
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
		else if (look_up(name_in, 0, 1) == 0 )//��� û�鵽 ������� ���ǲ���
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
		else if (look_up(name_in, 0,1) >0)//�鵽��(�����˷��ű����ڵ�ַ)
		{
			error(5);//�ظ�����
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

//�����Ϊpro��func��������ķ��ű� ����Ҫ�������pro��func������
/*void clean_table_proc(){
	int temp1 = index - 1;
	int temp2;
	while ((Table[temp1].kind == intkind || Table[temp1].kind == charkind || Table[temp1].kind == constintkind
		|| Table[temp1].kind == constcharkind || Table[temp1].kind == paracharkind || Table[temp1].kind == paraintkind)){
		Table[temp1].kind = 0;
		Table[temp1].address = 0;
		Table[temp1].size = 0;
		Table[temp1].paranum = 0;
		Table[temp1].value = 0;
		temp2 = 0;
		while (temp2<30)
			Table[temp1].name[temp2++] = '\0';
		temp1--;
	}
	index = temp1 + 1;////�����µķ��ű�ջ��ָ��
	return;
}*/
//�����
void insertArray(int i,int type,int length){
	atabindex++;
	tab[i].ref = atabindex;//i��tabindex
	atab[atabindex].eltyp = type;//����Ԫ������
	atab[atabindex].length = length;//���鳤��
	localadr = localadr + 4 * (length-1);
}

int look_up(char name_in[],int obj_in,int searchtype)//isprocΪ1��ʾΪ���ҵ���procedure������function
//searchtype Ϊ1 ����ʱ�Ĳ��� Ϊ0 ���ұ�ʶ��ʱ
{
	int a, b;
	b = 0;
	if (obj_in == prokind || obj_in==funckind){//��procedure ������ function
		for (a = tabindex - 1; a >= 1; a--){
			if (strcmp(tab[a].name, name_in) == 0){
				break;
			}
		}
		if (a ==0){//û�ҵ�
			if (searchtype==0){
				error(7);//û�ҵ������������������� ���̺��� δ����
				printf("%s\n", name_in);
			}
			return 0;
		}
		return a;//���ҳɹ��ͷ��ظ�pro����func�ڷ��ű��е�λ��
	}
	else{//���ҵ����Ǹ��������߱���
		//���ڵ�ǰ�㳬��
		int level = lev;
		for (; level >= 1; level--){
			a = display[level];
			if (btab[a].last == 0)
				continue;
			for (b = btab[a].last;; b--){
				if (strcmp(tab[b].name, name_in) == 0)//�ҵ���
					return b;
				if (tab[b].link == 0)
					break;
			}
		}
		if (searchtype == 0){
			error(8);
			printf("%s\n", name_in);
		}
		return 0;//û�ҵ�
	}
}

int look_tab(char name_in[]){//���� ���� �ͱ�ʶ����������
	int a, b;
	b = 0;
		int level = lev;
		for (; level >= 1; level--){
			a = display[level];
			if (btab[a].last == 0)
				continue;
			for (b = btab[a].last;; b--){
				if (strcmp(tab[b].name, name_in) == 0 && tab[b].lev==level)//�ҵ���
					return b;
				if (tab[b].link == 0 && tab[b].lev==level)
					break;
			}
		}
			error(8);
			printf("%s\n", name_in);
		return 0;//û�ҵ�
}

//���kind����
void insertkind(int type,int index)
{
	tab[index].typ =type;
	return;
}
//��������С



//<����>                   :: =   <�ֳ���>.
void main_program()
{
	getsym();
	lev = 1;
	sub_program();
	lev--;
	//����Ƿ�Ϊ.
	if (symbol == PERIOD)
		cout << "It is a legal program.���Ǹ��Ϸ�����������" << endl;
	else
		error(1);//ȱ��.
	//TODO!!!!!!!!!!!�������ټ��һ�»���û���ַ���.���治�����ַ�
}

//< �ֳ���>               :: = [<����˵������>][<����˵������>]{[<����˵������>] | [<����˵������>]}<�������>
//��󲻶�� ���������
void sub_program()
{
	if (symbol == CONSTSYM)
	{
		//����˵������
		getsym();
		defconsts_part();

	}
	if (symbol==VARSYM)
	{
		//����˵������
		getsym();
		defvars_part();
	}

	while (symbol == PROSYM || symbol==FUNCSYM )//���ñ�־ LAB id��
	{
		normal = 1;
		//����˵�����ֻ����Ǻ���˵������
		if (symbol == PROSYM)
		{
			obj = prokind;
			//����˵������
			getsym();
			defproce();
			
		}
		else 
		{
			//����˵������
			obj = funckind;
			getsym();
			deffunc();
		}

	}
	if (symbol == BEGINSYM)
	{
		//�Ǹ������
		if (lev - 1 > 0){

			arg *lab1 = new_profunclab(btab[display[lev - 1]].last);//
			//addrӦΪlab-1;
			tab[btab[display[lev - 1]].last].adr = lab - 1;//lab���
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
		error(1);//û�и������ �﷨����
		getsym();
	}
	//lev--;
	cout << "It is a sub procedure.���Ǹ��ֳ���"<<endl;
	

}

//< ����˵������>  ::=   const<��������>{,<��������>};
//< ��������>           :: = <��ʶ��>�� <����>
//�����һ��
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
			error(10);//�϶��ǵȺ�
			while (symbol != COMMA && symbol != SEMICOLON) { /////����
				if (symbol == EOFSYM) {
					printf("EOFSYM!\n");
					return;
				}
				getsym();
			}
			return;
		}
			
		cout << "It is a constnumber definition.���Ǹ���������" << endl;
	}
	else
	{
		error(11);
		while (symbol != COMMA && symbol != SEMICOLON) { /////����
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
				error(10);//�϶��ǵȺ�
				while (symbol != COMMA && symbol != SEMICOLON) { /////����
					if (symbol == EOFSYM) {
						printf("EOFSYM!\n");
						return;
					}
					getsym();
				}
				cout << "It is a constnumber definition.���Ǹ���������" << endl;
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
		error(15);//ȱ�ٷֺ�
		while (symbol != VARSYM && symbol !=PROSYM && symbol !=FUNCSYM && symbol != BEGINSYM && symbol!=CONSTSYM){////////ȱ�ֺ�ʱ������������һ����ȷ�ĺϷ����
			if (symbol == EOFSYM) { printf("EOF!\n"); return; }
			getsym();
		}
		return;
	}
	cout << "It is a consts definition part.���Ǹ�����˵������" << endl;

}

//< ����>   ::=    [+| -] <�޷�������>|<�ַ�>
//�����һ��
void constnum()
{
	if (symbol == PLUS || symbol == MINUS || symbol == NUMBER)
	{
		if (symbol == PLUS)
		{
			getsym();
			if (symbol == NUMBER)
			{
				//�Ǵ��Ӻŵ�����Ү��
				cout << "It is a positive number.���Ǹ�����" << endl;
				typ = intkind;
				insertTab(id, obj, typ, 0);
				
				//***δ��
				//_itoa_s(number, var2, 10, 10);
				tab[tabindex - 1].adr = number;
				//emit((char*)"ASG",id,var2,(char*)"",lev);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���д��Ԫʽ
			}
			else
				error(1);//�Ӻź��������

		}
		else if (symbol == MINUS)
		{
			getsym();
			if (symbol == NUMBER)
			{
				//�Ǹ�����
				cout << "It is a negative number.���Ǹ�����" << endl;
				typ = intkind;
				insertTab(id, obj, typ, 0);
				number = 0 - number;
				tab[tabindex - 1].adr = number;
				//_itoa_s(number, var2, 10, 10);
				//emit((char*)"ASG", id, var2, (char*)"",lev);////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���д��Ԫʽ
				//***δ��
			}
			else
				error(1);//���ź��������
		}
		else
		{
			cout << "It is a number.���Ǹ����֣�����" << endl;
			typ = intkind;
			insertTab(id, obj, typ, 0);
			//_itoa_s(number, var2, 10, 10);
			tab[tabindex - 1].adr = number;
			//emit((char*)"ASG", id, var2, (char*)"",lev);////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���д��Ԫʽ
			//***δ��
		}
		getsym();
	}
	else if (symbol == APOSCHARAC)
	{
		//���ַ�
		cout << "It is a Apos Chracter.���Ǹ��ַ�" << endl;
		typ = charkind;
		insertTab(id, obj, typ, 0);
		tab[tabindex - 1].adr = (int)tokenBuffer[1];//ת����ascii
		//emit((char*)"ASG", id,tokenBuffer, (char*)"",lev);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!���д��Ԫʽ
		//***δ��
		getsym();
	}
	else
	{
		error(14);//���ǷǷ��ַ� ��ֵ���Ϸ�
		while (symbol != COMMA && symbol != SEMICOLON) { /////����
			if (symbol == EOFSYM) {
				printf("EOFSYM!\n");
				return;
			}
			getsym();
		}
		return;
	}
}



//< ����˵������>  :: = var <����˵��>; {<����˵��>; }
//�����һ��
void defvars_part()
{
	obj = varkind;
	tempnum = 0;
	normal = 1;
	if (symbol == IDEN)
	{
		strcpy_s(id, tokenBuffer);
		normal = 1;//����ֵ
		insertTab(id, obj,typ,normal);
		tempnum++;
		//δ��
		getsym();
		defvars();
	}
	else
	{
		error(0);//�����Ǳ�ʶ��
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
		error(15);//ȱ�ٷֺ� �������Ͳ���������ˣ���������������������������
		while (symbol != IDEN && symbol != VARSYM && symbol != FUNCSYM && symbol!=PROSYM && symbol!=BEGINSYM) {
			if (symbol == EOFSYM) { printf("EOF!\n"); return; }
			getsym();
		}
	}
	while (symbol==IDEN)
	{
		strcpy_s(id, tokenBuffer);
		normal = 1;//����ֵ
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
			error(1);//ȱ�ٷֺ� �������Ͳ���������ˣ���������������������������
		}
	}
	cout << "It is a variables definiton part.���Ǹ�����˵������" << endl;

	
}

//< ����˵��>           :: = <��ʶ��>{, <��ʶ��>} : <����>
//�����һ��
void defvars()
{
	while (symbol == COMMA)
	{
		
		getsym();
		if (symbol == IDEN)
		{
			strcpy_s(id, tokenBuffer);
			normal = 1;//����ֵ
			insertTab(id, obj, typ, normal);
			tempnum++;
			//δ��
			getsym();
		}
		else
		{
			error(0);//���ź���ض��Ǳ�ʶ��
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
		error(25);//������ð��
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
		else//����
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
				error(17);//��������������
				getsym();
			}
			if (symbol == NUMBER)
			{
				num = number;
				getsym();//��������
			}
			else
			{
				error(29);//�������޷�������
				getsym();
			}
			if (symbol == RBRACKET)
			{
				getsym();
			}
			else
			{
				error(17);//��������������
				getsym();
			}
			if (symbol == OFSYM)
			{
				getsym();
			}
			else
			{
				error(1);//������of
				getsym();
			}
			if (symbol == INTSYM || symbol == CHARSYM)
			{
				if (symbol == INTSYM)
				{
					//��������
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
					//�ַ�������
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
				error(1);//ֻ�����������͵�����
				getsym();
			}
		}
	}
	else
	{
		error(1);//�����﷨������
		getsym();
	}
	cout << "It is a variables definition.���Ǹ�����˵��";
}

//< ����˵������>  :: = <�����ײ�><�ֳ���>{; <�����ײ�><�ֳ���>};
//�����һ��
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
		error(1);//ȱ�ٷֺţ�������������������������������
	}
	cout << "This is a procedure difenition.����һ������˵������" << endl;
}

//< �����ײ�>           ::=   procedure<��ʶ��>[<��ʽ������>];
//�����һ��
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
		//����adr
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
			error(15);//ȱ�ٷֺţ�����������������������������������
		}
	}
	else
	{
		error(1);//�˴���Ҫ�Ǳ�ʶ��
		getsym();
	}
	cout << "This is a proedure head.����һ�������ײ�" << endl;
}

//< ��ʽ������>       ::= '('<��ʽ������>{; <��ʽ������>}')'
//�����һ��
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
		error(1);//������������
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
		error(1);//�˴������ǣ�
		getsym();
	}
	cout << "It is a formal parameters chart.���Ǹ���ʽ������" << endl;
}

//< ��ʽ������>       :: = [var]<��ʶ��>{, <��ʶ��>}: <��������>
//�����һ��
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
		error(28);//�˴�Ӧ���Ǹ���ʶ��
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
			error(28);//�˴�Ӧ���Ǳ�ʶ��
			getsym();
		}
	}
	if (symbol == COLON)
	{
		getsym();
		if (symbol == INTSYM || symbol == CHARSYM)
		{
			if (symbol == INTSYM) //int����
			{
				for (; tempnum > 0; tempnum--)
				{
					insertkind(intkind, tabindex - tempnum);

				}
				tempnum = 0;
				getsym();
			}
			else //char����
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
			error(1);//ֻ����int������char
			getsym();
		}
	}
	else
	{
		error(1);//�˴�Ӧ����ð��
		getsym();
	}
	cout << "It is a formal parameters section.������ʽ������" << endl;
}

//< ����˵������>  :: =  <�����ײ�><�ֳ���>{; <�����ײ�><�ֳ���>};
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
		error(1);//ȱ�ٷֺţ�������������������������������
	}
	cout << "This is a function difenition.����һ������˵������" << endl;
}

//< �����ײ�>         :: =   function <��ʶ��>[<��ʽ������>]: <��������>;
//�����һ��
void functionhead()
{
	if (symbol == IDEN)
	{
		strcpy_s(id, tokenBuffer);
		strcpy_s(funcproname, tokenBuffer);
		insertTab(id, obj, notype, normal);//��ʱ��Ϊnotype
		//emit((char*)"LAB", id, (char*)"", (char*)"",lev);
		temptab = tabindex - 1;
		getsym();
		lev++;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		display[lev] = btabindex;
	}
	else
	{
		error(1);//�����Ǳ�ʶ��
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
		error(1);//������ð��
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
		error(1);//ֻ�����ֲ�������
		getsym();
	}
	if (symbol == SEMICOLON)
		getsym();
	else
	{
		error(1);//�����зֺ�
	}
	cout << "It is a function head.����һ�������ײ�" << endl;
}

//< �������>   ::=   begin<���>{; <���>}end
//�����һ��
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
		error(1);//�Ƿ��ַ���
		getsym();
	}
	cout << "This is a combo sentence.����һ���������" << endl;
}

//< ����> :: = <���ʽ><��ϵ�����><���ʽ>
//�����һ��//������var1
void condition(bool val)//������var4
{
	arg* place1=genarg();//��ʵ��
	
	expr();//var3
	
	equal(place1, var3);


	rela_operator(val);//op
	expr();//var3


	emit(op, var4, place1, var3,lev);
	cout << "This is a condition.����һ������" << endl;
}

//< ��ϵ�����>      ::=   <|<=|>|>= |=|<>
//�����һ��
void rela_operator(bool val)
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
		if (val)
			strcpy_s(op, 30, "NQL");
		else
		strcpy_s(op, 30, "EQL");
	}
	else
	{
		error(1);//��ϵ�������̫��
		getsym();
	}

}

//< �������> ::=   if<����>then<���> | if<����>then<���>else<���>
//�����һ��
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
	equal(var4,lab1);////////��������������������������������������������������������������������������
	//emit((char*)"test2", var1, (char*)"", (char*)"", lev);//!
	condition(false);//����ȥvar4
	if (symbol == THENSYM)
	{
		getsym();
		sentence();
		emit((char*)"JMP",lab2,NULL,NULL,lev);
		emit((char*)"LAB", lab1, NULL, NULL,lev);
	}
	else
	{
		error(1);//������then
		getsym();
	}
	if (symbol == ELSESYM)
	{
		getsym();
		sentence();
		
	}
	emit((char*)"LAB", lab2, NULL, NULL,lev);
	cout << "This is a if sentence.����һ��if���" << endl;
}

//< ��ѭ�����>      ::=   do<���> while<����>
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
		bool val = true;
		getsym();
		condition(true);
	}
	else
	{
		error(1);//������while
		getsym();
	}
	cout << "This is a do sentence.����һ��do���" << endl;
}

//< �����> ::=   read'('<��ʶ��>{,<��ʶ��>}')'
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
			error(1);//�˴������Ǳ�ʶ��
			getsym();
		}
	}
	else
	{
		error(1);//�˴������ǣ�
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
			error(1);//�˴������Ǳ�ʶ��
			getsym();
		}
	}
	if (symbol == RPAREN)
	{
		getsym();
	}
	else
	{
		error(1);//�˴������ǣ�
		getsym();
	}
	cout << "This is a read sentence.����һ�������" << endl;
}


//< д���> ::=   write'('<�ַ���>,<���ʽ>')'|write'('<�ַ���>')'|write'('<���ʽ>')'
//�����һ��
void writesentence()
{
	if (symbol == LPAREN)
	{
		getsym();
		
		if (symbol == QUOSTRING)//���ַ���
		{
			strlength[stringnum] = strlen(tokenBuffer);//����
			
			strcat_s(str,1000,tokenBuffer);//ƴ��
			stringnum++;
			strinum[stringnum] = strinum[stringnum - 1] + strlen(tokenBuffer);

			var1 = bdarg(tokenBuffer,stringnum-1,chuanType);
			
			
			/////////////////////////
			///////�ַ������Ȳ��ܳ���30
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
		error(1);//�˴�����Ϊ��
		getsym();
	}
	if (symbol == RPAREN)
	{
		getsym();
	}
	else
	{
		error(1);//�˴�����Ϊ��
		getsym();
	}
	cout << "This is a write sentence.����һ��д���" << endl;
}

//< forѭ�����>  ::=   for <��ʶ��>  := <���ʽ> ��downto | to�� <���ʽ> do <���> //����Ϊ1
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
			error(8);//δ������
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
				
				if (symbol == TOSYM){
					getsym();
					expr();//var3
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
						error(1);//forѭ��ȱ��do
						getsym();
					}
				}
				else				
				{
					getsym();
					expr();//var3
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
						error(1);//forѭ��ȱ��do
						getsym();
					}

				}
				emit((char*)"LAB", lab2, NULL, NULL,lev);
			}
			else
			{
				error(0);//ֻ��Ϊdownto ���� to
				getsym();
			}
		}
		else
		{
			error(28);//ֻ��Ϊ:=
			getsym();
		}
	}
	else
	{
		error(1);//ֻ��Ϊ��ʶ��
		getsym();
	}
	cout << "This is a for sentence.����һ��for���" << endl;
}

//< ���>::=   <��ֵ���>|<�������>|<��ѭ�����>|<���̵������>|<�������>|<�����>|<д���>|<forѭ�����>|<��>
//�����һ��
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
	else if (symbol == BEGINSYM)//�������
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
	else if (symbol == IDEN)//�п����Ǹ�ֵ���/���̵������/
	{
		int inde;
		strcpy_s(id, tokenBuffer);
		getsym();
		inde = look_tab(id);
		if (symbol == BECOMES || symbol == LBRACKET)//��ֵ���
		{
			becomesentence();
		}
		else if (tab[inde].obj==prokind)//���̵������
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
	}
	else
	{
		error(1);//��������֧Ӧ���ǲ��������
		getsym();
	}
	cout << "This is a sentence.����һ�����" << endl;
}

//< ��ֵ���> ::=  <��ʶ��> := <���ʽ>| <������ʶ��> := <���ʽ> | <��ʶ��>'['<���ʽ>']':= <���ʽ>
//�����һ��
void becomesentence()
{
	int inde;
	arg* place1;
	arg* temp;
	if (symbol == BECOMES)//��ʶ�������Ǻ������ʽ
	{
		inde=look_tab(id);
		temp = bdarg(id, inde, argType);
		temp->ident = inde;
		if (tab[inde].obj == constkind)
		{
			error(14);
			//�������ܸ�ֵ
			return;
		}
		if (inde > 0){
			if (tab[inde].obj == funckind){//
				//������ֵ���
				//������� �Լ����Լ���ֵ �������ำֵ
				if (strcmp(id, tab[btab[display[lev-1]].last].name) == 0){
					getsym();
					expr();//����var3
					emit((char*)"RVAL", var3, NULL, NULL, lev);
				}
				else
				{
					getsym();
					expr();//����var3
					emit((char*)"ASG", temp, var3, NULL, lev);//����Ҳ��ֵ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

				}
			}
			else{
				getsym();
				expr();
				temp->type = var3->type;
				emit((char*)"ASG", temp, var3, NULL, lev);
				//��ʶ����ֵ��� ע��const��prokind���ܱ���ֵ
			}
		}
		else
		{
			error(8);//��ʶ��δ������ ���û�鵽
		}
	}
	else if (symbol == LBRACKET)//����Ԫ��
	{
		inde = look_tab(id);
		temp = bdarg(id, inde, argType);
		if (inde == 0){
			error(8);
		}
		
		if (tab[inde].typ != arraychar && tab[inde].typ != arrayint){
			error(8);//��������
			return;//������
		}
		getsym();
		expr();

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
			error(1);//�˴�����Ϊ��������
			getsym();
		}
		if (symbol == BECOMES)
		{
			getsym();
		}
		else
		{
			error(1);//�˴�����Ϊ��ֵ����
			getsym();
		}
		expr();
		emit((char*)"ASG", place1, var3, NULL, lev);
	}
	else
	{
		error(1);//����Ӧ�ò������
		getsym();
	}
	cout << "This is a becomes sentence.����һ����ֵ���" << endl;
}

//< ���̵������>  ::=  <��ʶ��>[<ʵ�ڲ�����>]
//�����һ��
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
		emit((char*)"CALP", a, NULL, NULL, lev);//����var1���˼ǵüǵã�
	}
	else
	{
		error(7);
	}
	cout << "This is a call procedure sentence.����һ�����̵������" << endl;
}

//< ʵ�ڲ�����> ::= '(' <ʵ�ڲ���> {, <ʵ�ڲ���>}')'
//< ʵ�ڲ���>           :: = <���ʽ>
//�����һ��
void real_para_chart(arg* p1)//��������
{
	int i = 1;
	//char place[30];//
	char n[30];
	int temp = 0;
	expr();//����var3
	_itoa_s(i,n,10,10);

	arg *newarg = (arg *)malloc(sizeof(arg));
	newarg->ident = 0;
	strcpy_s(newarg->word, n);

	emit((char*)"PUSH", p1, newarg, var3, lev);
	i++;
	while (symbol == COMMA)
	{
		getsym();
		expr();//����var3
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
		error(1);//�˴�����Ϊ�� 
		getsym();
	}

	cout << "This is a real character chart.���Ǹ�ʵ�ڲ�����" << endl;
}

//< ���ʽ> ::=   [+|-]<��>{<�ӷ������><��>}//�ӷ����������ʹ�ӺŻ��߼���
//�����һ��
void expr()//����var3
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
	if (symbol == IDEN || symbol == NUMBER || symbol == LPAREN)//����
	{		
		/*if (k==false)
			strcpy_s(op, 10, "");*/
		if (symbol == IDEN)
			strcpy_s(id, tokenBuffer);
		term();//����var2
		//д��ɶ
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
		error(0);//�˴���������
		getsym();
	}
	while (symbol == PLUS || symbol==MINUS)//�ӷ�����������ǼӺŻ��߼���
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
		term();//��
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

		
		emit(tempop, var3, place1, place2, lev);//����var3
	}
	cout << "This is a expression.����һ�����ʽ" << endl;
}

//< ��> ::=   <����>{<�˷������><����>}
//�����һ��
void term()//����var2
{

	arg* place1=genarg();
	factor();//factor����var1
	equal(place1,var1);
	var2 = genarg();
	equal(var2,var1);
	while (symbol == TIMES || symbol==SLASH)
	{
		if (symbol == TIMES){
			getsym();

			factor();//vr1
			var2 = genarg();
			equal(var2,newplace());
			insert_temp_var(var2->word);
			var2->ident = tabindex - 1;
			if (place1->type != justnumber)
				tab[var2->ident].typ = tab[place1->ident].typ;
			else
				tab[var2->ident].typ = intkind;
			emit((char*)"MUL", var2, place1, var1, lev);
			place1 = genarg();
			equal(place1, var2);
			//equal(var2, place1);
		}
		else{
			getsym();
			factor();//vR1
			var2 = genarg();
			equal(var2, newplace());
			insert_temp_var(var2->word);
			var2->ident = tabindex - 1;
			if (place1->type != justnumber)
			tab[var2->ident].typ = tab[place1->ident].typ;
			else
				tab[var2->ident].typ = intkind;
			emit((char*)"DIV", var2, place1, var1, lev);
			place1 = genarg();
			equal(place1, var2);
			//equal(var2, place1);
		}
	}
	cout << "This is a term.����һ����" << endl;
}

//< ����>  :: = <��ʶ��> | <��ʶ��>'['<���ʽ>']' | <�޷�������> | '('<���ʽ>')' | <�����������>
//�����һ��
void factor()//����var1
{
	char temp[30];
	arg* place1;
	int inde;
	if (symbol == IDEN)//������������Ԫ�ػ��ߺ����������
	{
		
		//strcpy_s(place1,10, tokenBuffer);
		
		inde = look_tab(tokenBuffer);
		place1 = bdarg(tokenBuffer, inde, argType);

		getsym();
		if (inde == 0){
			error(7);//û�ҵ�
		}
		var1 = genarg();
		equal(var1,place1);
		if (symbol == LBRACKET)//����Ԫ��
		{
			getsym();
			expr();//expr������var2

			var1 = newplace();
			var1->type = tempargshuyuan;//����Ԫ��

			insert_temp_var(var1->word);
			var1->ident = tabindex - 1;
			if (tab[place1->ident].typ == arrayint)
				tab[var1->ident].typ = intkind;
			else
				tab[var1->ident].typ = charkind;


			//strcpy_s(var1, 30, newplace());
			emit((char*)"SEL", var1, place1, var2, lev);
			if (symbol == RBRACKET)
			{
				getsym();
			}
			else
			{
				error(1);//ȱ����������
				getsym();
			}
		}
		else if (tab[inde].obj==funckind)//�����������
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
			error(1);//������������
			getsym();
		}
	}
	else
	{
		error(1);//�﷨����
		getsym();
	}
	cout << "���Ǹ�����" << endl;
}

//< �����������>      ::=   <��ʶ��>[<ʵ�ڲ�����>]
//���һ��
void callfunction(arg* p1,bool para)//����var1
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
	

	temp = newplace();
	insert_temp_var(temp->word);
	temp->ident = tabindex - 1;

	var1 = genarg();
	equal(var1, temp);
	emit((char*)"CALF", a, var1, NULL, lev);//����var1���˼ǵüǵã�

}
