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

T_ADD,                  T_SUB,
T_MUL,                  T_DIV,                  T_MOD,                  

T_AND,                  T_OR,                   T_NOT,

T_OPEN_PAR,             T_CLOSE_PAR
};


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
	lexer(const int psize, char* pbuf, bool pautofree);
	~lexer();

	void SetBuffer(const int psize, char* pbuf, bool pautofree);

	void StripWSpace(bool p_strip);

	void FreeBuffer();

	token* GetToken();

private:
	inline void SetMax(const int pmax, const int pno);

	inline const int Compare(const char pc) const;
	inline const int Compare(const char* ps) const;

	inline const bool IsLetter(const int off) const;
	inline const bool IsNumber(const int off) const;

	inline const int GetID();
	inline const int GetInt(const int off=0);
	inline const int GetDouble();
	inline const int GetWSpace();

	inline void EatWSpace();
	//void EatComment();
	//void EatBigComment();
	token* EatString();
	token* EatIdentifier(const bool decase);
	//token* EatInteger();
	//token* EatDouble();

	inline const char Buf(const int n) const;

	inline void NewLine();

	int max,maxno;
	int size,pos,epos;
	int line,linepos;
	int lineoffset,newlinepos;
	int prevtoken;
	char* buf;
	enum {NORMAL, MWHT} state;
	bool stripWS;
	bool autofree;
};

#endif // LEXER_H
