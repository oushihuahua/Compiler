// compiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include <string>
#include "stdio.h"
#include "define.h"
#include "errorType.h"
#include <fstream> 

using namespace std;
int wordNum = 0;
int symbol;
char ch;
char tokenBuffer[82] = {0};//token缓冲区 最多放81个 
int tokenBufferPoint = 0;
int number = 0;
FILE* fin;
FILE* fout;
FILE* temp;
char filename[20];
int err = 0;//error错误数计数变量
int linenumber=1;//源程序行号 从1开始计数 打开失败变为0
extern void asmhead();
extern void main_program();
extern void printTable();
extern void printBtab();
extern void printmidcode();
extern char str[1000];
string printstring[] = { "","IDEN", "NUMBER", "STRING", "BEGINSYM", "ENDSYM", "CONSTSYM", "VARSYM", "INTSYM", "ARRAYSYM", "OFSYM", "CHARSYM", "PROSYM", "FUNCSYM", "IFSYM",
"THENSYM", "ELSESYM", "DOSYM", "WHILESYM", "FORSYM", "DOWNTOSYM", "TOSYM", "READSYM", "WRITESYM", "PLUS", "MINUS", "TIMES", "SLASH", "LBRACKET", "RBRACKET", 
"LPAREN", "RPAREN", "LSS", "GTR", "EQL", "APOSCHARAC", "QUOSTRING", "COLON", "COMMA", "SEMICOLON", "PERIOD", "BECOMES", "NEQ", "LEQ", "GEQ" };

void error(int e){
	//fprintf(fout,"error %d: 第%d行:%s\n",e,linenumer,errorde[e].c_str());
	printf("error %d: 第%d行:%s\n", e, linenumber, errorcode[e].c_str());
	err++;
}

void getsym()
{	
	void clearToken();
	void getch();
	bool isLetter();
	bool isDigit();
	int reserver();
	void catToken();
	void retract();
	bool isString();
	int transNum();
	clearToken();
	
	getch();
	while (ch == ' ' || ch == '\n' || ch == '\t'){
		if (ch == '\n')
		{
			linenumber++;
		}
		getch();		
	}
		
	if (ch == EOF)
	{
		cout << "End of a file." << endl;
		return;
	}
	if (isLetter())
	{
		while (isLetter() || isDigit())
		{
			catToken();
			getch();
		}
		retract();
		int resultValue = reserver();
		if (resultValue == 0)//没查到啊 是标志符号
		{
			symbol = IDEN;
			cout << ++wordNum << " " << printstring[symbol] << " " << tokenBuffer << endl;
		}
		else //是保留字
		{
			symbol = resultValue;
			cout << ++wordNum << " " << printstring[symbol] << " " << tokenBuffer << endl;
		}

	}
	else if (isDigit())
	{
		while (isDigit())
		{
			catToken();
			getch();
		}
		retract();
		int number = transNum();
		symbol = NUMBER;
		cout << ++wordNum << " " << printstring[symbol] << " " << number << endl;
	}
	else if (ch == ':')
	{
		getch();
		if (ch == '=')
		{
			symbol = BECOMES;
			
			cout << ++wordNum << " " << printstring[symbol] << " " << ":=" << endl;
		}
		else
		{
			retract();
			symbol = COLON;
			cout << ++wordNum << " " << printstring[symbol] << " " << ":" << endl;
		}
	}
	else if (ch == '<')
	{
		getch();
		if (ch == '=')
		{
			symbol = LEQ;
			
			cout << ++wordNum << " " << printstring[symbol] << " " << "<=" << endl;
		}
		else if (ch == '>')
		{
			symbol = NEQ;
			
			cout << ++wordNum << " " << printstring[symbol] << " " << "<>" << endl;
		}
		else
		{
			retract();
			symbol = LSS;
			cout << ++wordNum << " " << printstring[symbol] << " " << "<" << endl;
		}
	}
	else if (ch == '>')
	{
		getch();
		if (ch == '=')
		{
			symbol = GEQ;
			
			cout << ++wordNum << " " << printstring[symbol] << " " << ">=" << endl;
		}
		else
		{
			retract();
			symbol = GTR;
			cout << ++wordNum << " " << printstring[symbol] << " " << ">" << endl;
		}
	}
	else if (ch == '[')
	{
			symbol = LBRACKET;
			cout << ++wordNum << " " << printstring[symbol] << " " << "[" << endl;		
	}
	else if (ch == '\'')
	{
		catToken();
		getch();
		if (isLetter() || isDigit())
		{
			catToken();
			getch();
			if (ch == '\'')
			{
				catToken();
				symbol = APOSCHARAC;
				cout << ++wordNum << " " << printstring[symbol] << " " << tokenBuffer << endl;
			}
			else
			{
				cout << "Error: The token with \' is wrong" << endl;
				return;
			}

		}
		else
		{
			cout << "Error: The token with \' is wrong" << endl;
			return;
		}

	}
	else if (ch == '\"')
	{
		catToken();
		getch();
		while (isString())
		{
			catToken();
			getch();
		}
		if (ch == '\"')
		{
			catToken();
			symbol = QUOSTRING;
			cout << ++wordNum << " " << printstring[symbol] << " " << tokenBuffer << endl;
		}
		else
		{
			cout << "Error: The token with \" is wrong" << endl;
			return;
		}
	}
	else if (ch == '+'){ symbol = PLUS; cout << ++wordNum << " " << printstring[symbol] << " " << "+" << endl; }
	else if (ch == '-'){ symbol = MINUS; cout << ++wordNum << " " << printstring[symbol] << " " << "-" << endl; }
	else if (ch == '*'){ symbol = TIMES; cout << ++wordNum << " " << printstring[symbol] << " " << "*" << endl; }
	else if (ch == '/'){ symbol = SLASH; cout << ++wordNum << " " << printstring[symbol] << " " << "/" << endl; }
	else if (ch == ']'){ symbol = RBRACKET; cout << ++wordNum << " " << printstring[symbol] << " " << "]" << endl; }
	else if (ch == '('){ symbol = LPAREN; cout << ++wordNum << " " << printstring[symbol] << " " << "(" << endl; }
	else if (ch == ')'){ symbol = RPAREN; cout << ++wordNum << " " << printstring[symbol] << " " << ")" << endl; }
	else if (ch == '='){ symbol = EQL; cout << ++wordNum << " " << printstring[symbol] << " " << "=" << endl; }
	else if (ch == ','){ symbol = COMMA; cout << ++wordNum << " " << printstring[symbol] << " " << "," << endl; }
	else if (ch == ';'){ symbol = SEMICOLON; cout << ++wordNum << " " << printstring[symbol] << " " << ";" << endl; }
	else if (ch == '.'){ symbol = PERIOD; cout << ++wordNum << " " << printstring[symbol] << " " << "." << endl; }
	else
	{
		symbol = EOFSYM;
		cout << "Error: The token can't be identified." << endl;
	}
}
void open_file(){
	//cout << "input the file name" << endl;
	string filename;

	cin >> filename;//读入还是有小问题
	char *p = (char*)filename.data();
	fopen_s(&fin, p , "r+");
	fopen_s(&fout,"wordsanalysis.txt", "w+");
	if (fin == NULL)
	{
		cout << "the open failed" << endl;
		linenumber = 0;
	}

}
bool isString()
{
	if (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126))
		return true;
	else
		return false;
}
int transNum()
{
	number = 0;
	for (int i = 0; i < tokenBufferPoint; i++)
	{
		number = number * 10 + tokenBuffer[i]-'0';
	}
	return number;
}
int reserver()
{
	int i = 0;
	if (strcmp(tokenBuffer, "begin")==0)
		i = BEGINSYM;
	else if (strcmp(tokenBuffer, "end") == 0)
		i = ENDSYM;
	else if (strcmp(tokenBuffer, "const") == 0)
		i = CONSTSYM;
	else if (strcmp(tokenBuffer, "var") == 0)
		i = VARSYM;
	else if (strcmp(tokenBuffer, "integer") == 0)
		i = INTSYM;
	else if (strcmp(tokenBuffer, "array") == 0)
		i = ARRAYSYM;
	else if (strcmp(tokenBuffer, "of") == 0)
		i = OFSYM;
	else if (strcmp(tokenBuffer, "char") == 0)
		i = CHARSYM;
	else if (strcmp(tokenBuffer, "procedure") == 0)
		i = PROSYM;
	else if (strcmp(tokenBuffer, "function") == 0)
		i = FUNCSYM;
	else if (strcmp(tokenBuffer, "if") == 0)
		i = IFSYM;
	else if (strcmp(tokenBuffer, "then") == 0)
		i = THENSYM;
	else if (strcmp(tokenBuffer, "else") == 0)
		i = ELSESYM;
	else if (strcmp(tokenBuffer, "do") == 0)
		i = DOSYM;
	else if (strcmp(tokenBuffer, "while") == 0)
		i = WHILESYM;
	else if (strcmp(tokenBuffer, "for") == 0)
		i = FORSYM;
	else if (strcmp(tokenBuffer, "downto") == 0)
		i = DOWNTOSYM;
	else if (strcmp(tokenBuffer, "to") == 0)
		i = TOSYM;
	else if (strcmp(tokenBuffer, "read") == 0)
		i = READSYM;
	else if (strcmp(tokenBuffer, "write") == 0)
		i = WRITESYM;
	else
		i = 0;
	return i;
}
void catToken()
{
	if (tokenBufferPoint < 80)
	{
		tokenBuffer[tokenBufferPoint++] = ch;
	}
	else
	{
		cout << "Error: The token is too long" << endl;
		return;
	}
}
bool isLetter()
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	else
		return false;	
}
bool isDigit()
{
	if (ch >= '0'&&ch <= '9')
		return true;
	else
		return false;
}
void getch()//读取下一个字符
{
	ch=fgetc(fin);
}
void clearToken()
{
	int i = 0;
	while (tokenBuffer[i]!='\0')
	{
		tokenBuffer[i++] = {'\0'};
	}
	tokenBufferPoint = 0;//token缓冲区指针置为0
}
void retract()
{
	fseek(fin, -1, SEEK_CUR);//读指针往前退回一个
}
int _tmain(int argc, _TCHAR* argv[])
{
	//void open_file();
	//void getsym();
	//freopen_s(&temp,"CondsoleOut.txt", "w", stdout);//用于帮助调试
	open_file();
	main_program();
	printTable();
	printBtab();
	printmidcode();
	printf(str);
	asmhead();
}