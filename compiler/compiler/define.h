#include <iostream>
#include <string>

using namespace std;
enum symbols{ IDEN=1, NUMBER, STRING, BEGINSYM, ENDSYM, CONSTSYM, VARSYM, INTSYM, ARRAYSYM, OFSYM, CHARSYM, PROSYM, FUNCSYM, IFSYM, THENSYM, ELSESYM, DOSYM, WHILESYM, FORSYM, DOWNTOSYM, TOSYM, READSYM, WRITESYM, PLUS, MINUS, TIMES, SLASH, LBRACKET, RBRACKET, LPAREN, RPAREN, LSS, GTR, EQL, APOSCHARAC, QUOSTRING, COLON, COMMA, SEMICOLON,PERIOD, BECOMES, NEQ, LEQ, GEQ, EOFSYM };

//obj:
#define constkind 101
#define varkind 102
#define prokind 103 
#define funckind 104
#define parakind 109
#define tempvar 100//临时变量

//typ:
#define intkind 105
#define charkind 106 
#define arrayint 107
#define arraychar 109
#define notype 108


#define LabType 110
#define temparg 111//临时变量
#define argType 112//变量
#define profunclab 113
#define entrylab 114//入口函数
#define retarg 115
#define chuanType 116//字符串
#define tempargshuyuan 117//临时数组元素
#define justnumber 118//数字
#define intarg 119//中间变量 且是int类型
#define chararg 120//

#define UNIT 4

/*#define intkind   101 /////整型变量
#define charkind  102 /////字符变量
#define constintkind 103 ////整型常量
#define constcharkind 104 ////字符常量
#define funckind 105 ////函数 有返回值
#define prokind 106/////procedure 无返回值
#define paraintkind 107 ///参数kind use or not to use？
#define paracharkind 108//char类型参数
*/

typedef struct{ /////符号表
	char name[30];
	int link;
	int  obj;////kind定义见上
	int  typ; //如果是constint或者constchar的话，有值；
	int ref;//数组以及pro和func有值 其余为0 pro和func指向分程序表	
	int  lev;//主程序为1
	int  normal;//传地址为0 传值为1
	int  adr;//目前没用 都为0
}info;

typedef struct{
	int last;//该分程序最后一个标识符的位置
	int lpar;//该分程序最后一个参数的位置
	int psze;//暂时还不用
	int vsze;//暂时还不用
	int lastvar;//最后一个变量

}btabFrame;

typedef struct{
	int eltyp;//数组元素类型
	int length;//数组大小
}arrayinfo;
typedef struct{
	char word[30];
	int ident;
	int type;
	int strnum;
}arg;
typedef struct{///四元式结构
	char op[30];
	arg *var1;//存储的地方
	arg *var2;//第一个操作数
	arg *var3;//第二个操作数
	int leve;

}middlecode;

typedef struct{
	int type;//可能是1：int 可能是2：‘数字’ 可能是3 ‘字母’
	int num;
	int ch;
}constype;