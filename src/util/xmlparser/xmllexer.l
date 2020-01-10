	/*** WARNING - COMMENTS IN LEX MUST BE INDENTED ***/

	/* DEFINITIONS SECTION */
	/* Everything between %{ ... %} is copied verbatim to the start of the lex generated C code. */

%{
#include <string.h>
#include "xmlparser.h"
/* #ifndef YY_FLEX_LEX_COMPAT
int yylineno = 1;
#endif */
%}

		/* Some useful regular expressions to use in the RULES section */
DIGIT		([0-9])
ALPHA		([A-Za-z])
ALPHANUMERIC	([A-Za-z0-9])
PUNCTUATION	([\,\.\:\(\)\-\!\"\'\`\^])
ARITHMATIC	([\+\-\*\/\<\>\=])
WHITESPACE	([\t ]+)
EOLN		(\n)
NUMBER		([0-9]+)
INTEGER		(-?[0-9]+)
REAL		(-?[0-9]+\.[0-9]+)
WORD		([A-Za-z]+)
QUOTEDTEXT	(\"[^"\n]*["\n])
FILENAME	([A-Za-z0-9\.][A-Za-z0-9\.\-\+\_]*)
PATHNAME	(\/?[A-Za-z0-9\.][A-Za-z0-9\.\-\+\_]*(\/[A-Za-z0-9\.][A-Za-z0-9\.\-\+\_]*)*\/?) 
HOSTNAME	([A-Za-z][A-Za-z0-9\-\_]*(\.[A-Za-z][A-Za-z0-9\-\_]*)*) 
IPADDR		([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)

	/* Special mode just for when reading in unquoted VALUE text */
%x READVALUE

	/* Use default yywrap behavior */
%option  noyywrap

	/* END OF DEFINITIONS SECTION */
%%
	/* RULES SECTION */
	/* DESCRIBE EVERY LEXICAL TOKEN/PATTERN FOLLOWED BY THE APPROPRIATE ACTION.
	   TYPICALLY THE ACTION RETURNS THE TOKEN IDENTIFIER AND ITS VALUE (IN YYLVAL) */

	/* The following simple lexical tokens only return the token identifier */

"<INSTANCE" {
	return(STARTINSTANCE);
	}

"</INSTANCE>" {
	return(ENDINSTANCE);
	}

"<PROPERTY" {
	return(STARTPROPERTY);
	}

"</PROPERTY>" {
	return(ENDPROPERTY);
	}

"<VALUE>" {
	BEGIN READVALUE; /* Start reading in unquoted VALUE text */
	return(STARTVALUE);
	}

"</VALUE>" {
	return(ENDVALUE);
	}

"<VALUE.ARRAY>" {
	return(STARTVALUEARRAY);
	}

"</VALUE.ARRAY>" {
	return(ENDVALUEARRAY);
	}

"NAME"	{
	return(NAME);
	}

"CLASSNAME" {
	return(CLASSNAME);
	}

"TYPE"	{
	return(TYPE);
	}

<<EOF>> {
	return(ENDOFFILE);
	}

	/* The following lexical tokens return the token identifier and the value of the token (in yylval) */

{QUOTEDTEXT} {
	yylval.string = (char *)strdup(yytext+1); /* Remove the start quote */
	yylval.string[strlen(yylval.string)-1] = '\0'; /* Remove the end quote */
	return(QUOTEDTEXT);
	}

	/*** Note: This rule only applies when reading in unquoted VALUE text! ***/
<READVALUE>([^<]*|(<[^/]))/"</VALUE>" {
        BEGIN INITIAL; /* Stop reading in unquoted VALUE text */
	yylval.string = (char *)strdup(yytext);
	return(UNQUOTEDTEXT);
	}

{EOLN}	{
	yylineno++;
	}

	/* Ignore whitespace */
{WHITESPACE} ;

	/* Return single character punctuation characters as-is */
.       {
	return(yytext[0]);
	}

	/* END OF RULES SECTION */
%%
	/* USER SUBROUTINE SECTION */
	/* Everything below is copied verbatim to the end of the lex generated C code. */

void yyerror(char *errmsg)
{
   fprintf(stderr, "line %d: %s at '%s'\n", yylineno, errmsg, yytext);
}

