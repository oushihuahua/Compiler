#include "stdafx.h"
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "define.h"
#include <fstream>
#include <sstream>

using namespace std;
extern middlecode code[1000];
extern int stringnum;//�ַ���С�� ��stringnum-1��
extern int strlength[100];
extern int strinum[100];
extern char str[1000];
void writeAsm(string op, string o1, string o2, string o3);
void loadOperand(arg* o, string r, int level);
string loadAddress(arg* o, int l,bool val);
void dealCall(middlecode instr);

extern btabFrame btab[200];//btab
extern int display[200];//�ֳ���������
//int displayindex = 1;//�ֳ���������ָ�� ��1��ʼ
extern info tab[200];//���ű� ��info��ɵĽṹ��
extern int codenum;//һ���ж�������Ԫʽ �±��һ��
extern char* printInstruct(middlecode);
middlecode instr;
extern int tbt[200];

ofstream fout("assemble.asm");



//ofstream fout("ass.asm");
void asmhead(){
	stringstream ss;
	int addr=0;
	char q[30];
	int temp = stringnum - 1;
	fout << "	.data"<<endl;
	for (int i = 0; i<stringnum; i++){
		_itoa_s(i, q, 10, 10);
		fout << "s" << q << ":	.asciiz ";
		addr += strlength[i] + 1;
		for (int j = strinum[i]; j < strinum[i] + strlength[i];j++){			
			fout << str[j];
		}
		fout << endl;
	}
	addr = addr / UNIT + 1;//ȡ��
	fout << endl;
	fout << "\t.text" << endl;
	ss.str("");
	ss << addr * UNIT;
	writeAsm("li", "$fp", ss.str(),"");
	writeAsm("li", "$sp", ss.str(),"");
	writeAsm("li", "$t0", ss.str(),"");

	ss.str("");

	int locvarsize = 0;
	int lastvar1 = btab[1].lastvar;//���һ����ʶ��
	for (; lastvar1 != btab[1].lpar; lastvar1 = tab[lastvar1].link)
		if (tab[lastvar1].obj == varkind || tab[lastvar1].obj == tempvar) //�ҵ����һ���ֲ�����
			break;
	int fistvar = lastvar1;
	for (int i = lastvar1; i != btab[1].lpar; i = tab[i].link)
		if (tab[i].obj == varkind || tab[i].obj==tempvar) //��һ���ֲ�����
			fistvar = i;
	//fout << fistvar;
	//fout << lastvar1;
	if (lastvar1 == btab[1].lpar)
		locvarsize = 0;
	else
		locvarsize = tab[lastvar1].adr - tab[fistvar].adr + 4;
	ss.str("");
	ss << locvarsize;
	writeAsm("addi", "$sp", "$sp", ss.str());
	writeAsm("j", "entry","","");

	//��ʼһ��һ������Ԫʽ
	for (int i = 0; i < codenum; i++) {
		instr = code[i];
		fout << "# " << printInstruct(code[i]) << " #" << endl;

		string temp;

		if (strcmp(instr.op, (char*)"ADD") == 0 || strcmp(instr.op, (char*)"SUB") == 0 ||
			strcmp(instr.op, (char*)"MUL") == 0 || strcmp(instr.op, (char*)"DIV") == 0)
		{//lw lw op sw
			loadOperand(instr.var2, "$t0", instr.leve);//�������ڲ�� ��Ŀǰ��ƫ����
			loadOperand(instr.var3, "$t1", instr.leve);//
			temp = strcmp(instr.op, (char*)"ADD") == 0 ? "add" : strcmp(instr.op, (char*)"SUB") == 0 ? "sub" :
				   strcmp(instr.op, (char*)"MUL") == 0 ? "mul" : "div";
			writeAsm(temp, "$t0", "$t0", "$t1");
			writeAsm("sw", "$t0", loadAddress(instr.var1, instr.leve,true),"");//����ȥ���� ��Ԫʽlevel ���ص�ǰfp��ƫ����
		//	writeAsm(temp, "$t0", "$t0", "$t1");
		}
		else if (strcmp(instr.op, (char*)"NEQ") == 0)
		{
			loadOperand(instr.var2, "$t0", instr.leve);
			writeAsm("neg", "$t0", "$t0","");
			writeAsm("sw", "$t0", loadAddress(instr.var1, instr.leve, true), "");
		}
		else if (strcmp(instr.op, (char*)"BGT") == 0 || strcmp(instr.op, (char*)"LEQ") == 0 ||
			strcmp(instr.op, (char*)"EQL") == 0 || strcmp(instr.op, (char*)"NQL") == 0 ||
			strcmp(instr.op, (char*)"SMT") == 0 || strcmp(instr.op, (char*)"SEQ") == 0)
		{
			loadOperand(instr.var2, "$t0", instr.leve);
			loadOperand(instr.var3, "$t1", instr.leve);

			temp = strcmp(instr.op, (char*)"BGT") == 0 ? "bgt" : strcmp(instr.op, (char*)"LEQ") == 0 ? "bge" :
				strcmp(instr.op, (char*)"EQL") == 0 ? "beq" : strcmp(instr.op, (char*)"NQL") == 0 ? "bne" :
				strcmp(instr.op, (char*)"SMT") == 0 ? "blt" : "ble";
			ss.str("");
			ss << instr.var1->word;
			writeAsm(temp, "$t0", "$t1", ss.str());
		}
		else if (strcmp(instr.op, (char*)"JMP") == 0)
		{
			writeAsm("j", instr.var1->word,"","");
		}
		else if (strcmp(instr.op, (char*)"CALP") == 0 || strcmp(instr.op, (char*)"CALF") == 0)
		{
			writeAsm("move", "$t0", "$fp","");      // save $fp to $t0
			//writeAsm("move", "$fp", "$sp","");      // reset $fp
			writeAsm("addi", "$sp", "$sp", "4"); // alloc DL mem 
			writeAsm("sw", "$t0", "-4($sp)","");    // save last $fp in DL
			writeAsm("addi", "$sp", "$sp", "4"); // alloc return value mem
			
			dealCall(instr);
		}
		else if (strcmp(instr.op, (char*)"RET") == 0)
		{
//			int a;
			ss.str("");
			ss << (instr.leve - 1 + 2)*UNIT;// reset $sp
			writeAsm("subi", "$fp", "$fp", ss.str());
			writeAsm("move", "$sp", "$fp","");  // reset $sp
			
			writeAsm("lw", "$t0", "0($fp)",""); // save $fp in DL to $t0
			
			writeAsm("lw", "$v0", "4($fp)", ""); // load return value to $v0

			writeAsm("move", "$fp", "$t0","");  // reset $fp
			ss.str("");
			// get parameter list size
			ss << btab[tbt[instr.var1->ident]].psze;
			// reset $sp (skip parameters)
			writeAsm("sub", "$sp", "$sp", ss.str());
			// return ra
			writeAsm("jr", "$ra","","");
		}
		else if (strcmp(instr.op, (char*)"SAVE") == 0)
		{
			writeAsm("addi", "$sp", "$sp", "4");
			writeAsm("sw", "$31", "-4($sp)","");
		}
		else if (strcmp(instr.op, (char*)"PUSH") == 0)
		{
			// value parameter
			int a;
			a = atoi(instr.var2->word);
			if (tab[instr.var1->ident + a].normal==1)
				loadOperand(instr.var3, "$t0", instr.leve);
			// address parameter
			//else if (i > 0 && instrs[i - 1].op == OFF) // already got offset [OFF]!!!!!!!!!!!!!!!!!!!!!!!!!!!!!�±��޷�����
				//writeAsm("lw", "$t0", loadAddress(instr.s2, instr.lev));
			else
				writeAsm("la", "$t0", loadAddress(instr.var3, instr.leve, true), "");
			writeAsm("sw", "$t0", "0($sp)","");
			writeAsm("addi", "$sp", "$sp", "4");
			
		}
		else if ((strcmp(instr.op, (char*)"WRT") == 0))
		{
			// print string : 4, $v0 contains string print
			// $a0 : address of null-terminated string to print
			if (instr.var1->type == chuanType) {//��ӡ�ַ���
				writeAsm("li", "$v0", "4","");
				ss.str("");
				ss << "s" << instr.var1->ident;
				writeAsm("la", "$a0", ss.str(),"");
			}
			else if (tab[instr.var1->ident].typ == intkind && tab[instr.var1->ident].obj==constkind) {
				// print const integer
				writeAsm("li", "$v0", "1","");
				ss.str("");
				ss << tab[instr.var1->ident].adr;
				writeAsm("li", "$a0", ss.str(),"");
			}
			else {
				// print integer : 1, $v0 contains integer print
				// print character : 11, $v0 contains character print
				if (tab[instr.var1->ident].typ == intkind )
					writeAsm("li", "$v0", "1","");
				else writeAsm("li", "$v0", "11","");
				// constant or variable
				if (instr.var1->type== justnumber) {//������
					ss.str("");
					ss << instr.var1->word;
					writeAsm("li", "$a0", ss.str(),"");
				}
				else loadOperand(instr.var1, "$a0", instr.leve);
			}
			fout << "syscall" << endl;
		}		
		else if (strcmp(instr.op, (char*)"RED") == 0){
			// read integer : 5, $v0 contains integer read
			// read character : 12, $v0 contains character read
			if (tab[instr.var1->ident].typ == intkind )
				writeAsm("li", "$v0", "5","");
			else writeAsm("li", "$v0", "12","");
			fout << "syscall" << endl;
			// save what was read
			writeAsm("sw", "$v0", loadAddress(instr.var1, instr.leve, true), "");

		}
		else if (strcmp(instr.op, (char*)"RVAL") == 0)
		{
			// load return value to $t0
			loadOperand(instr.var1, "$t0", instr.leve);
			// rval adr = 0 + $fp

			ss.str("");
			ss << (-1)*(instr.leve - 1 + 1)*UNIT << "($fp)";
			writeAsm("sw", "$t0", ss.str(),"");
		}
		
		else if (strcmp(instr.op, (char*)"LAB") == 0)
		{
			ss.str("");
			ss << instr.var1->word;
			writeAsm(ss.str(),"","","");
		}

		else if (strcmp(instr.op, (char*)"SEL") == 0 || strcmp(instr.op, (char*)"OFF") == 0)
		{
			// load s1 addr to $t0
			writeAsm("la", "$t0", loadAddress(instr.var2, instr.leve, true), "");
			// load s2 value to $t1 (offset)
			loadOperand(instr.var3, "$t1", instr.leve);
			// calculate addr = s1.adr + offset * 4
			writeAsm("mul", "$t1", "$t1", "4");
			writeAsm("add", "$t0", "$t0", "$t1");
			// save addr
			if (strcmp(instr.op, (char*)"OFF") == 0)
				writeAsm("sw", "$t0", loadAddress(instr.var1, instr.leve, false),"");
			// save value
			
				writeAsm("lw", "$t0", "0($t0)","");
				writeAsm("sw", "$t0", loadAddress(instr.var1, instr.leve, true), "");
			
		}
		else if (strcmp(instr.op, (char*)"ASG") == 0)
		{
			loadOperand(instr.var2, "$t0", instr.leve);
			writeAsm("sw", "$t0", loadAddress(instr.var1, instr.leve, true), "");
		}
	}
	
}
void dealCall(middlecode instr)
{
	stringstream ss;
	int lowlevel = instr.leve - 1 < tab[instr.var1->ident].lev //�Ǻ������߹����� ʵ��levelΪlevel+1
		? instr.leve - 1 : tab[instr.var1->ident].lev;

	// copy SL from previous frame
	/*fout << "# copy SLs from previous frame #" << endl;
	for (int i = 0; i < lowlevel; i++) {
		ss.str("");
		ss << (i + 2) * UNIT;
		writeAsm("lw", "$t1", ss.str() + "($t0)");
		writeAsm("sw", "$t1", "0($sp)");
		writeAsm("addi", "$sp", "$sp", "4");
	}

	// if call inner block, append new static link
	fout << "# append new SL #" << endl;
	if (instr.lev - 1 <= tab[instr.d.ident].lev) {
		writeAsm("sw", "$t0", "0($sp)");
		writeAsm("addi", "$sp", "$sp", "4");
	}
	*/
	if (instr.leve == tab[instr.var1->ident].lev)//�������ڲ�
	{
		for (int i = instr.leve - 1; i>0; i--){
			ss.str("");
			ss << (-i) * UNIT;
			writeAsm("lw", "$t1", ss.str() + "($t0)","");
			writeAsm("sw", "$t1", "0($sp)","");
			writeAsm("addi", "$sp", "$sp", "4");
		}
		writeAsm("sw", "$t0", "0($sp)", "");
		writeAsm("addi", "$sp", "$sp", "4");
	}
	else if (instr.leve >= tab[instr.var1->ident].lev + 1){

		for (int i = tab[instr.var1->ident].lev; i >0; i--){
			ss.str("");
			ss << (-i) * UNIT;
			writeAsm("lw", "$t1", ss.str() + "($t0)", "");
			writeAsm("sw", "$t1", "0($sp)", "");
			writeAsm("addi", "$sp", "$sp", "4");
		}
	}
	writeAsm("move", "$fp", "$sp", "");      // reset $fp


	int locvarsize = 0;
	int lastvar1 = btab[tbt[instr.var1->ident]].lastvar;
	for (; lastvar1 != btab[tbt[instr.var1->ident]].lpar; lastvar1 = tab[lastvar1].link)
		if (tab[lastvar1].obj == varkind || tab[lastvar1].obj == tempvar) break;
	int fistvar = lastvar1;
	for (int i = lastvar1; i != btab[tbt[instr.var1->ident]].lpar; i = tab[i].link)
		if (tab[i].obj == varkind || tab[i].obj == tempvar) fistvar = i;
	if (lastvar1 == btab[tbt[instr.var1->ident]].lpar) // no local variable
		locvarsize = 0;
	else
		locvarsize = tab[lastvar1].adr - tab[fistvar].adr + 4;
	ss.str("");
	ss << locvarsize;

	writeAsm("addi", "$sp", "$sp", ss.str());
	//writeAsm("move", "$fp", "$sp", "");
	// jump and link to procedure/function
	ss.str("");
	ss << instr.var1->word;
	writeAsm("jal", ss.str(),"","");

	// after returning, restore previous status of registers
	writeAsm("lw", "$31", "-4($sp)","");
	writeAsm("subi", "$sp", "$sp", "4");
	// if calling function, save return value
	if (strcmp(instr.op,(char*)"CALF") == 0)
		writeAsm("sw", "$v0", loadAddress(instr.var2, instr.leve, true), "");

}
void loadOperand(arg* o, string r,int l){//��ƫ����
	stringstream ss;

	//TODO���ǲ���������
	if (o->type == justnumber){
		ss << o->word;
		writeAsm("li", r, ss.str(),"");
	}
	//���ǲ��ǳ���
	else if (tab[o->ident].obj == constkind) {
		ss << tab[o->ident].adr;
		writeAsm("li", r, ss.str(), "");
	}
	//���ǵĻ�ʹ��loadAddress
	else
		writeAsm("lw", r, loadAddress(o, l, true), "");
	//writeAsm("lw", r, loadAddress(o, l));
}
string loadAddress(arg* o,int l,bool val)//��ƫ����
{
	stringstream ss;
	//��ǰ��
	//1)�����
	//��ֵ����ַ
	if (tab[o->ident].lev == l) {//����
		ss.str("");
		ss << tab[o->ident].adr << "($fp)";
		// var parameter
		if (tab[o->ident].normal==0 && val ) {//����Ǵ���ַ��
			writeAsm("lw", "$t2", ss.str(),"");//�ӵ�ַ��ɵ�ַ���ֵ
			ss.str("");
			ss << "0($t2)";
		}
	}
	//���ڵ�ǰ��
	else//�ö�̬������
	{
		ss.str("");
		ss << (l-tab[o->ident].lev ) *( -1*UNIT) << "($fp)";
		writeAsm("lw", "$t2", ss.str(),"");
		if (tab[o->ident].normal==0 && val) {//����ַ
			ss.str(""); ss << tab[o->ident].adr << "($t2)";
			writeAsm("lw", "$t2", ss.str(),"");
			ss.str(""); ss << "0($t2)";
		}
		else {
			ss.str(""); 
			ss << tab[o->ident].adr << "($t2)";
		}
	}
	return ss.str();
}
void writeAsm(string op, string o1, string o2, string o3) {
	fout << op;
	if (o1 == "") { // ����label���
		//assert(o2 == "" && o3 == "");
		fout << ':' << endl;
	}
	else {
		fout << '\t' << o1;
		if (o2 != "") fout << ", " << o2;
		if (o3 != "") fout << ", " << o3;
		fout << endl;
	}
}