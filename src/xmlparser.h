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
     STARTINSTANCE = 258,
     ENDINSTANCE = 259,
     STARTPROPERTY = 260,
     ENDPROPERTY = 261,
     STARTVALUE = 262,
     ENDVALUE = 263,
     STARTVALUEARRAY = 264,
     ENDVALUEARRAY = 265,
     NAME = 266,
     CLASSNAME = 267,
     TYPE = 268,
     ENDOFFILE = 269,
     QUOTEDTEXT = 270,
     UNQUOTEDTEXT = 271
   };
#endif
/* Tokens.  */
#define STARTINSTANCE 258
#define ENDINSTANCE 259
#define STARTPROPERTY 260
#define ENDPROPERTY 261
#define STARTVALUE 262
#define ENDVALUE 263
#define STARTVALUEARRAY 264
#define ENDVALUEARRAY 265
#define NAME 266
#define CLASSNAME 267
#define TYPE 268
#define ENDOFFILE 269
#define QUOTEDTEXT 270
#define UNQUOTEDTEXT 271




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 27 "util/xmlparser/xmlparser.y"
{
   char * string;
}
/* Line 1489 of yacc.c.  */
#line 85 "xmlparser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE NFSv3xmlyylval;

