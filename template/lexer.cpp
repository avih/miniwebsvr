#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"

lexer::lexer() {
  size=0;
  pos=0;
  buf=NULL;
  stripWS=false;
}

lexer::lexer(int psize, char* pbuf) {
  buf=NULL;
  SetBuffer(psize,pbuf);
}

lexer::~lexer() {
  if (buf) delete[] buf;
}

char lexer::Buf(int n) {
  if ((n<0)||(n>=size)) return 0;
  return buf[n];
}

void lexer::StripWSpace(bool p_strip) {
  stripWS=p_strip;
}

void lexer::SetBuffer(int psize, char* pbuf) {
  size=psize;
  pos=0;
  epos=-1;
  line=1;
  linepos=0;
  lineoffset=0;
  newlinepos=0;
  state=NORMAL;
  prevtoken=-1;
  buf=pbuf;
  stripWS=false;
  
  if (buf==NULL) {
   size=0;
   return;
  }
  
}

token* lexer::GetToken() {
  token* tok=NULL;
  int opos=pos;
  
  if (pos==size) {
    if (epos < pos) {
        tok=new token();
	tok->Token=T_CDATA;
	tok->line=line;
	tok->offset=epos-linepos;
	tok->content=new char[pos-epos+1];
	memcpy(tok->content,&buf[epos],pos-epos);
	tok->content[pos-epos]=0;
	pos=opos;
	epos=pos;
	return tok;

    }
  	
    tok=new token();
    tok->Token=T_EOF;
    tok->line=line;
    tok->offset=pos-linepos;
    return tok;
  }

  max=0;
  maxno=-1;

  if (state == NORMAL) {

    SetMax(Compare("<TMPL_VAR"),T_TMPL_VAR);

    SetMax(Compare("<TMPL_IF"),T_TMPL_IF);
    SetMax(Compare("</TMPL_IF>"),T_UTMPL_IF);

    SetMax(Compare("<TMPL_ELSE>"),T_TMPL_ELSE);

    SetMax(Compare("<TMPL_LOOP"),T_TMPL_LOOP);
    SetMax(Compare("</TMPL_LOOP>"),T_UTMPL_LOOP);

    if (Compare('\n')) {
      lineoffset++;
      newlinepos=pos;
    }
  } else if (state == MWHT) {

  SetMax(Compare("=="),T_EQUAL);
  SetMax(Compare("!="),T_NOT_EQUAL);
  SetMax(Compare('='),T_EQUAL);
  SetMax(Compare('+'),T_ADD);
  SetMax(Compare('-'),T_SUB);
  SetMax(Compare('*'),T_MUL);
  SetMax(Compare('/'),T_DIV);
  SetMax(Compare('%'),T_MOD);
  SetMax(Compare('&'),T_AND);
  SetMax(Compare('|'),T_OR);
  SetMax(Compare('!'),T_NOT);
  SetMax(Compare('('),T_OPEN_PAR);
  SetMax(Compare(')'),T_CLOSE_PAR);
  SetMax(Compare('"'),T_STRING);
  SetMax(GetID(),T_ID);
  SetMax(GetInt(),1004);
  SetMax(GetDouble(),1005);
  SetMax(GetWSpace(),1006);

  SetMax(Compare('\n'),1002);

    SetMax(Compare('>'),T_TMPL_END);
  }

//  printf("epos=%d max=%d maxno=%d pos=%d char=%d\n",epos,max,maxno,pos,Buf(pos));

  if ((epos!=-1)&&(maxno!=-1)) {
bool stripped=0;
    if (stripWS) {
	int oopos=pos;
	pos=epos;
	int num=GetWSpace();
	if (num) {
		pos+=num;
		stripped=1;
	}

	epos=pos;
	pos=oopos;
    }
    if (epos<pos) {
    tok=new token();
    tok->Token=T_CDATA;
    tok->line=line;
    tok->offset=epos-linepos;
    tok->content=new char[pos-epos+1+stripped];
    tok->content[0]=' ';
    if (lineoffset) {
	line+=lineoffset;
	linepos=newlinepos;

	lineoffset=0;
    }
    memcpy(tok->content+stripped,&buf[epos],pos-epos);
    tok->content[pos-epos+stripped]=0;
    pos=opos;
    epos=-1;
    return tok;
    } else {
	pos=opos;
	epos=-1; 

	if (lineoffset) {
	  line+=lineoffset;
	  linepos=newlinepos;

	  lineoffset=0;
        }
    }

  }

  switch (maxno) {
  case -1       : if (epos==-1) epos=pos;
                  pos++;
                  return GetToken();  
case T_TMPL_VAR:
case T_TMPL_IF:
case T_TMPL_LOOP:
		state=MWHT;
		break;
  case T_TMPL_END: state=NORMAL;
  		  break;
  case T_ID     : {int omax=max;
                  max=0;

                  SetMax(Compare("eq"),T_EQUAL);
                  SetMax(Compare("ne"),T_NOT_EQUAL);

                  SetMax(Compare("lt"),T_LESS);
                  SetMax(Compare("le"),T_LESS_EQUAL);
                  SetMax(Compare("gt"),T_GREATER);
                  SetMax(Compare("ge"),T_GREATER_EQUAL);

                  SetMax(Compare("true"),T_TRUE);
                  SetMax(Compare("false"),T_FALSE);
                  SetMax(Compare("and"),T_AND);
                  SetMax(Compare("or"),T_OR);
                  SetMax(Compare("not"),T_NOT);
                  if ((maxno==T_ID)||(omax>max))
                  {
                    max=omax;
                    tok=EatIdentifier(true);
                  }}
                  break; 
  case T_STRING : tok=EatString();
                  break;
  case T_SUB    : switch (prevtoken) {
                  case T_EQUAL         :
                  case T_NOT_EQUAL     :
                  case T_LESS          :
                  case T_LESS_EQUAL    :
                  case T_GREATER       :
                  case T_GREATER_EQUAL :
                  case T_ADD           :
                  case T_SUB           :
                  case T_MUL           :
                  case T_DIV           :
                  case T_MOD           :
                  case T_INC           :
                  case T_DEC           :
                  case T_AND           :
                  case T_OR            :
                  case T_NOT           :
                  case T_OPEN_PAR      : maxno=T_UMINUS;
                  };
                  break; 
/*  case 1000     : EatComment();
                  return GetToken();
  case 1001     : EatBigComment();         
                  return GetToken();*/
  case 1002     : pos++;
                  NewLine();                             
                  return GetToken();
/*  case 1003     : pos++;
                  return GetToken();*/
/*  case 1004     : tok=EatDouble();
                  maxno=T_FLOAT;
                  break;
  case 1005     : max--;
                  tok=EatDouble();
                  maxno=T_FLOAT;
                  max++;
                  break;*/
  case 1004     :
  case 1005     : tok=EatIdentifier(false);
		  tok->Token=T_STRING;
	          break;
  case 1006     : EatWSpace();         
                  return GetToken();
  }
 
  pos=opos+max;
  prevtoken=maxno;

//  printf("Match: epos=%d max=%d maxno=%d pos=%d char=%d\n",epos,max,maxno,pos,Buf(pos));

  if (tok==NULL) {
    tok=new token();
    tok->Token=maxno;
    tok->line=line;
    tok->offset=pos-linepos;
  }  
  return tok;
}

void lexer::SetMax(int pmax, int pno) {
  if (pmax>max) {
    max=pmax;
    maxno=pno;
  }
}

int lexer::Compare(char pc) {
  return (pc==Buf(pos));
}

int lexer::Compare(char* ps) {
  for (int i=0;i<(int)strlen(ps);i++) {
    if (ps[i]!=Buf(pos+i)) return 0;
  }
  
  return strlen(ps);
}

bool lexer::IsLetter(int off) {
  if ((Buf(off)>='a')&&(Buf(off)<='z')) return true;
  if ((Buf(off)>='A')&&(Buf(off)<='Z')) return true;
  return false;
}

bool lexer::IsNumber(int off) {
  if ((Buf(off)>='0')&&(Buf(off)<='9')) return true;
  return false;
}    

int lexer::GetID() {
  int no=0;
  if (IsLetter(pos)) {
    no=1;
    while (IsLetter(pos+no) || IsNumber(pos+no) || Buf(pos+no)=='_') no++;
  }
  return no;
}

int lexer::GetInt(int off) {
  int no=off;
  while (IsNumber(pos+no)) no++;
  return no;
}

int lexer::GetDouble() {
  int no=GetInt(0);
  if (no==0) return 0;
  if (Buf(pos+no)=='.') return GetInt(no+1);
  return 0;
}

int lexer::GetWSpace() {
  int no=0;
  while (Buf(pos+no)==' ' || Buf(pos+no)=='\t' || Buf(pos+no)=='\n' || Buf(pos+no)=='\r' ) no++;
  return no;
}

void lexer::EatWSpace() {
  pos+=max;
}

void lexer::EatComment() {
  while (pos<=size) {
    pos++;
    if (Buf(pos-1)=='\n') {
      NewLine();
      return;
    } 
  }
}

void lexer::EatBigComment() {
  while (pos<=size) {
    pos++;
    if (pos<2) pos=2;
    if (Buf(pos)=='\n') NewLine();
    if (Buf(pos-2)=='*' && Buf(pos-1)=='/') return;
  }
}

token* lexer::EatString() {
  token* tok=new token();
  tok->Token=T_STRING;
  tok->line=line;
  tok->offset=pos-linepos;

  max=2;
  while (((pos+max-1)<=size)&&((Buf(pos+max-2)=='\\')||(Buf(pos+max-1)!='"'))) max++;

  tok->content = new char[max-1];
  if ((pos+1)>=size) pos=size-2;
  if ((pos+max)>size) max=size-pos;
  memcpy(tok->content,buf+pos+1,max-2);
  tok->content[max-2]=0;

  pos+=max;
  return tok;
}

token* lexer::EatIdentifier(bool decase) {
  token* tok=new token();
  tok->Token=T_ID;
  tok->line=line;
  tok->offset=pos-linepos;

  tok->content = new char[max+1];
  memcpy(tok->content,buf+pos,max);
  tok->content[max]=0;

  if (decase) {
    for (int i=0;i<max;i++) {
      if ((tok->content[i]>='A')&&(tok->content[i]<='Z')) tok->content[i]+='a'-'A';
    }
  }

  pos+=max;
  return tok;
}

/*token* lexer::EatInteger() {
  char tmp[40];
  memcpy(tmp,buf+pos,max);
  tmp[max]=0;

  token* tok=new token();
  tok->Token=T_INTEGER;
  tok->line=line;
  tok->offset=pos-linepos;
  
  tok->content.integer = atoi(tmp);
  
  pos+=max;
  return tok;
}

token* lexer::EatDouble() {
  char tmp[40];
  memcpy(tmp,buf+pos,max);
  tmp[max]=0;

  token* tok=new token();
  tok->Token=T_FLOAT;
  tok->line=line;
  tok->offset=pos-linepos;
  
  tok->content.real = strtod(tmp,NULL);
  
  pos+=max;
  return tok;
}*/

void lexer::NewLine() {
  line++;
  linepos=pos;
}

