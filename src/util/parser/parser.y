/* DEFINITIONS SECTION */
/* Everything between %{ ... %} is copied verbatim to the start of the parser generated C code. */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GLOBALS.h"			/* Contains the custom Linux_SystemSetting classname _SETTINGCLASSNAME */

#include "cmpidt.h"     		/* Contains CIM type definitions */
static CMPIType propertytype;			/* CIM type of the current property */

extern FILE * NFSv3yyout;
extern char * NFSv3comments;

/* DEFINE ANY GLOBAL VARS HERE */
static char pathname[_MAXNAMELENGTH];		/* Pathname of the current exported directory */
static char * entryname;			/* String containing the unique entry name; i.e. "hostname:pathname" */

static void setentryvalue(char * pathname, char * hostname);
static void startinstance (char * classname);
static void endinstance();
static void startproperty (char * name, CMPIType type);
static void endproperty();
static void setvalue (void * value);

%}

/* All possible CIM property types that can be returned by the lexer */
%union {
   CMPIBoolean          boolean;
   CMPIChar16           char16;
   CMPIUint8            uint8;
   CMPIUint16           uint16;
   CMPIUint32           uint32;
   CMPIUint64           uint64;
   CMPISint8            sint8;
   CMPISint16           sint16;
   CMPISint32           sint32;
   CMPISint64           sint64;
   CMPIReal32           real32;
   CMPIReal64           real64;
   /* Note - we override the CIM definition of string and dateTime to make these data types
      easier to handle in the lexer/parser. Both are instead implemented as simple text strings. */
   char *		string;
   char *		dateTime;
}

/* DEFINE SIMPLE (UNTYPED) LEXICAL TOKENS HERE */
%token ANONGID ANONUID

/* DEFINE LEXICAL TOKENS THAT RETURN A VALUE HERE, ALONG WITH THEIR RETURN TYPE */
%token <string> PATHNAME
%token <string> HOSTNAME
%token <string> SQUASH
%token <string> PERMISSION 
%token <boolean> SECURE
%token <boolean> SYNC
%token <boolean> WRITEDELAY
%token <boolean> HIDE
%token <boolean> SUBTREECHECK
%token <boolean> SECURELOCKS
%token <uint16> NUMBER
%token <boolean> CROSSMNT
%token <string> ALLSQUASH
%token <boolean> ACL

/* DEFINE PARSER RULE TYPES HERE */

/* END OF DEFINITIONS SECTION */
%%
/* RULES SECTION */

/* DESCRIBE THE STRUCTURE OF THE CONFIGURATION FILE, SYNTAX OF EACH ENTRY, ETC */

/* NFSv3 config file contains one or more directory entry stanzas */
stanzas:	       /* empty */
        |       stanza stanzas
	     ;

/* Each entry contains the directory name followed by one or more remote host instances */
stanza:		pathname instances

/* Save the directory name because it will apply to subsequent instances too */
pathname:	PATHNAME
			{
				strcpy(pathname,$1); free($1);
			}
	|	'"' PATHNAME '"'
			{
				strcpy(pathname,$2); free($2);
			}
	;

instances:	instance
	|	instance instances

/* Start a new instance in the output */
instance:		{
				startinstance(_SETTINGCLASSNAME);
				startproperty("Directory",CMPI_string); setvalue(pathname); endproperty();
	       		} 
		hostinfo
			{
                                /* Check if there are any comments pending */
                                if (NFSv3comments != NULL) {
                                   startproperty("Description",CMPI_string);
                                   /* Strip off the trailing newline */
                                   if (NFSv3comments[strlen(NFSv3comments)-1] == '\n') NFSv3comments[strlen(NFSv3comments)-1] = '\0';
                                   setvalue(NFSv3comments);
                                   endproperty();
                                   free(NFSv3comments); NFSv3comments = NULL;
                                }
				endinstance();
			}

/* Host info contains the remote hostname and an optional list of attributes. 
   If the hostname is missing then the entry describes the default mount attributes. */
hostinfo:       HOSTNAME '(' attributes ')'
			{
				startproperty("RemoteHost",CMPI_string); setvalue($1); endproperty();
				startproperty("SettingID",CMPI_string); setentryvalue(pathname,$1); endproperty();
				free($1);
			}
        |       '(' attributes ')'
			{
				startproperty("SettingID",CMPI_string); setentryvalue(pathname,""); endproperty();
			}
        |       HOSTNAME
	|	HOSTNAME '(' ')'
			{
				startproperty("RemoteHost",CMPI_string); setvalue($1); endproperty();
				startproperty("SettingID",CMPI_string); setentryvalue(pathname,$1); endproperty();
				free($1);
			}
        ;

/* Comma separated list of attributes */
attributes:	attribute
	|	attribute ',' attributes
	;

/* All possible attributes */
attribute:	PERMISSION		{ startproperty("Permission",CMPI_string); setvalue($1); endproperty(); free($1); }
	|	SECURE			{ startproperty("Secure",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       SQUASH			{ startproperty("Squash",CMPI_string); setvalue($1); endproperty(); free($1); }
        |       SYNC			{ startproperty("Sync",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       WRITEDELAY		{ startproperty("WriteDelay",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       HIDE			{ startproperty("Hide",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       SUBTREECHECK		{ startproperty("SubtreeCheck",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       SECURELOCKS		{ startproperty("SecureLocks",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       ANONGID '=' NUMBER	{ startproperty("AnonGID",CMPI_uint16); setvalue(&$3); endproperty(); }
        |       ANONUID '=' NUMBER	{ startproperty("AnonUID",CMPI_uint16); setvalue(&$3); endproperty(); }
        |       CROSSMNT		{ startproperty("Crossmnt",CMPI_boolean); setvalue(&$1); endproperty(); }
        |       ALLSQUASH		{ startproperty("AllSquash",CMPI_string); setvalue($1); endproperty(); free($1); }
        |       ACL		{ startproperty("Acl",CMPI_boolean); setvalue(&$1); endproperty(); }
        ;

/* END OF RULES SECTION */
%%
/* USER SUBROUTINE SECTION */

/* Short routine to write a compound SettingID value "hostname:pathname" */
static void setentryvalue(char * pathname, char * hostname)
{
   char * entryname = malloc(strlen(pathname)+strlen(hostname)+2);
   sprintf(entryname, "%s:%s", hostname, pathname);
   setvalue(entryname);
   free(entryname);
}

/* DO NOT CHANGE ANYTHING BELOW THIS LINE */

/* XML CIM output subroutines */

/* Start XML CIM instance specification */
static void startinstance (char * classname)
{
   fprintf(NFSv3yyout, "<INSTANCE CLASSNAME=\"%s\">\n", classname);
}

/* End XML CIM instance specification */
static void endinstance()
{
   fprintf(NFSv3yyout, "</INSTANCE>\n\n");
}

/* Start XML CIM property specification */
static void startproperty (char * name, CMPIType type)
{
   fprintf(NFSv3yyout, " <PROPERTY NAME=\"%s\"", name);
   propertytype = type;
   switch(propertytype) {
      case CMPI_boolean:	fprintf(NFSv3yyout, " TYPE=\"boolean\""); break;
      case CMPI_char16:		fprintf(NFSv3yyout, " TYPE=\"char16\""); break;
      case CMPI_uint8:		fprintf(NFSv3yyout, " TYPE=\"uint8\""); break;
      case CMPI_sint8:		fprintf(NFSv3yyout, " TYPE=\"sint8\""); break;
      case CMPI_uint16:		fprintf(NFSv3yyout, " TYPE=\"uint16\""); break;
      case CMPI_sint16:		fprintf(NFSv3yyout, " TYPE=\"sint16\""); break;
      case CMPI_uint32:		fprintf(NFSv3yyout, " TYPE=\"uint32\""); break;
      case CMPI_sint32:		fprintf(NFSv3yyout, " TYPE=\"sint32\""); break;
      case CMPI_uint64:		fprintf(NFSv3yyout, " TYPE=\"uint64\""); break;
      case CMPI_sint64:		fprintf(NFSv3yyout, " TYPE=\"sint64\""); break;
      case CMPI_real32:		fprintf(NFSv3yyout, " TYPE=\"real32\""); break;
      case CMPI_real64:		fprintf(NFSv3yyout, " TYPE=\"real64\""); break;
      case CMPI_string:         fprintf(NFSv3yyout, " TYPE=\"string\""); break;
      case CMPI_dateTime:       fprintf(NFSv3yyout, " TYPE=\"dateTime\""); break;
      default:			fprintf(stderr, "Unknown property type CMPIType=%d\n", propertytype);
				exit(1);
				break;
   }
   fprintf(NFSv3yyout, ">");
}

/* End XML CIM property specification */
static void endproperty()
{
   fprintf(NFSv3yyout, " </PROPERTY>\n");
}

/* Set an XML CIM value */
static void setvalue (void * value)
{
   fprintf(NFSv3yyout, " <VALUE>");
   switch(propertytype) {
      case CMPI_boolean:     	fprintf(NFSv3yyout, *((CMPIBoolean *)value)? "true":"false"); break;
      case CMPI_char16:		fprintf(NFSv3yyout, "%c", *((CMPIChar16 *)value)); break;
      case CMPI_uint8:		fprintf(NFSv3yyout, "%u", *((CMPIUint8 *)value)); break;
      case CMPI_sint8:		fprintf(NFSv3yyout, "%d", *((CMPISint8 *)value)); break;
      case CMPI_uint16:		fprintf(NFSv3yyout, "%u", *((CMPIUint16 *)value)); break;
      case CMPI_sint16:		fprintf(NFSv3yyout, "%d", *((CMPISint16 *)value)); break;
      case CMPI_uint32:		fprintf(NFSv3yyout, "%u", *((CMPIUint32 *)value)); break;
      case CMPI_sint32:		fprintf(NFSv3yyout, "%d", *((CMPISint32 *)value)); break;
      case CMPI_uint64:		fprintf(NFSv3yyout, "%u", *((CMPIUint64 *)value)); break;
      case CMPI_sint64:		fprintf(NFSv3yyout, "%d", *((CMPISint64 *)value)); break;
      case CMPI_real32:		fprintf(NFSv3yyout, "%f", *((CMPIReal32 *)value)); break;
      case CMPI_real64:		fprintf(NFSv3yyout, "%f", *((CMPIReal64 *)value)); break;
      case CMPI_string:         fprintf(NFSv3yyout, "%s", (char *)value); break;
      case CMPI_dateTime:       fprintf(NFSv3yyout, "%s", (char *)value); break;
      default:          	fprintf(stderr, "Unknown property type CMPIType=%d\n", propertytype);
				exit(1);
				break;
   }
   fprintf(NFSv3yyout, "</VALUE>");
}


/* yacc parser subroutines */

int NFSv3yyparsefile(FILE * infile, FILE * outfile)
{
   NFSv3yyrestart(infile);
   NFSv3yyout = outfile;
   return(NFSv3yyparse());
}

int main ()
{
   return NFSv3yyparse();
}

