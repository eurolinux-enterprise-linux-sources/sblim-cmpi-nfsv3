/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ANONGID = 258,
     ANONUID = 259,
     PATHNAME = 260,
     HOSTNAME = 261,
     SQUASH = 262,
     PERMISSION = 263,
     SECURE = 264,
     SYNC = 265,
     WRITEDELAY = 266,
     HIDE = 267,
     SUBTREECHECK = 268,
     SECURELOCKS = 269,
     NUMBER = 270,
     CROSSMNT = 271,
     ALLSQUASH = 272,
     ACL = 273
   };
#endif
/* Tokens.  */
#define ANONGID 258
#define ANONUID 259
#define PATHNAME 260
#define HOSTNAME 261
#define SQUASH 262
#define PERMISSION 263
#define SECURE 264
#define SYNC 265
#define WRITEDELAY 266
#define HIDE 267
#define SUBTREECHECK 268
#define SECURELOCKS 269
#define NUMBER 270
#define CROSSMNT 271
#define ALLSQUASH 272
#define ACL 273




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 31 "util/parser/parser.y"
{
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
/* Line 1489 of yacc.c.  */
#line 104 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE NFSv3yylval;

