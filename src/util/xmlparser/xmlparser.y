/* DEFINITIONS SECTION */
/* Everything between %{ ... %} is copied verbatim to the start of the parser generated C code. */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GLOBALS.h"		/* Contains _SETTINGCLASSNAME */

#define RC_OK 0
#define RC_SYNTAX_ERROR 1
#define RC_BAD_CLASSNAME 2
#define RC_SETPROPERTY_FAILED 3
#define RC_EOF EOF

extern int NFSv3setProperty( char * name, char * typename, char * valuebuffer); /* see Linux_SystemSettingUtil.c */
extern int NFSv3setArrayProperty( char * name, char * typename, char * valuebuffer); /* see Linux_SystemSettingUtil.c */

/* DEFINE ANY GLOBAL VARS HERE */
static char propertyname[_MAXNAMELENGTH];
static char propertytype[_MAXNAMELENGTH];
static int propertyisarray;
%}

/* All possible lexical token types that might be returned by the lexer */
%union {
   char * string;
}

/* DEFINE SIMPLE (UNTYPED) LEXICAL TOKENS HERE */
%token STARTINSTANCE ENDINSTANCE STARTPROPERTY ENDPROPERTY STARTVALUE ENDVALUE STARTVALUEARRAY ENDVALUEARRAY
%token NAME CLASSNAME TYPE ENDOFFILE

/* DEFINE LEXICAL TOKENS THAT RETURN A VALUE HERE, ALONG WITH THEIR RETURN TYPE */
%token <string> QUOTEDTEXT
%token <string> UNQUOTEDTEXT

/* END OF DEFINITIONS SECTION */
%%
/* RULES SECTION */

/* DESCRIBE THE STRUCTURE OF THE CONFIGURATION FILE, SYNTAX OF EACH ENTRY, ETC */

instance:	STARTINSTANCE CLASSNAME '=' QUOTEDTEXT
		{
			/* Check that this instance's classname matches the provider */
			int rc = strcmp(_SETTINGCLASSNAME,$4);
			free($4);
			if (rc) return RC_BAD_CLASSNAME; 
		}
		'>' properties ENDINSTANCE
		{
			/* Return after parsing each instance */
			return RC_OK;
		}

	|	ENDOFFILE { return RC_EOF; }
	;

properties:	property
	|	property properties
	;

property:	STARTPROPERTY NAME '=' QUOTEDTEXT TYPE '=' QUOTEDTEXT '>' 
		{
			strcpy(propertyname,$4); free($4);
			strcpy(propertytype,$7); free($7);
			propertyisarray = 0; /* default to simple property data value */
		}
		propertyvalue ENDPROPERTY
	;

propertyvalue:	simplevalue
	|	arrayvalue
	;

simplevalue:	STARTVALUE UNQUOTEDTEXT ENDVALUE
		{
			/* Check if the simple data value goes into an array or not */
			int rc;
			if (!propertyisarray) {
			   rc = NFSv3setProperty(propertyname, propertytype, $2);
			} else {
			   rc = NFSv3setArrayProperty(propertyname, propertytype, $2);
			}
                        free($2);
                        if (!rc) return RC_SETPROPERTY_FAILED;
		}
	;

arrayvalue:	STARTVALUEARRAY 
		{
			/* The subsequent simple property values will go in an array */
			propertyisarray = 1;
		}
		simplevalues ENDVALUEARRAY
	;

/* Array may contain one or more simple values */
simplevalues:	simplevalue
	|	simplevalue simplevalues
	;

/* END OF RULES SECTION */
%%
/* USER SUBROUTINE SECTION */

