#ifndef PARSER_H
#define PARSER_h

#include "lexer.h"

/** The BNF-like structure of the language parser
 *
 * string
 * 	: T_ID 
 * 	| T_STRING
 *
 * b_expression
 * 	: b_term [T_OR b_expression]* 
 *
 * b_term
 * 	: not_factor [T_AND b_term]* 
 *
 * not_factor
 * 	: T_NOT b_factor
 * 	| b_factor
 *
 * b_factor
 * 	: T_TRUE
 * 	| T_FALSE
 * 	| relation
 *
 * relation
 * 	: expression [(T_EQUAL|T_NOT_EQUAL|T_LESS|T_LESS_EQUAL|T_GREATER|T_GREATER_EQUAL) expression]*
 * 
 * expression
 * 	: term [(T_ADD|T_SUB) expression]*
 *
 * term
 * 	: s_factor [(T_MUL|T_DIV|T_MOD) term]*
 *
 * s_factor
 * 	: T_UMINUS string
 * 	| string
 *
 * varprint
 * 	: T_TMPL_VAR string T_TMPL_END 
 *
 *
 * elseif
 * 	: T_UTMPL_IF
 * 	| T_TMPL_ELSE statement_list T_UTMPL_IF
 *
 * if
 * 	: T_TMPL_IF expression T_TMPL_END statement_list elseif 
 *
 * loop
 * 	: T_TMPL_LOOP T_ID T_TMPL_END statement_list T_UTMPL_LOOP
 *
 * statement
 * 	: T_CDATA	 
 * 	| varprint
 * 	| if
 * 	| loop
 *
 * statement_list
 * 	: [statement]*
 */

/** The Token names
 */
extern const char* Token_STR[30];

/**
 *
 */
class parser {
public:
	parser();

	void parse(lexer *p_lex);

private:
	void gettok();

	void error(const char* msg);

	int accept(TERMINAL T);
	int expect(TERMINAL T);

	int p_string();
	int p_b_expression();
	int p_b_term();
	int p_not_factor();
	int p_b_factor();
	int p_relation();
	int p_expression();
	int p_term();
	int p_s_factor();
	int p_varprint();
	int p_elseif();
	int p_if();
	int p_loop();
	int p_statement();
	int p_statement_list();


	lexer *lex;
	token *newtok;
	token *tok;
};

#endif // PARSER_H
