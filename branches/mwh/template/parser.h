#ifndef PARSER_H
#define PARSER_h

/** The BNF structure of the language
 *
 * string
 * 	: T_ID 
 * 	| T_STRING
 *
 * print
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
 * 	: statement statement_list
 * 	| [empty]
 */

#endif // PARSER_H
