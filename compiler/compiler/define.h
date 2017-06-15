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
#define tempvar 100//��ʱ����

//typ:
#define intkind 105
#define charkind 106 
#define arrayint 107
#define arraychar 109
#define notype 108


#define LabType 110
#define temparg 111//��ʱ����
#define argType 112//����
#define profunclab 113
#define entrylab 114//��ں���
#define retarg 115
#define chuanType 116//�ַ���
#define tempargshuyuan 117//��ʱ����Ԫ��
#define justnumber 118//����
#define intarg 119//�м���� ����int����
#define chararg 120//

#define UNIT 4

/*#define intkind   101 /////���ͱ���
#define charkind  102 /////�ַ�����
#define constintkind 103 ////���ͳ���
#define constcharkind 104 ////�ַ�����
#define funckind 105 ////���� �з���ֵ
#define prokind 106/////procedure �޷���ֵ
#define paraintkind 107 ///����kind use or not to use��
#define paracharkind 108//char���Ͳ���
*/

typedef struct{ /////���ű�
	char name[30];
	int link;
	int  obj;////kind�������
	int  typ; //�����constint����constchar�Ļ�����ֵ��
	int ref;//�����Լ�pro��func��ֵ ����Ϊ0 pro��funcָ��ֳ����	
	int  lev;//������Ϊ1
	int  normal;//����ַΪ0 ��ֵΪ1
	int  adr;//Ŀǰû�� ��Ϊ0
}info;

typedef struct{
	int last;//�÷ֳ������һ����ʶ����λ��
	int lpar;//�÷ֳ������һ��������λ��
	int psze;//��ʱ������
	int vsze;//��ʱ������
	int lastvar;//���һ������

}btabFrame;

typedef struct{
	int eltyp;//����Ԫ������
	int length;//�����С
}arrayinfo;
typedef struct{
	char word[30];
	int ident;
	int type;
	int strnum;
}arg;
typedef struct{///��Ԫʽ�ṹ
	char op[30];
	arg *var1;//�洢�ĵط�
	arg *var2;//��һ��������
	arg *var3;//�ڶ���������
	int leve;

}middlecode;

typedef struct{
	int type;//������1��int ������2�������֡� ������3 ����ĸ��
	int num;
	int ch;
}constype;