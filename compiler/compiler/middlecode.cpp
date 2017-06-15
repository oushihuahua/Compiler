#include "stdafx.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "define.h"

using namespace std;
middlecode code[1000];
int codenum = 0;
int lab = 0;
int temp_value = 0;
FILE* fp;
FILE* fp1;
extern int tabindex;
extern info tab[200];
extern int btabindex;
extern btabFrame btab[200];

void equal(arg* save, arg* give)
{
	strcpy_s(save->word,30, give->word);
	save->ident = give->ident;
	save->type = give->type;
}

arg* genarg()
{
	arg *newarg = (arg *)malloc(sizeof(arg));
	newarg->ident = 0;
	return newarg;

}


arg* bdarg(char name[], int inde, int type){
	arg *newarg = (arg *)malloc(sizeof(arg));
	strcpy_s(newarg->word, name);
	newarg->ident = inde;
	newarg->type = type;	
	return newarg;
}
void emit(char op[], arg* name1, arg* name2, arg* name3,int level)
{
	strcpy_s(code[codenum].op, op);
	code[codenum].var1 = name1;
	code[codenum].var2 = name2;
	code[codenum].var3 = name3;
	code[codenum].leve = level;
	codenum++;
}

void printmidcode()//打印四元式
{

	
	int i = 0;
	fopen_s(&fp,"midcode.txt", "w+");
	while (i<codenum) {
		if (strcmp(code[i].op, "delete") != 0) {
			fprintf(fp, "%s,\t", code[i].op);
			fprintf(fp, "%s,\t", (code[i].var1)->word);
			//fprintf(fp, "%d", (code[i].var1)->ident);
			fprintf(fp, "%s,\t", (code[i].var2)->word);
			//fprintf(fp, "%d", (code[i].var2)->ident);
			fprintf(fp, "%s;\n", (code[i].var3)->word);
			//fprintf(fp, "%d", (code[i].var3)->ident);
		}
		if (strcmp(code[i].op, "end") == 0)
			fprintf(fp, "\n");
		i++;
	}
	fclose(fp);
	return;
}
char* printInstruct(middlecode mid){
	char *s = (char *)malloc(60 * sizeof(char));
	memset(s, 0, 60 * sizeof(char));
	strcat_s(s, 60, mid.op);
	if (mid.var1->word!=NULL)
	strcat_s(s, 60, mid.var1->word);
	if (mid.var2->word != NULL)
	strcat_s(s, 60, mid.var2->word);
	if (mid.var3->word != NULL)
	strcat_s(s, 60, mid.var3->word);
	return s;
}
void printTable()//打印符号表
{
	int i = 1;
	fopen_s(&fp1,"Table.txt", "w+");
	fprintf(fp1,"i\t name\t link\t obj\t typ\t ref\t lev\t normal\t adr\t \n");
	while (i<tabindex) {
		fprintf(fp1, "%d\t %s\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t \n",i, tab[i].name, tab[i].link, tab[i].obj, tab[i].typ, tab[i].ref, tab[i].lev, tab[i].normal,tab[i].adr);
		i++;
	}
	fclose(fp1);
	return;
}

void printBtab()
{
	int i = 1;
	fopen_s(&fp, "btab.txt", "w+");
	fprintf(fp, "i\t last\t lpar\t \n");
	for (i; i <= btabindex; i++)
	{
		fprintf(fp, "%d\t %d\t %d\t \n", i, btab[i].last, btab[i].lpar);
	}
	fclose(fp);
	return;
}
arg* newlab(){
	arg *newarg = (arg *)malloc(sizeof(arg));
	//arg* a;
	char *p = (char *)malloc(10 * sizeof(char));
	char q[10];
	memset(p, 0, 10 * sizeof(char));//将p清0
	p[0] = 'l';
	p[1] = 'a';
	p[2] = 'b';
	_itoa_s(lab, q, 10);
	strcat_s(p,10 ,q);//p和气后的链接 放到p中 "lab"+lab(数字)
	lab++;
	strcpy_s(newarg->word, p);
	newarg->type = LabType;
	newarg->ident = 0;
	
	//返回字符串地址
	return newarg;
}
arg* genfuncprolab(int i){
	arg *newarg = (arg *)malloc(sizeof(arg));
	//arg* a;
	char *p = (char *)malloc(10 * sizeof(char));
	char q[10];
	memset(p, 0, 10 * sizeof(char));//将p清0
	p[0] = 'l';
	p[1] = 'a';
	p[2] = 'b';
	_itoa_s(i, q, 10);
	strcat_s(p, 10, q);//p和气后的链接 放到p中 "lab"+lab(数字)
	strcpy_s(newarg->word, p);
	newarg->type = LabType;
	newarg->ident = 0;

	//返回字符串地址
	return newarg;
}

arg* new_profunclab(int inde){
	arg *newarg = (arg *)malloc(sizeof(arg));
	//arg* a;
	char *p = (char *)malloc(10 * sizeof(char));
	char q[10];
	memset(p, 0, 10 * sizeof(char));//将p清0
	p[0] = 'l';
	p[1] = 'a';
	p[2] = 'b';
	_itoa_s(lab, q, 10);
	strcat_s(p, 10, q);//p和气后的链接 放到p中 "lab"+lab(数字)
	lab++;
	strcpy_s(newarg->word, p);
	newarg->type = profunclab;
	newarg->ident = inde;

	//返回字符串地址
	return newarg;
}

arg* newplace()
{
	arg *newarg = (arg *)malloc(sizeof(arg));
	char *p = (char *)malloc(10 * sizeof(char));
	char q[10];
	memset(p, 0, 10 * sizeof(char));
	p[0] = '$';
	_itoa_s(temp_value, q, 10,10);
	strcat_s(p,10,q);
	temp_value++;


	strcpy_s(newarg->word, p);
	newarg->type = temparg;
	newarg->ident = 0;


	return newarg;//返回字符串地址 $0啥的
}
