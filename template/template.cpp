#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//#include "hash.h"
#include "lexer.h"
#include "parser.h"

void Parse(const string &p_filename) {
	FILE *in;
	int contentlength;
	char *buf;
	lexer lex;
	parser parse;

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

				cout << endl << "Parsing start:" << endl << endl;

				parse.parse(&lex);
				
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
/*	hash global;

	global.push("name","nick");
	global.push("bork","koos");

	cout << "name=" << global.gethash("name") << endl;
	cout << "bork=" << global.gethash("bork") << endl;
	cout << "blah=" << global.gethash("blah") << endl;*/

	Parse("test.tmpl");
}
