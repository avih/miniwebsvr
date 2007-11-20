#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "lexer.h"
#include "parser.h"

string blank("");

/**
 * The Token names
 */
const char* TSTR[] = {
"T_EOF",

"T_CDATA",

"T_TMPL_END",
"T_TMPL_VAR" , "T_TMPL_IF" , "T_TMPL_ELSE" , "T_TMPL_LOOP" ,
"T_UTMPL_IF", "T_UTMPL_LOOP",

"T_ID",
"T_TRUE",                 "T_FALSE",
"T_STRING",

"T_EQUAL",                "T_NOT_EQUAL",            "T_LESS",
"T_LESS_EQUAL",           "T_GREATER",              "T_GREATER_EQUAL",

"T_UMINUS",

"T_ADD",                  "T_SUB",                  "T_MUL",
"T_DIV",                  "T_MOD",                  "T_INC",
"T_DEC",

"T_AND",                  "T_OR",                   "T_NOT",

"T_OPEN_PAR",             "T_CLOSE_PAR",
};

/**
 * A string->string name->value pair
 */
class variable {
public:
	variable(const string &p_name,const string &p_value) {
		name=p_name;
		value=p_value;
	}

	string name;
	string value;
};

/** 
 * A very unpotimized & simple string->string hash implementation
 */
class hash {
public:
	const string gethash(const string &p_name) const {
		unsigned int indx;
		for (indx = 0; indx < vars.size(); ++indx) {
			if (vars[indx].name == p_name) return vars[indx].value; 
		}
		return blank;
	}

	void push(const string &p_name, const string &p_value) {
		vars.push_back(variable(p_name,p_value));
	}

private:
	vector<variable> vars;
};


void Parse(const string &p_filename) {
	FILE *in;
	int contentlength;
	char *buf;
	lexer lex;
	token *tok;

	in = fopen(p_filename.c_str(), "rb");
	if (in) {
		if (!fseek(in,0,SEEK_END)) {
			// Supports seek
			contentlength=ftell(in);
			fseek(in,0,SEEK_SET);

			buf = (char*)malloc(contentlength);
			if (buf) {
				fread(buf,contentlength,1,in);
				lex.SetBuffer(contentlength,buf,true);
				lex.StripWSpace(true);

				cout << "Parsing start:" << endl;
				
				tok = lex.GetToken();
				while (tok->Token != T_EOF) {
					cout << "Line: " << tok->line << " Offset: " << tok->offset << " Token: " << TSTR[tok->Token];
					if (tok->content) {
						cout << " Content: \"" << tok->content << "\"";
					}
					cout << endl;
					delete tok;
					tok = lex.GetToken();
				}
				delete tok;
			} else {
				cout << "Could not allocate " << contentlength << " byte buffer" << endl;
			}
		} else {
			cout << "Cannot find file size of " << p_filename << endl;
		}
		fclose(in);
	} else {
		cout << "Cannot find " << p_filename << endl;
	}
}

int main() {
	hash global;

	global.push("name","nick");
	global.push("bork","koos");

	cout << "name=" << global.gethash("name") << endl;
	cout << "bork=" << global.gethash("bork") << endl;
	cout << "blah=" << global.gethash("blah") << endl;

	Parse("test.tmpl");
}
