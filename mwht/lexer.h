#ifndef LEXER_H
#define LEXER_H

#include <string>
using namespace std;

enum TERMINAL {
T_EOF,

T_CDATA,

T_TMPL_END,
T_TMPL_VAR , T_TMPL_IF , T_TMPL_ELSE , T_TMPL_LOOP ,
T_UTMPL_IF, T_UTMPL_LOOP,

T_ID,
T_TRUE,                 T_FALSE,
T_STRING,

T_EQUAL,                T_NOT_EQUAL,            T_LESS,
T_LESS_EQUAL,           T_GREATER,              T_GREATER_EQUAL,

T_UMINUS,

T_ADD,                  T_SUB,                  T_MUL,
T_DIV,                  T_MOD,                  T_INC,
T_DEC,

T_AND,                  T_OR,                   T_NOT,

T_OPEN_PAR,             T_CLOSE_PAR,
};

//extern const char* TSTR[];

//---------------------------------------------------------------------------

class token {
public:
	token() {
		Token=0;
		line=-1;
		offset=-1;
		content=NULL;
	}
	~token() {
		if (content!=NULL) delete[] content;
	}
	int Token;
	int line;
	int offset;
	char *content;
};

//---------------------------------------------------------------------------

class lexer {
public:
	lexer();
	lexer(int psize, char* pbuf);
	~lexer();

	void SetBuffer(int psize, char* pbuf);

	void StripWSpace(bool p_strip);

	token* GetToken();

private:
	void SetMax(int pmax, int pno);

	int Compare(char pc);
	int Compare(char* ps);

	bool IsLetter(int off);
	bool IsNumber(int off);

	int GetID();
	int GetInt(int off=0);
	int GetDouble();
	int GetSingle();
	int GetWSpace();

	void EatWSpace();
	void EatComment();
	void EatBigComment();
	token* EatString();
	token* EatIdentifier();
	token* EatInteger();
	token* EatDouble();

	char Buf(int n);

	void NewLine();

	int max,maxno;
	int size,pos,epos;
	int line,linepos;
	int prevtoken;
	char* buf;
	enum {NORMAL, MWHT} state;
	bool stripWS;
};

#endif // LEXER_H
