MiniWeb Helper Templates

This is a template system to assist in generating HTML content dynamically.
One of the first useful examples I intend to create is the directory listing
template.

It will have a template "parser" (for interpreting the template) and some way
of "Compiling" the template to C++ source. (If that is not reasonably simple
to do, an option of at least exporting the internal parsed tree to source would
be cool too)

Templates will be as follows:
- Text files
- Everything will be passed through AS-IS. (Optionally the template "parser"
  may strip unneeded whitespace)
- It will not be type strict, like XSLT's. Everything will be treated as strings initially.
- I will model it after the excelent Perl module HTML::Template::Expr
- It will have scope inheritance
- It will only parse <MWHT_[command] [stuff]></MWHT_[command]> tags.
- It will aim to be small, efficient library
- For code reuse it WILL eventually share the XML internal data structure.
  (Essentially a typed recursive hash implementation)

Usage Specification:
NOTE: NAME= can be replaced by EXPR="[expression]"

Variable names MUST start with a _ or a-z
Variable names will be case-sensitive
Scope inheritance will be handled in run-time, unless the variable is preceded
by .(dot), every .(dot) indicates that the parser should start searching at
the parent. So ..name means (Use variable "name" 2 parent-levels up & onwards)

A list will follow the same naming conventions as a variable, but a list is
its own scope level, each list-item can contain several variables or lists.
A list is essentially an array of list-items.

<TMPL_VAR NAME=[var]>
Will replace it with the content of variable [var], if the variable does not
exist, nothing will be written.

<TMPL_IF NAME=[var]>
IF-construct that will execute the included code on success, optional else
<MWHT_ELSE>
can be used

<TMPL_UNLESS NAME=[var]>
Opposite of <MWHT_IF>

<TMPL_LOOP [list]>
Will loop through list.
