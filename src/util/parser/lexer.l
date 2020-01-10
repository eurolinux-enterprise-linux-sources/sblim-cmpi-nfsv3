	/*** WARNING - COMMENTS IN LEX MUST BE TAB INDENTED ***/

	/* DEFINITIONS SECTION */
	/* Everything between %{ ... %} is copied verbatim to the start of the lex generated C code. */

%{
#include <string.h>
#include "cmpidt.h"
#include "parser.h"
/* #ifndef YY_FLEX_LEX_COMPAT
int yylineno = 1;
#endif */

char * NFSv3comments = NULL;		/* String buffer to accumulate comments */
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

	/* Use the default yywrap() behavior */
%option  noyywrap

	/* END OF DEFINITIONS SECTION */
%%
	/* RULES SECTION */
	/* DESCRIBE EVERY LEXICAL TOKEN/PATTERN FOLLOWED BY THE APPROPRIATE ACTION.
	   TYPICALLY THE ACTION RETURNS THE TOKEN IDENTIFIER AND ITS VALUE (IN YYLVAL) */

	/* The following simple lexical tokens only return the token identifier */

anongid	{
	return(ANONGID);
	}

anonuid	{
	return(ANONUID);
	}

	/* The following lexical tokens return the token identifier and the value of the token (in yylval) */

no_root_squash |
root_squash {
	yylval.string = (char *)strdup(yytext);
	return(SQUASH);
	}

rw |
ro	{
	yylval.string = (char *)strdup(yytext);
	return(PERMISSION);
	}

secure |
insecure {
	yylval.boolean = !strcmp(yytext,"secure");
	return(SECURE);
	}

sync |
async	{
	yylval.boolean = !strcmp(yytext,"sync");
	return(SYNC);
	}

wdelay |
no_wdelay {
	yylval.boolean = !strcmp(yytext,"wdelay");
	return(WRITEDELAY);
	}

hide |
nohide	{
	yylval.boolean = !strcmp(yytext,"hide");
	return(HIDE);
	}

crossmnt |
nocrossmnt {
	yylval.boolean = !strcmp(yytext,"crossmnt");
	return(CROSSMNT);
	}

all_squash |
no_all_squash {
	yylval.string = (char *)strdup(yytext);
	return(ALLSQUASH);
	}

acl |
no_acl {
	yylval.boolean = !strcmp(yytext,"acl");
	return(ACL);
	}

subtree_check |
no_subtree_check {
	yylval.boolean = !strcmp(yytext,"subtree_check");
	return(SUBTREECHECK);
	}

auth_nlm |
secure_locks |
no_auth_nlm |
insecure_locks {
	yylval.boolean = !strcmp(yytext,"secure_locks") || !strcmp(yytext,"auth_nlm");
	return(SECURELOCKS);
	}
	
	/* Absolute pathname of the exported Unix directory */
\/{PATHNAME}? {
	yylval.string = (char *)strdup(yytext);
	return(PATHNAME);
	}

	/* Different ways to specify the NFS client machine name(s) */
{HOSTNAME} |
\* |
([A-Za-z\?\*][A-Za-z0-9\-\_\?\*]*)\.{HOSTNAME} |
{IPADDR} |
{IPADDR}\/({NUMBER}|{IPADDR}) |
\@[A-Za-z][A-Za-z0-9]* {
	yylval.string = (char *)strdup(yytext);
	return(HOSTNAME);
	}
 
	/* UID or GID */
{NUMBER} {
	yylval.uint16 = strtoul(yytext,NULL,0);
	return(NUMBER);
	}

{EOLN}	{
	yylineno++;
	}

	/* Ignore whitespace */
{WHITESPACE} ;

	/* Ignore end-of-line continuation markers */
\\{EOLN} {
	yylineno++;
	}

	/* Ignore comments */
\#(.*)\n {
	/* Extend the comments buffer */
	if (NFSv3comments == NULL) {
    	   NFSv3comments = malloc(strlen(yytext)+1);
	   strcpy(NFSv3comments,"");
	} else {
	   char * newcomments = malloc(strlen(NFSv3comments)+strlen(yytext)+1);
	   strcpy(newcomments, NFSv3comments);
	   free(NFSv3comments);
	   NFSv3comments = newcomments;
	}

	/* Add this comment line to the comments buffer */
	strcat(NFSv3comments,yytext);
	}

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
   fprintf(stderr, "NFSv3 parser, line %d: %s at '%s'\n", yylineno, errmsg, yytext);
}

