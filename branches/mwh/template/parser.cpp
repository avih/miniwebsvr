#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"

/** 
 * The Token names
 */
const char* Token_STR[30] = {
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

"T_ADD",                  "T_SUB",
"T_MUL",                  "T_DIV",                  "T_MOD",

"T_AND",                  "T_OR",                   "T_NOT",

"T_OPEN_PAR",             "T_CLOSE_PAR",
};

#define retbad(x) if (!x) return 0;

parser::parser() {
}

void parser::parse(lexer *p_lex) {
	lex = p_lex;
	tok = NULL;
	firsterror = false;

	gettok();
	p_statement_list();
	if (newtok->Token != T_EOF) {
		error("Stopped parsing before EOF, at");
	}
}

void parser::gettok() {
	tok = newtok;
	newtok = lex->GetToken();
}

int parser::error(const char* msg) {
	if (firsterror) return 0;
	cout << "Error: " << msg << " \"" << Token_STR[newtok->Token] << "\", line " << newtok->line << " offset " << newtok->offset << endl;
	firsterror=true;
	return 0;
}

int parser::accept(TERMINAL T) {
	if (newtok->Token == T) {
		gettok();
		return 1;
	}
	return 0;
}

int parser::expect(TERMINAL T) {
	if (accept(T))
		return 1;
	error("Unexpected symbol");
	return 0;
}

int parser::p_string() {
	if (accept(T_ID)) {
		return 1;
	} else if (accept(T_STRING)) {
		return 1;
	} else if (accept(T_OPEN_PAR)) {
		retbad(p_expression());
		retbad(expect(T_CLOSE_PAR));
		return 1;
	}
	return 0;
}

int parser::p_b_expression() {
	if (p_b_term()) {
		if (accept(T_OR)) {
			return p_b_expression();
		}
		return 1;
	}
	error("Expected a binary expression, halted at");
	return 0;
}

int parser::p_b_term() {
	if (p_not_factor()) {
		if (accept(T_AND)) {
			return p_b_term();
		}
		return 1;
	}
	return 0;
}

int parser::p_not_factor() {
	if (accept(T_NOT)) {
		return p_b_factor();
	}
	return p_b_factor();
}

int parser::p_b_factor() {
	if (accept(T_TRUE)) {
		return 1;
	} else if (accept(T_FALSE)) {
		return 1;
	} else if (p_relation()) {
		return 1;
	}
	return 0;
}

int parser::p_relation() {
	if (p_expression()) {
		if (accept(T_EQUAL)) {
			return p_expression();
		} else if (accept(T_NOT_EQUAL)) {
			return p_expression();
		} else if (accept(T_LESS)) {
			return p_expression();
		} else if (accept(T_LESS_EQUAL)) {
			return p_expression();
		} else if (accept(T_GREATER)) {
			return p_expression();
		} else if (accept(T_GREATER_EQUAL)) {
			return p_expression();
		}
		error("Expects comparison to create boolean");
		return 0;
	}
	return 0;
}

int parser::p_expression() {
	if (p_term()) {
		if (accept(T_ADD)) {
			return p_expression();
		} else if (accept(T_SUB)) {
			return p_expression();
		}
		return 1;
	}
	error("Expected an expression, halted at");
	return 0;
}


int parser::p_term() {
	if (p_s_factor()) {
		if (accept(T_MUL)) {
			return p_term();
		} else if (accept(T_DIV)) {
			return p_term();
		} else if (accept(T_MOD)) {
			return p_term();
		}
		return 1;
	}
	return 0;
}

int parser::p_s_factor() {
	if (accept(T_UMINUS)) {
		return p_string();
	} else if (p_string()) {
		return 1;
	}
	return 0;
}

int parser::p_varprint() {
	if (accept(T_TMPL_VAR)) {
		cout << "varprint" << endl;
		retbad(p_expression());
		retbad(expect(T_TMPL_END));
		return 1;
	}
	return 0;
}

int parser::p_elseif() {
	if (accept(T_TMPL_ELSE)) {
		cout << "else clause" << endl;
		retbad(p_statement_list());
		retbad(expect(T_UTMPL_IF));
		return 1;
	} else if(expect(T_UTMPL_IF)) {
		return 1;
	}
	return 0;
}

int parser::p_if() {
	if (accept(T_TMPL_IF)) {
		cout << "if statement" << endl;
		retbad(p_b_expression());
		retbad(expect(T_TMPL_END));
		retbad(p_statement_list());
		retbad(p_elseif());
		cout << "if end" << endl;
		return 1;
	}
	return 0;
}

int parser::p_loop() {
	if (accept(T_TMPL_LOOP)) {
		cout << "loop statement" << endl;
		retbad(expect(T_ID));
		retbad(expect(T_TMPL_END));
		retbad(p_statement_list());
		retbad(expect(T_UTMPL_LOOP));
		cout << "loop end" << endl;
		return 1;
	}
	return 0;
}

int parser::p_statement() {
	if (accept(T_CDATA)) {
		cout << "inline text" << endl;
		return 1;
	} else if (p_varprint()) {
		return 1;
	} else if (p_if()) {
		return 1;
	} else if (p_loop()) {
		return 1;
	}
	return 0;
}

int parser::p_statement_list() {
	while (p_statement()) {
	} 
	return 1;
}

