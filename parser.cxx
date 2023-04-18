/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

/*
 * This file is part of cxxwrap
 * Copyright (c) 1998, 1999 David Deaven (deaven@deaven.net)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Author: deaven $
 * $Id: parser.y,v 1.56 2006/12/17 04:08:11 deaven Exp $
 */

#include <ctype.h>
#include "Class.h"
#include "CType.h"
#include "PrimitiveCType.h"
#include "ObjectCType.h"
#include "FunctionCType.h"
#include "CreateCType.h"
#include "ArrayCType.h"
#include "VoidCType.h"
#include "EnumCType.h"

#define LEXER_BUFLEN 256
class Lexer {
public:
    enum {SRC_FILE, SRC_STRING} type;
    int last() { return lastc; }
    int get();
    void unget(int c);
    int getNumber(double *d);
    int matchKeyword(char* s);
    union {
      FILE *f;
      char *s;
    } source;
    int ch;		/* chars read */
    int line;	/* lines read */
    int c, lastc;	/* this character, last read character */
    int eof;
    char resultbuf[LEXER_BUFLEN];
    int ungetp;
    int ungetbuf[LEXER_BUFLEN];
};

#define MAX_CLASS_NEST 16

struct locals {
    locals() { }
    Lexer lex;
    int public_scope;
    int saved_public_scope[MAX_CLASS_NEST];
    int class_nest;
    int serial;
    int trace;
    int nerror;
    int nclassdef;
    int eatc;
    int copy_enum;
    const char* source_file;
    TypeMap typedefs;
} local;

#define YYMAXDEPTH 1000
#define yywrap() 1
#define yylex() myyylex()
#define yyerror(STR) myyyerror(STR)
static const char* msg(const char *s);
static void myyyerror(const char *s);
static int myyylex(void);
int yyparse(void);

static void global_reset();
static void push_scope(int);
static void pop_scope();
static int check_new_classdef(Class* c);


#line 161 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    CLASS = 258,                   /* CLASS  */
    STRUCT = 259,                  /* STRUCT  */
    UNION = 260,                   /* UNION  */
    TEMPLATE = 261,                /* TEMPLATE  */
    PUBLIC = 262,                  /* PUBLIC  */
    PRIVATE = 263,                 /* PRIVATE  */
    PROTECTED = 264,               /* PROTECTED  */
    VIRTUAL = 265,                 /* VIRTUAL  */
    IF = 266,                      /* IF  */
    THEN = 267,                    /* THEN  */
    ELSE = 268,                    /* ELSE  */
    STRING = 269,                  /* STRING  */
    ENUM = 270,                    /* ENUM  */
    NEW = 271,                     /* NEW  */
    DELETE = 272,                  /* DELETE  */
    NUM = 273,                     /* NUM  */
    PLUSPLUS = 274,                /* PLUSPLUS  */
    MINUSMINUS = 275,              /* MINUSMINUS  */
    EQUALSEQUALS = 276,            /* EQUALSEQUALS  */
    PLUSEQUALS = 277,              /* PLUSEQUALS  */
    MINUSEQUALS = 278,             /* MINUSEQUALS  */
    STAREQUALS = 279,              /* STAREQUALS  */
    SLASHEQUALS = 280,             /* SLASHEQUALS  */
    LEFTEQUALS = 281,              /* LEFTEQUALS  */
    RIGHTEQUALS = 282,             /* RIGHTEQUALS  */
    LEFTLEFT = 283,                /* LEFTLEFT  */
    RIGHTRIGHT = 284,              /* RIGHTRIGHT  */
    ANDAND = 285,                  /* ANDAND  */
    OROR = 286,                    /* OROR  */
    OREQUALS = 287,                /* OREQUALS  */
    ANDEQUALS = 288,               /* ANDEQUALS  */
    COLONCOLON = 289,              /* COLONCOLON  */
    ELLIPSIS = 290,                /* ELLIPSIS  */
    CTEXT = 291,                   /* CTEXT  */
    TILDE = 292,                   /* TILDE  */
    IDENTIFIER = 293,              /* IDENTIFIER  */
    INT = 294,                     /* INT  */
    FLOAT = 295,                   /* FLOAT  */
    SHORT = 296,                   /* SHORT  */
    LONG = 297,                    /* LONG  */
    DOUBLE = 298,                  /* DOUBLE  */
    VOID = 299,                    /* VOID  */
    CHAR = 300,                    /* CHAR  */
    BOOL = 301,                    /* BOOL  */
    COMMENT = 302,                 /* COMMENT  */
    CPP_DIRECTIVE = 303,           /* CPP_DIRECTIVE  */
    CONST = 304,                   /* CONST  */
    VOLATILE = 305,                /* VOLATILE  */
    REGISTER = 306,                /* REGISTER  */
    OPERATOR = 307,                /* OPERATOR  */
    UNSIGNED = 308,                /* UNSIGNED  */
    FRIEND = 309,                  /* FRIEND  */
    STATIC = 310,                  /* STATIC  */
    SIZEOF = 311,                  /* SIZEOF  */
    TYPEDEF = 312,                 /* TYPEDEF  */
    INLINE = 313,                  /* INLINE  */
    EXTERN = 314,                  /* EXTERN  */
    RETURN = 315,                  /* RETURN  */
    ARROW = 316,                   /* ARROW  */
    NOTEQUALS = 317                /* NOTEQUALS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define CLASS 258
#define STRUCT 259
#define UNION 260
#define TEMPLATE 261
#define PUBLIC 262
#define PRIVATE 263
#define PROTECTED 264
#define VIRTUAL 265
#define IF 266
#define THEN 267
#define ELSE 268
#define STRING 269
#define ENUM 270
#define NEW 271
#define DELETE 272
#define NUM 273
#define PLUSPLUS 274
#define MINUSMINUS 275
#define EQUALSEQUALS 276
#define PLUSEQUALS 277
#define MINUSEQUALS 278
#define STAREQUALS 279
#define SLASHEQUALS 280
#define LEFTEQUALS 281
#define RIGHTEQUALS 282
#define LEFTLEFT 283
#define RIGHTRIGHT 284
#define ANDAND 285
#define OROR 286
#define OREQUALS 287
#define ANDEQUALS 288
#define COLONCOLON 289
#define ELLIPSIS 290
#define CTEXT 291
#define TILDE 292
#define IDENTIFIER 293
#define INT 294
#define FLOAT 295
#define SHORT 296
#define LONG 297
#define DOUBLE 298
#define VOID 299
#define CHAR 300
#define BOOL 301
#define COMMENT 302
#define CPP_DIRECTIVE 303
#define CONST 304
#define VOLATILE 305
#define REGISTER 306
#define OPERATOR 307
#define UNSIGNED 308
#define FRIEND 309
#define STATIC 310
#define SIZEOF 311
#define TYPEDEF 312
#define INLINE 313
#define EXTERN 314
#define RETURN 315
#define ARROW 316
#define NOTEQUALS 317

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 91 "parser.y"

    String* str;
    CType* type;
    ObjectCType* objtype;
    Class* clazz;
    Number* number;
    Method* method;
    Arg* arg;
    int immediate;
    int* immediates;
    int expr;

#line 348 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);



/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_CLASS = 3,                      /* CLASS  */
  YYSYMBOL_STRUCT = 4,                     /* STRUCT  */
  YYSYMBOL_UNION = 5,                      /* UNION  */
  YYSYMBOL_TEMPLATE = 6,                   /* TEMPLATE  */
  YYSYMBOL_PUBLIC = 7,                     /* PUBLIC  */
  YYSYMBOL_PRIVATE = 8,                    /* PRIVATE  */
  YYSYMBOL_PROTECTED = 9,                  /* PROTECTED  */
  YYSYMBOL_VIRTUAL = 10,                   /* VIRTUAL  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_THEN = 12,                      /* THEN  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_STRING = 14,                    /* STRING  */
  YYSYMBOL_ENUM = 15,                      /* ENUM  */
  YYSYMBOL_NEW = 16,                       /* NEW  */
  YYSYMBOL_DELETE = 17,                    /* DELETE  */
  YYSYMBOL_NUM = 18,                       /* NUM  */
  YYSYMBOL_PLUSPLUS = 19,                  /* PLUSPLUS  */
  YYSYMBOL_MINUSMINUS = 20,                /* MINUSMINUS  */
  YYSYMBOL_EQUALSEQUALS = 21,              /* EQUALSEQUALS  */
  YYSYMBOL_PLUSEQUALS = 22,                /* PLUSEQUALS  */
  YYSYMBOL_MINUSEQUALS = 23,               /* MINUSEQUALS  */
  YYSYMBOL_STAREQUALS = 24,                /* STAREQUALS  */
  YYSYMBOL_SLASHEQUALS = 25,               /* SLASHEQUALS  */
  YYSYMBOL_LEFTEQUALS = 26,                /* LEFTEQUALS  */
  YYSYMBOL_RIGHTEQUALS = 27,               /* RIGHTEQUALS  */
  YYSYMBOL_LEFTLEFT = 28,                  /* LEFTLEFT  */
  YYSYMBOL_RIGHTRIGHT = 29,                /* RIGHTRIGHT  */
  YYSYMBOL_ANDAND = 30,                    /* ANDAND  */
  YYSYMBOL_OROR = 31,                      /* OROR  */
  YYSYMBOL_OREQUALS = 32,                  /* OREQUALS  */
  YYSYMBOL_ANDEQUALS = 33,                 /* ANDEQUALS  */
  YYSYMBOL_COLONCOLON = 34,                /* COLONCOLON  */
  YYSYMBOL_ELLIPSIS = 35,                  /* ELLIPSIS  */
  YYSYMBOL_CTEXT = 36,                     /* CTEXT  */
  YYSYMBOL_TILDE = 37,                     /* TILDE  */
  YYSYMBOL_IDENTIFIER = 38,                /* IDENTIFIER  */
  YYSYMBOL_INT = 39,                       /* INT  */
  YYSYMBOL_FLOAT = 40,                     /* FLOAT  */
  YYSYMBOL_SHORT = 41,                     /* SHORT  */
  YYSYMBOL_LONG = 42,                      /* LONG  */
  YYSYMBOL_DOUBLE = 43,                    /* DOUBLE  */
  YYSYMBOL_VOID = 44,                      /* VOID  */
  YYSYMBOL_CHAR = 45,                      /* CHAR  */
  YYSYMBOL_BOOL = 46,                      /* BOOL  */
  YYSYMBOL_COMMENT = 47,                   /* COMMENT  */
  YYSYMBOL_CPP_DIRECTIVE = 48,             /* CPP_DIRECTIVE  */
  YYSYMBOL_CONST = 49,                     /* CONST  */
  YYSYMBOL_VOLATILE = 50,                  /* VOLATILE  */
  YYSYMBOL_REGISTER = 51,                  /* REGISTER  */
  YYSYMBOL_OPERATOR = 52,                  /* OPERATOR  */
  YYSYMBOL_UNSIGNED = 53,                  /* UNSIGNED  */
  YYSYMBOL_FRIEND = 54,                    /* FRIEND  */
  YYSYMBOL_STATIC = 55,                    /* STATIC  */
  YYSYMBOL_SIZEOF = 56,                    /* SIZEOF  */
  YYSYMBOL_TYPEDEF = 57,                   /* TYPEDEF  */
  YYSYMBOL_INLINE = 58,                    /* INLINE  */
  YYSYMBOL_EXTERN = 59,                    /* EXTERN  */
  YYSYMBOL_RETURN = 60,                    /* RETURN  */
  YYSYMBOL_61_ = 61,                       /* '='  */
  YYSYMBOL_62_ = 62,                       /* '?'  */
  YYSYMBOL_63_ = 63,                       /* ':'  */
  YYSYMBOL_ARROW = 64,                     /* ARROW  */
  YYSYMBOL_65_ = 65,                       /* '|'  */
  YYSYMBOL_66_ = 66,                       /* '^'  */
  YYSYMBOL_67_ = 67,                       /* '&'  */
  YYSYMBOL_NOTEQUALS = 68,                 /* NOTEQUALS  */
  YYSYMBOL_69_ = 69,                       /* '+'  */
  YYSYMBOL_70_ = 70,                       /* '-'  */
  YYSYMBOL_71_ = 71,                       /* '*'  */
  YYSYMBOL_72_ = 72,                       /* '/'  */
  YYSYMBOL_73_ = 73,                       /* '%'  */
  YYSYMBOL_74_ = 74,                       /* '('  */
  YYSYMBOL_75_ = 75,                       /* '['  */
  YYSYMBOL_76_ = 76,                       /* ';'  */
  YYSYMBOL_77_ = 77,                       /* '<'  */
  YYSYMBOL_78_ = 78,                       /* '>'  */
  YYSYMBOL_79_ = 79,                       /* ','  */
  YYSYMBOL_80_ = 80,                       /* '{'  */
  YYSYMBOL_81_ = 81,                       /* '}'  */
  YYSYMBOL_82_ = 82,                       /* ')'  */
  YYSYMBOL_83_ = 83,                       /* '!'  */
  YYSYMBOL_84_ = 84,                       /* ']'  */
  YYSYMBOL_85_ = 85,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 86,                  /* $accept  */
  YYSYMBOL_module = 87,                    /* module  */
  YYSYMBOL_file_scope_item = 88,           /* file_scope_item  */
  YYSYMBOL_class_def = 89,                 /* class_def  */
  YYSYMBOL_template_par_list = 90,         /* template_par_list  */
  YYSYMBOL_template_par = 91,              /* template_par  */
  YYSYMBOL_class_decl = 92,                /* class_decl  */
  YYSYMBOL_93_1 = 93,                      /* $@1  */
  YYSYMBOL_94_2 = 94,                      /* $@2  */
  YYSYMBOL_95_3 = 95,                      /* $@3  */
  YYSYMBOL_96_4 = 96,                      /* $@4  */
  YYSYMBOL_parent_spec = 97,               /* parent_spec  */
  YYSYMBOL_parent_list = 98,               /* parent_list  */
  YYSYMBOL_parent_decl = 99,               /* parent_decl  */
  YYSYMBOL_class_def_body = 100,           /* class_def_body  */
  YYSYMBOL_scoped_class_def_item = 101,    /* scoped_class_def_item  */
  YYSYMBOL_class_def_item = 102,           /* class_def_item  */
  YYSYMBOL_opt_bitfield = 103,             /* opt_bitfield  */
  YYSYMBOL_opt_id = 104,                   /* opt_id  */
  YYSYMBOL_var_decl = 105,                 /* var_decl  */
  YYSYMBOL_id_list = 106,                  /* id_list  */
  YYSYMBOL_friend_decl = 107,              /* friend_decl  */
  YYSYMBOL_type_decl = 108,                /* type_decl  */
  YYSYMBOL_enum_decl = 109,                /* enum_decl  */
  YYSYMBOL_enum_item_list = 110,           /* enum_item_list  */
  YYSYMBOL_enum_item = 111,                /* enum_item  */
  YYSYMBOL_opt_var_assign_text = 112,      /* opt_var_assign_text  */
  YYSYMBOL_113_5 = 113,                    /* $@5  */
  YYSYMBOL_method_decl = 114,              /* method_decl  */
  YYSYMBOL_func = 115,                     /* func  */
  YYSYMBOL_culled_arg_list = 116,          /* culled_arg_list  */
  YYSYMBOL_cpp_operator = 117,             /* cpp_operator  */
  YYSYMBOL_unary_operator = 118,           /* unary_operator  */
  YYSYMBOL_binary_operator = 119,          /* binary_operator  */
  YYSYMBOL_assignment_operator = 120,      /* assignment_operator  */
  YYSYMBOL_id = 121,                       /* id  */
  YYSYMBOL_opt_scoped_id = 122,            /* opt_scoped_id  */
  YYSYMBOL_scoped_id = 123,                /* scoped_id  */
  YYSYMBOL_func_end = 124,                 /* func_end  */
  YYSYMBOL_125_6 = 125,                    /* $@6  */
  YYSYMBOL_func_mods = 126,                /* func_mods  */
  YYSYMBOL_opt_init_list = 127,            /* opt_init_list  */
  YYSYMBOL_init_list = 128,                /* init_list  */
  YYSYMBOL_init_item = 129,                /* init_item  */
  YYSYMBOL_arg_list = 130,                 /* arg_list  */
  YYSYMBOL_arg = 131,                      /* arg  */
  YYSYMBOL_func_arg = 132,                 /* func_arg  */
  YYSYMBOL_opt_var_assign = 133,           /* opt_var_assign  */
  YYSYMBOL_type_list = 134,                /* type_list  */
  YYSYMBOL_type = 135,                     /* type  */
  YYSYMBOL_type_mods = 136,                /* type_mods  */
  YYSYMBOL_type_mod = 137,                 /* type_mod  */
  YYSYMBOL_type_red1 = 138,                /* type_red1  */
  YYSYMBOL_array_type_indirections = 139,  /* array_type_indirections  */
  YYSYMBOL_array_type_indirection = 140,   /* array_type_indirection  */
  YYSYMBOL_type_primitive = 141,           /* type_primitive  */
  YYSYMBOL_object_type_primitive = 142,    /* object_type_primitive  */
  YYSYMBOL_type_indirection = 143,         /* type_indirection  */
  YYSYMBOL_integral_type_primitive = 144,  /* integral_type_primitive  */
  YYSYMBOL_optional_int = 145,             /* optional_int  */
  YYSYMBOL_optional_long = 146,            /* optional_long  */
  YYSYMBOL_scope_type = 147,               /* scope_type  */
  YYSYMBOL_opt_constant_expr = 148,        /* opt_constant_expr  */
  YYSYMBOL_constant_expr_list = 149,       /* constant_expr_list  */
  YYSYMBOL_opt_constant_expr_list = 150,   /* opt_constant_expr_list  */
  YYSYMBOL_c_expr = 151,                   /* c_expr  */
  YYSYMBOL_assignment_expr = 152,          /* assignment_expr  */
  YYSYMBOL_constant_expr = 153,            /* constant_expr  */
  YYSYMBOL_logic_or_expr = 154,            /* logic_or_expr  */
  YYSYMBOL_logic_and_expr = 155,           /* logic_and_expr  */
  YYSYMBOL_inclusive_or_expr = 156,        /* inclusive_or_expr  */
  YYSYMBOL_exclusive_or_expr = 157,        /* exclusive_or_expr  */
  YYSYMBOL_and_expr = 158,                 /* and_expr  */
  YYSYMBOL_equality_expr = 159,            /* equality_expr  */
  YYSYMBOL_relational_expr = 160,          /* relational_expr  */
  YYSYMBOL_shift_expr = 161,               /* shift_expr  */
  YYSYMBOL_additive_expr = 162,            /* additive_expr  */
  YYSYMBOL_multiplicative_expr = 163,      /* multiplicative_expr  */
  YYSYMBOL_cast_expr = 164,                /* cast_expr  */
  YYSYMBOL_unary_expr = 165,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 166,             /* postfix_expr  */
  YYSYMBOL_primary_expr = 167,             /* primary_expr  */
  YYSYMBOL_c_expr_list = 168,              /* c_expr_list  */
  YYSYMBOL_opt_c_expr_list = 169           /* opt_c_expr_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  84
/* YYNRULES -- Number of rules.  */
#define YYNRULES  256
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  433

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   317


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    83,     2,     2,     2,    73,    67,     2,
      74,    82,    71,    69,    79,    70,    85,    72,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    63,    76,
      77,    61,    78,    62,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    84,    66,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,    65,    81,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    64,    68
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   198,   198,   198,   201,   202,   203,   204,   205,   209,
     210,   220,   228,   229,   230,   234,   238,   246,   251,   259,
     259,   274,   274,   290,   290,   305,   305,   323,   324,   328,
     329,   337,   341,   348,   349,   353,   354,   368,   388,   389,
     399,   406,   433,   434,   435,   436,   440,   441,   445,   446,
     450,   466,   491,   498,   502,   511,   512,   513,   514,   529,
     536,   562,   569,   573,   577,   586,   587,   601,   613,   616,
     616,   623,   632,   641,   650,   659,   672,   678,   685,   695,
     709,   715,   721,   722,   722,   723,   723,   724,   724,   725,
     725,   726,   726,   727,   728,   728,   729,   733,   734,   735,
     736,   737,   738,   742,   743,   744,   745,   746,   747,   751,
     752,   753,   754,   755,   756,   757,   758,   759,   760,   761,
     764,   767,   768,   772,   773,   783,   784,   785,   785,   786,
     789,   789,   791,   791,   793,   793,   795,   798,   799,   800,
     812,   822,   830,   859,   863,   871,   877,   883,   893,   896,
     905,   909,   917,   918,   924,   932,   944,   945,   954,   955,
     956,   957,   958,   962,   963,   964,   973,   984,   992,   996,
     997,  1003,  1004,  1005,  1006,  1007,  1008,  1012,  1017,  1022,
    1027,  1037,  1038,  1042,  1046,  1050,  1054,  1060,  1060,  1061,
    1061,  1061,  1064,  1065,  1066,  1070,  1071,  1074,  1075,  1078,
    1078,  1080,  1082,  1085,  1086,  1090,  1091,  1095,  1096,  1100,
    1101,  1105,  1106,  1110,  1111,  1115,  1116,  1117,  1121,  1122,
    1123,  1124,  1125,  1129,  1130,  1131,  1135,  1136,  1137,  1142,
    1143,  1144,  1145,  1149,  1150,  1154,  1155,  1156,  1157,  1188,
    1189,  1193,  1194,  1195,  1196,  1197,  1198,  1199,  1203,  1218,
    1219,  1220,  1227,  1231,  1232,  1235,  1237
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "CLASS", "STRUCT",
  "UNION", "TEMPLATE", "PUBLIC", "PRIVATE", "PROTECTED", "VIRTUAL", "IF",
  "THEN", "ELSE", "STRING", "ENUM", "NEW", "DELETE", "NUM", "PLUSPLUS",
  "MINUSMINUS", "EQUALSEQUALS", "PLUSEQUALS", "MINUSEQUALS", "STAREQUALS",
  "SLASHEQUALS", "LEFTEQUALS", "RIGHTEQUALS", "LEFTLEFT", "RIGHTRIGHT",
  "ANDAND", "OROR", "OREQUALS", "ANDEQUALS", "COLONCOLON", "ELLIPSIS",
  "CTEXT", "TILDE", "IDENTIFIER", "INT", "FLOAT", "SHORT", "LONG",
  "DOUBLE", "VOID", "CHAR", "BOOL", "COMMENT", "CPP_DIRECTIVE", "CONST",
  "VOLATILE", "REGISTER", "OPERATOR", "UNSIGNED", "FRIEND", "STATIC",
  "SIZEOF", "TYPEDEF", "INLINE", "EXTERN", "RETURN", "'='", "'?'", "':'",
  "ARROW", "'|'", "'^'", "'&'", "NOTEQUALS", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'('", "'['", "';'", "'<'", "'>'", "','", "'{'", "'}'", "')'",
  "'!'", "']'", "'.'", "$accept", "module", "file_scope_item", "class_def",
  "template_par_list", "template_par", "class_decl", "$@1", "$@2", "$@3",
  "$@4", "parent_spec", "parent_list", "parent_decl", "class_def_body",
  "scoped_class_def_item", "class_def_item", "opt_bitfield", "opt_id",
  "var_decl", "id_list", "friend_decl", "type_decl", "enum_decl",
  "enum_item_list", "enum_item", "opt_var_assign_text", "$@5",
  "method_decl", "func", "culled_arg_list", "cpp_operator",
  "unary_operator", "binary_operator", "assignment_operator", "id",
  "opt_scoped_id", "scoped_id", "func_end", "$@6", "func_mods",
  "opt_init_list", "init_list", "init_item", "arg_list", "arg", "func_arg",
  "opt_var_assign", "type_list", "type", "type_mods", "type_mod",
  "type_red1", "array_type_indirections", "array_type_indirection",
  "type_primitive", "object_type_primitive", "type_indirection",
  "integral_type_primitive", "optional_int", "optional_long", "scope_type",
  "opt_constant_expr", "constant_expr_list", "opt_constant_expr_list",
  "c_expr", "assignment_expr", "constant_expr", "logic_or_expr",
  "logic_and_expr", "inclusive_or_expr", "exclusive_or_expr", "and_expr",
  "equality_expr", "relational_expr", "shift_expr", "additive_expr",
  "multiplicative_expr", "cast_expr", "unary_expr", "postfix_expr",
  "primary_expr", "c_expr_list", "opt_c_expr_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-266)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-178)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -266,   899,  -266,  -266,   202,    -2,    -2,   -11,  1069,    -4,
      -2,  -266,  -266,  -266,    57,   142,  -266,  -266,  -266,  -266,
    -266,  -266,  -266,   191,  -266,  1146,  -266,  -266,  -266,    14,
      47,  -266,    46,  -266,  -266,   100,    96,   -25,    27,  1146,
    -266,   336,  -266,  -266,  -266,   202,    -2,    -2,  -266,     5,
      79,    44,  -266,     1,    91,  -266,   104,    29,    -2,   138,
     105,    -2,    -2,    20,  -266,   117,  -266,  -266,  -266,  -266,
    -266,  -266,   137,    45,  -266,    54,   140,  -266,    -2,   492,
    1090,  1146,   147,  -266,  -266,    24,  -266,   336,  -266,  -266,
     128,  -266,  -266,  -266,    -2,   157,   180,  -266,  -266,   143,
    -266,   151,  -266,    -2,   149,   160,  -266,  -266,  -266,    73,
    -266,   176,    -2,  -266,    23,   134,  -266,    -2,  -266,   155,
     155,  -266,   852,  -266,  -266,  -266,  -266,   619,  -266,    54,
    -266,  -266,     9,   228,   205,   201,   208,     7,    28,   110,
      93,   178,  -266,  -266,    66,  -266,  -266,  -266,  -266,  -266,
    -266,  -266,  -266,  -266,  -266,  -266,  -266,  -266,  -266,   220,
     236,  -266,  -266,  -266,  -266,  -266,  -266,  -266,   237,  -266,
    -266,   240,    54,  -266,  -266,   232,  -266,  -266,  -266,   234,
    -266,  -266,   231,  -266,   241,  -266,    75,   243,   244,    -2,
      -2,   235,  -266,   128,  -266,    -2,    -2,    -2,   245,  -266,
     239,   957,   957,  -266,   249,    29,    -2,  -266,  -266,  -266,
      89,    -2,   233,  1090,  -266,   123,  -266,    54,  -266,  -266,
     619,  -266,   267,   246,   247,  -266,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,  -266,  -266,    -2,    54,    -2,
    -266,  -266,  -266,  -266,  -266,   242,  -266,  1090,  1090,   278,
    1090,   235,    43,  -266,  -266,  1146,   248,  -266,   252,  -266,
     253,   255,  -266,  -266,   180,   957,  -266,   202,    -2,    -2,
    -266,  -266,  -266,  1013,  1167,    -2,   417,  -266,  -266,   272,
    -266,  -266,  -266,   273,   691,    -2,    -2,    -2,   263,  -266,
    -266,   304,  -266,   259,   268,   261,  -266,   281,    54,  -266,
     228,   302,  -266,  -266,   205,   201,   208,     7,    28,    28,
     110,   110,   110,   110,    93,    93,   178,   178,  -266,  -266,
    -266,  -266,  -266,   287,   286,  -266,  -266,   288,   289,  -266,
      51,  -266,   235,  -266,  -266,   300,   957,  1146,  -266,   749,
     -21,   202,    -2,    -2,  -266,    -2,    -2,   301,  -266,  -266,
    -266,   357,   305,  -266,  -266,    -7,   157,  -266,  -266,   306,
    1090,  -266,  -266,  -266,    54,    54,  -266,   278,   278,   358,
    1146,  -266,   313,   310,  1090,   807,   315,  -266,   158,   320,
     321,   323,  -266,  -266,  -266,  -266,  1090,   318,  -266,  -266,
      51,    51,   325,   303,  -266,   331,  -266,  -266,   324,  -266,
    -266,  -266,  -266,  -266,  -266,   326,  -266,   313,   313,  -266,
    1146,    54,   373,  -266,  -266,  -266,   333,   328,  -266,  -266,
      54,  -266,  -266
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,     0,     1,     8,     0,    48,    48,     0,     0,     0,
     121,   120,   184,   171,   187,   189,   172,   174,   183,   173,
     158,   160,   161,     0,   159,     0,   162,     3,     4,     0,
     148,     5,   176,     7,    73,   123,     0,   177,   121,     0,
     156,   152,   163,   175,   169,     0,     0,     0,   176,   123,
     177,   164,    23,   123,   178,    25,   179,     0,   121,   123,
     177,   121,     0,     0,    71,     0,   188,   185,   190,   191,
     186,   170,     0,     0,    11,     0,     0,    62,   122,     0,
     137,     0,     0,    50,    74,    53,   157,   153,   182,   181,
     154,   165,   178,   179,     0,    27,     0,    14,    19,     0,
      12,     0,    13,     0,     0,    15,    18,    72,    75,     0,
      65,    68,     0,    61,     0,     0,   251,     0,   250,     0,
       0,   101,     0,    97,    99,   100,    98,     0,   102,     0,
     248,   149,   203,   205,   207,   209,   211,   213,   215,   218,
     223,   226,   229,   233,   235,   241,     6,   124,    94,    95,
      83,    84,    89,   113,   114,   110,   111,    91,    92,    85,
      86,    87,    88,   119,   117,   109,    93,   105,   108,    90,
     103,   104,   195,   106,   107,     0,    80,    81,    82,     0,
      96,   144,     0,    79,   138,   143,   148,     0,   150,     0,
       0,    51,   166,   155,    21,     0,     0,     0,    28,    29,
       0,     0,     0,    17,     0,     0,     0,    64,    69,    67,
       0,     0,     0,   137,    59,     0,   249,     0,   236,   237,
       0,   239,   248,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   246,   247,     0,   255,     0,
     242,   115,   116,   118,   112,     0,   196,   137,   137,   130,
     137,     0,   148,   140,   180,     0,     0,    54,    53,   167,
       0,    31,    33,    34,     0,     0,    45,     0,    48,    48,
     192,   194,   193,   121,     0,    48,     0,    35,    37,    46,
      42,    41,    40,     0,     0,     0,    48,    48,     0,    16,
      66,     0,    63,     0,     0,     0,    60,     0,     0,   252,
     206,     0,   201,   202,   208,   210,   212,   214,   216,   217,
     221,   222,   219,   220,   224,   225,   227,   228,   230,   231,
     232,   245,   253,   256,     0,   244,   168,     0,     0,   131,
     132,   139,   142,   141,   151,     0,     0,     0,    30,     0,
     177,     0,     0,     0,    58,   121,    48,     0,    49,    24,
      36,     0,     0,    38,    26,   123,    27,    10,    70,     0,
     137,   145,   240,   234,     0,     0,   243,   130,   130,     0,
       0,   125,    76,     0,   137,     0,     0,    20,   177,   178,
     179,     0,    43,    47,    39,     9,   137,     0,   204,   254,
     132,   132,     0,   133,   134,     0,   126,   127,     0,    22,
      32,    55,    56,    57,    44,     0,   146,    77,    78,   129,
       0,   199,     0,    52,   147,   135,   200,     0,   197,   128,
       0,   136,   198
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -266,  -266,  -266,  -266,   209,  -266,     0,  -266,  -266,  -266,
    -266,   322,  -266,   141,  -180,  -265,  -266,  -266,    17,   415,
     250,  -266,   422,    13,   317,   224,  -266,  -266,    12,    -8,
    -194,  -266,   352,  -266,  -266,   299,  -266,    -1,  -140,  -266,
    -115,  -266,  -266,    15,   173,  -266,   -15,  -160,  -240,     8,
      10,   -28,     2,  -100,   -62,  -266,  -266,  -266,   411,  -266,
    -266,  -266,  -266,  -266,  -266,  -266,  -228,   -63,  -266,   210,
     211,   212,   207,   213,    39,   -91,    48,    49,  -111,    38,
    -266,  -266,  -266,  -266
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    27,    28,   104,   105,   285,   200,   270,    99,
     101,    98,   198,   199,   286,   287,   288,   362,    52,   289,
      83,   290,   291,    48,   109,   110,   209,   301,   292,    34,
     182,   175,   129,   177,   178,    59,    36,   130,   382,   422,
     340,   383,   403,   404,   183,   184,   185,    76,   187,    38,
      39,    40,    41,   191,   192,    42,    43,    91,    44,    67,
      70,   293,   255,   426,   427,   311,   312,   224,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   333,   334
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,    29,    64,    50,    54,    56,    51,    60,    63,    65,
      72,    86,   131,    33,    32,   215,    61,   180,   225,   305,
     332,   360,   294,    55,    60,   344,   263,    94,   232,   360,
      84,    11,   103,    73,    11,    94,    11,    65,    60,    94,
     226,    87,    96,    11,    60,    92,    93,    51,    95,    80,
     107,    90,    81,   108,   234,   235,    81,    65,    78,   -27,
      65,    11,    86,   337,   338,    11,    57,    11,   116,   395,
     117,   227,   118,   119,   120,   233,    62,   147,    60,    60,
      60,   -49,   250,    11,   360,   245,   246,   179,   186,   188,
      74,   121,    11,   147,   211,   349,    66,   193,  -123,   172,
     112,    82,   343,   190,    75,   236,   237,   386,    75,   256,
     122,    88,   379,    11,   380,    89,   216,   213,   -49,   114,
     360,   123,    77,   124,   125,   126,   222,   381,   127,   269,
     247,   328,   329,   330,    78,   223,    75,   128,   238,   239,
     248,   172,    96,   320,   321,   322,   323,   399,    79,   114,
     -48,   249,   206,   269,   207,    97,    81,   218,   219,   -27,
     221,   342,   240,   241,   313,    86,   385,   100,   206,   116,
     302,   117,    94,   118,   119,   120,   397,    20,    21,    22,
     102,    68,    81,    24,    69,   313,    26,   195,   196,   197,
     408,    80,   121,    11,   271,   272,   273,   373,   172,   306,
      37,    37,   415,   261,   298,    45,    46,    47,   213,   172,
     214,   122,    60,   113,    32,    32,   146,     9,   189,   222,
      96,   186,   123,   201,   124,   125,   126,   204,   307,   217,
      12,   202,    14,    15,   411,    81,    18,   208,   128,   205,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   242,
     243,   244,   295,   296,   297,    23,    60,    60,   228,    60,
     417,   418,   400,   401,    60,   186,   186,   230,   186,    72,
     229,   318,   319,   188,    37,   231,   350,    92,    93,    51,
     269,   251,    37,    60,   356,    37,   324,   325,    32,   326,
     327,   355,    73,    37,   366,   354,    55,   252,   253,    32,
      35,   254,   357,    49,    53,    53,   257,    32,   258,    35,
     172,   398,   313,   259,    55,   304,  -177,  -177,  -177,   275,
     260,   264,  -177,   265,   274,  -177,   336,   339,   308,   309,
     345,   190,   347,   346,  -177,   361,   363,    85,  -177,   367,
     368,   369,   370,   371,    81,    37,    60,    84,    37,  -177,
     388,   389,   390,    51,    65,   188,   106,    35,   428,    32,
      35,   111,    32,   372,    95,   374,   375,   432,   376,    60,
     377,   378,   115,   391,   384,   393,   402,   392,   186,    60,
     396,   394,   420,    60,    37,    20,    21,    22,   405,   406,
     407,    24,   186,   410,    26,    60,   412,   413,    32,   414,
     416,   419,   203,    88,   186,   421,   423,    89,   424,   429,
     431,   111,   430,   212,   299,   348,    30,   194,   276,    60,
     277,   278,   279,    31,   280,   281,   282,     8,   405,   210,
     300,   176,     9,   341,    71,   425,   310,   316,     0,   314,
     267,   315,     0,     0,   317,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,  -121,
      23,   283,    24,     0,   284,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   262,     0,     0,   266,   268,
       0,     0,     0,     0,     0,    45,    46,    47,   359,     0,
      35,    35,     0,     0,   106,   111,     0,     9,   148,   149,
     303,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,     0,     0,     0,   121,
      11,    12,    13,    14,    15,    16,    17,    18,    19,     0,
       0,    20,    21,    22,     0,    23,   331,    24,   335,     0,
      26,     0,     0,   165,     0,     0,   166,   167,   168,   123,
     169,   124,   125,   126,   170,   171,     0,   172,     0,   173,
     174,     0,     0,     0,    35,   128,    49,    53,    53,     0,
       0,     0,    35,     0,   358,    35,     0,     0,     0,     0,
       0,     0,     0,    35,   365,   358,   358,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,   116,     9,   117,     0,   118,   119,   120,
       0,     0,     0,     0,     0,    35,     0,     0,    35,     0,
       0,     0,     0,     0,    35,   358,   121,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    20,    21,
      22,     0,    23,     0,    24,   122,     0,    26,     0,     0,
       0,     0,     0,     0,    35,     0,   123,     0,   124,   125,
     126,     0,   276,   127,   277,   278,   279,     0,   280,   281,
     282,     8,   128,     0,     0,     0,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,     0,     0,
      20,    21,    22,  -121,    23,   283,    24,     0,   284,    26,
     276,     0,   277,   278,   279,     0,   280,   281,   282,     8,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
       0,     0,   364,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    20,    21,
      22,  -121,    23,   283,    24,     0,   284,    26,   276,     0,
     277,   278,   279,     0,   280,   281,   282,     8,     0,     0,
       0,     0,     9,     0,     0,     0,     0,     0,     0,     0,
     387,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,  -121,
      23,   283,    24,     0,   284,    26,   116,     0,   117,     0,
     118,   119,   120,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   409,   121,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       3,     0,     4,     5,     6,     7,     0,     0,   122,     8,
       0,     0,     0,     0,     9,     0,     0,     0,     0,   123,
       0,   124,   125,   126,     0,     0,   220,     0,     0,     0,
       0,     0,     0,     0,     0,   128,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    20,    21,
      22,  -121,    23,     0,    24,     0,    25,    26,   276,     0,
     277,   278,   279,     0,   280,   281,   282,     8,     0,     0,
       0,     0,     9,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,  -121,
      23,   283,    24,     0,   284,    26,   351,   352,   353,     0,
       0,     0,     0,     8,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
       0,     0,    20,    21,    22,     0,    23,     0,    24,     0,
       0,    26,    45,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     0,
       0,     0,     0,    45,    46,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    58,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     0,     0,    20,    21,
      22,     0,    23,     0,    24,   181,     0,    26,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    20,
      21,    22,     0,    23,     0,    24,     0,     0,    26,    45,
      46,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,     0,     0,     0,     0,     0,     0,     0,     0,
     277,   278,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,     0,     0,    20,    21,    22,     0,    23,
       0,    24,     0,     0,    26,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,     0,
      23,     0,    24,     0,     0,    26
};

static const yytype_int16 yycheck[] =
{
       1,     1,    10,     4,     5,     6,     4,     8,     9,    10,
      25,    39,    75,     1,     1,   115,     8,    79,   129,   213,
     248,   286,   202,     6,    25,   265,   186,    34,    21,   294,
      38,    38,     3,    25,    38,    34,    38,    38,    39,    34,
      31,    39,    63,    38,    45,    46,    47,    45,    49,    74,
      58,    41,    77,    61,    26,    27,    77,    58,    34,    80,
      61,    38,    90,   257,   258,    38,    77,    38,    14,    76,
      16,    62,    18,    19,    20,    68,    80,    78,    79,    80,
      81,    80,   144,    38,   349,    19,    20,    79,    80,    81,
      76,    37,    38,    94,    71,   275,    39,    87,    74,    75,
      80,    74,   262,    79,    61,    77,    78,   347,    61,   172,
      56,    67,    61,    38,    63,    71,   117,    74,    75,    74,
     385,    67,    76,    69,    70,    71,   127,    76,    74,   191,
      64,   242,   243,   244,    34,   127,    61,    83,    28,    29,
      74,    75,    63,   234,   235,   236,   237,   375,    52,    74,
      75,    85,    79,   215,    81,    76,    77,   119,   120,    80,
     122,   261,    69,    70,   227,   193,   346,    76,    79,    14,
      81,    16,    34,    18,    19,    20,   370,    49,    50,    51,
      76,    39,    77,    55,    42,   248,    58,     7,     8,     9,
     384,    74,    37,    38,   195,   196,   197,   308,    75,    76,
     201,   202,   396,   186,   204,     3,     4,     5,    74,    75,
      76,    56,   213,    76,   201,   202,    76,    15,    71,   220,
      63,   213,    67,    80,    69,    70,    71,    78,   220,    74,
      39,    80,    41,    42,    76,    77,    45,    61,    83,    79,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    71,
      72,    73,     3,     4,     5,    53,   257,   258,    30,   260,
     400,   401,   377,   378,   265,   257,   258,    66,   260,   284,
      65,   232,   233,   265,   275,    67,   277,   278,   279,   277,
     342,    61,   283,   284,   284,   286,   238,   239,   275,   240,
     241,   283,   284,   294,   295,   283,   279,    61,    61,   286,
       1,    61,   285,     4,     5,     6,    74,   294,    74,    10,
      75,   374,   375,    82,   297,    82,    49,    50,    51,    80,
      79,    78,    55,    79,    79,    58,    84,    49,    82,    82,
      82,    79,    77,    80,    67,    63,    63,    38,    71,    76,
      36,    82,    74,    82,    77,   346,   347,   355,   349,    82,
     351,   352,   353,   351,   355,   347,    57,    58,   421,   346,
      61,    62,   349,    82,   365,    63,    79,   430,    82,   370,
      82,    82,    73,   356,    74,    18,    18,    76,   370,   380,
      74,    76,    79,   384,   385,    49,    50,    51,   380,    76,
      80,    55,   384,    78,    58,   396,    76,    76,   385,    76,
      82,    76,   103,    67,   396,    74,    82,    71,    82,    36,
      82,   112,    79,   114,   205,   274,     1,    95,     1,   420,
       3,     4,     5,     1,     7,     8,     9,    10,   420,   112,
     206,    79,    15,   260,    23,   420,   226,   230,    -1,   228,
     190,   229,    -1,    -1,   231,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,   189,   190,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    81,    -1,
     201,   202,    -1,    -1,   205,   206,    -1,    15,    16,    17,
     211,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    -1,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    -1,
      -1,    49,    50,    51,    -1,    53,   247,    55,   249,    -1,
      58,    -1,    -1,    61,    -1,    -1,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    -1,    75,    -1,    77,
      78,    -1,    -1,    -1,   275,    83,   277,   278,   279,    -1,
      -1,    -1,   283,    -1,   285,   286,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   294,   295,   296,   297,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,   346,    -1,    -1,   349,    -1,
      -1,    -1,    -1,    -1,   355,   356,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    -1,    55,    56,    -1,    58,    -1,    -1,
      -1,    -1,    -1,    -1,   385,    -1,    67,    -1,    69,    70,
      71,    -1,     1,    74,     3,     4,     5,    -1,     7,     8,
       9,    10,    83,    -1,    -1,    -1,    15,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    -1,    57,    58,
       1,    -1,     3,     4,     5,    -1,     7,     8,     9,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    -1,    57,    58,     1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,    14,    -1,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    37,
      38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,     4,     5,     6,    -1,    -1,    56,    10,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    67,
      -1,    69,    70,    71,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    52,    53,    -1,    55,    -1,    57,    58,     1,    -1,
       3,     4,     5,    -1,     7,     8,     9,    10,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    -1,    57,    58,     3,     4,     5,    -1,
      -1,    -1,    -1,    10,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    49,    50,    51,    -1,    53,    -1,    55,    -1,
      -1,    58,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    49,    50,
      51,    -1,    53,    -1,    55,    35,    -1,    58,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    49,
      50,    51,    -1,    53,    -1,    55,    -1,    -1,    58,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    -1,    49,    50,    51,    -1,    53,
      -1,    55,    -1,    -1,    58,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    49,    50,    51,    -1,
      53,    -1,    55,    -1,    -1,    58
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    87,     0,     1,     3,     4,     5,     6,    10,    15,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    55,    57,    58,    88,    89,    92,
     105,   108,   109,   114,   115,   121,   122,   123,   135,   136,
     137,   138,   141,   142,   144,     3,     4,     5,   109,   121,
     123,   138,   104,   121,   123,   104,   123,    77,    37,   121,
     123,   135,    80,   123,   115,   123,    39,   145,    39,    42,
     146,   144,   132,   135,    76,    61,   133,    76,    34,    52,
      74,    77,    74,   106,   115,   121,   137,   138,    67,    71,
     136,   143,   123,   123,    34,   123,    63,    76,    97,    95,
      76,    96,    76,     3,    90,    91,   121,   115,   115,   110,
     111,   121,    80,    76,    74,   121,    14,    16,    18,    19,
      20,    37,    56,    67,    69,    70,    71,    74,    83,   118,
     123,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    76,   123,    16,    17,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    61,    64,    65,    66,    68,
      72,    73,    75,    77,    78,   117,   118,   119,   120,   135,
     140,    35,   116,   130,   131,   132,   135,   134,   135,    71,
      79,   139,   140,   136,    97,     7,     8,     9,    98,    99,
      93,    80,    80,   121,    78,    79,    79,    81,    61,   112,
     110,    71,   121,    74,    76,   139,   123,    74,   165,   165,
      74,   165,   123,   135,   153,   164,    31,    62,    30,    65,
      66,    67,    21,    68,    26,    27,    77,    78,    28,    29,
      69,    70,    71,    72,    73,    19,    20,    64,    74,    85,
     140,    61,    61,    61,    61,   148,   153,    74,    74,    82,
      79,   104,   121,   133,    78,    79,   121,   106,   121,   140,
      94,   123,   123,   123,    79,    80,     1,     3,     4,     5,
       7,     8,     9,    54,    57,    92,   100,   101,   102,   105,
     107,   108,   114,   147,   100,     3,     4,     5,    92,    90,
     111,   113,    81,   121,    82,   116,    76,   135,    82,    82,
     155,   151,   152,   153,   156,   157,   158,   159,   160,   160,
     161,   161,   161,   161,   162,   162,   163,   163,   164,   164,
     164,   121,   152,   168,   169,   121,    84,   116,   116,    49,
     126,   130,   139,   133,   134,    82,    80,    77,    99,   100,
     123,     3,     4,     5,   114,   135,    92,   104,   121,    81,
     101,    63,   103,    63,    81,   121,   123,    76,    36,    82,
      74,    82,    82,   164,    63,    79,    82,    82,    82,    61,
      63,    76,   124,   127,    74,   100,   134,    81,   123,   123,
     123,   104,    76,    18,    76,    76,    74,   116,   153,   152,
     126,   126,    18,   128,   129,   135,    76,    80,   116,    81,
      78,    76,    76,    76,    76,   116,    82,   124,   124,    76,
      79,    74,   125,    82,    82,   129,   149,   150,   153,    36,
      79,    82,   153
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    87,    87,    88,    88,    88,    88,    88,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    93,
      92,    94,    92,    95,    92,    96,    92,    97,    97,    98,
      98,    99,    99,    99,    99,   100,   100,   101,   102,   102,
     102,   102,   102,   102,   102,   102,   103,   103,   104,   104,
     105,   105,   105,   106,   106,   107,   107,   107,   107,   108,
     108,   108,   108,   109,   109,   110,   110,   111,   112,   113,
     112,   114,   114,   114,   114,   114,   115,   115,   115,   116,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   118,   118,   118,
     118,   118,   118,   119,   119,   119,   119,   119,   119,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     121,   122,   122,   123,   123,   124,   124,   125,   124,   124,
     126,   126,   127,   127,   128,   128,   129,   130,   130,   130,
     131,   131,   131,   131,   131,   132,   132,   132,   133,   133,
     134,   134,   135,   135,   135,   135,   136,   136,   137,   137,
     137,   137,   137,   138,   138,   138,   139,   139,   140,   141,
     141,   141,   141,   141,   141,   141,   141,   142,   142,   142,
     142,   143,   143,   144,   144,   144,   144,   145,   145,   146,
     146,   146,   147,   147,   147,   148,   148,   149,   149,   150,
     150,   151,   152,   153,   153,   154,   154,   155,   155,   156,
     156,   157,   157,   158,   158,   159,   159,   159,   160,   160,
     160,   160,   160,   161,   161,   161,   162,   162,   162,   163,
     163,   163,   163,   164,   164,   165,   165,   165,   165,   165,
     165,   166,   166,   166,   166,   166,   166,   166,   167,   167,
     167,   167,   167,   168,   168,   169,   169
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     3,     1,     1,     7,
       6,     2,     3,     3,     3,     1,     3,     2,     1,     0,
       7,     0,     8,     0,     6,     0,     6,     0,     2,     1,
       3,     2,     5,     2,     2,     1,     2,     1,     2,     3,
       1,     1,     1,     3,     4,     1,     0,     2,     0,     1,
       2,     3,     8,     1,     3,     4,     4,     4,     2,     4,
       5,     3,     2,     5,     4,     1,     3,     2,     0,     0,
       3,     2,     3,     1,     2,     3,     6,     8,     8,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     2,     2,     1,     2,     1,
       1,     0,     2,     1,     3,     1,     2,     0,     4,     3,
       0,     1,     0,     2,     1,     3,     4,     0,     1,     3,
       2,     3,     3,     1,     1,     5,     7,     8,     0,     2,
       1,     3,     1,     2,     2,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     2,     3,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       4,     1,     1,     1,     1,     2,     2,     0,     1,     0,
       1,     1,     1,     1,     1,     0,     1,     1,     3,     0,
       1,     1,     1,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     4,     1,     2,     2,     2,     2,
       4,     1,     2,     4,     3,     3,     2,     2,     1,     2,
       1,     1,     3,     1,     3,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* file_scope_item: class_def  */
#line 201 "parser.y"
                  { }
#line 1982 "y.tab.c"
    break;

  case 5: /* file_scope_item: type_decl  */
#line 202 "parser.y"
                  { if ((yyvsp[0].type)->alias) global.typedefs.put((yyvsp[0].type)->alias, (yyvsp[0].type)); }
#line 1988 "y.tab.c"
    break;

  case 6: /* file_scope_item: var_decl opt_var_assign ';'  */
#line 203 "parser.y"
                                    { }
#line 1994 "y.tab.c"
    break;

  case 7: /* file_scope_item: method_decl  */
#line 204 "parser.y"
                    { }
#line 2000 "y.tab.c"
    break;

  case 8: /* file_scope_item: error  */
#line 205 "parser.y"
              { /* best recovery is with no action */ }
#line 2006 "y.tab.c"
    break;

  case 9: /* class_def: TEMPLATE '<' template_par_list '>' CLASS id ';'  */
#line 209 "parser.y"
                                                        { }
#line 2012 "y.tab.c"
    break;

  case 10: /* class_def: TEMPLATE '<' template_par_list '>' class_decl ';'  */
#line 211 "parser.y"
        {
	    (yyvsp[-1].clazz)->comment = (yyvsp[-5].str)->comment;
	    (yyvsp[-1].clazz)->setParameters((yyvsp[-3].arg));
	    if (0 == check_new_classdef((yyvsp[-1].clazz)))
	    {
		global.classJar.add((yyvsp[-1].clazz));
		local.nclassdef++;
	    }
	}
#line 2026 "y.tab.c"
    break;

  case 11: /* class_def: class_decl ';'  */
#line 221 "parser.y"
        {
	    if (0 == check_new_classdef((yyvsp[-1].clazz)))
	    {
		global.classJar.add((yyvsp[-1].clazz));
		local.nclassdef++;
	    }
	}
#line 2038 "y.tab.c"
    break;

  case 12: /* class_def: STRUCT scoped_id ';'  */
#line 228 "parser.y"
                             { }
#line 2044 "y.tab.c"
    break;

  case 13: /* class_def: UNION scoped_id ';'  */
#line 229 "parser.y"
                            { }
#line 2050 "y.tab.c"
    break;

  case 14: /* class_def: CLASS scoped_id ';'  */
#line 230 "parser.y"
                            { }
#line 2056 "y.tab.c"
    break;

  case 15: /* template_par_list: template_par  */
#line 235 "parser.y"
        {
	    (yyval.arg) = (yyvsp[0].arg);
	}
#line 2064 "y.tab.c"
    break;

  case 16: /* template_par_list: template_par ',' template_par_list  */
#line 239 "parser.y"
        {
	    (yyval.arg) = (yyvsp[-2].arg);
	    (yyval.arg)->next = (yyvsp[0].arg);
	}
#line 2073 "y.tab.c"
    break;

  case 17: /* template_par: CLASS id  */
#line 247 "parser.y"
        {
	    CType* t = new ObjectCType((yyvsp[-1].str)->comment, (yyvsp[0].str)->val, NULL);
	    (yyval.arg) = new Arg((yyvsp[-1].str)->comment, (yyvsp[0].str)->val, t);
	}
#line 2082 "y.tab.c"
    break;

  case 18: /* template_par: id  */
#line 252 "parser.y"
        {
	    CType* t = new ObjectCType((yyvsp[0].str)->comment, (yyvsp[0].str)->val, NULL);
	    (yyval.arg) = new Arg((yyvsp[0].str)->comment, (yyvsp[0].str)->val, t);
	}
#line 2091 "y.tab.c"
    break;

  case 19: /* $@1: %empty  */
#line 259 "parser.y"
                                    {
	    push_scope(PRIVATE); local.typedefs.clear();
	    }
#line 2099 "y.tab.c"
    break;

  case 20: /* class_decl: CLASS scoped_id parent_spec $@1 '{' class_def_body '}'  */
#line 262 "parser.y"
        {
	    ObjectCType* t = new ObjectCType(NULL, (yyvsp[-5].str)->val,
		global.pool.add(local.source_file));
	    (yyval.clazz) = new Class((yyvsp[-6].str)->comment, t, (yyvsp[-4].objtype), (yyvsp[-1].method));
	    (yyval.clazz)->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  class %s\n", (yyval.clazz)->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
#line 2116 "y.tab.c"
    break;

  case 21: /* $@2: %empty  */
#line 274 "parser.y"
                                       {
	    push_scope(PRIVATE); local.typedefs.clear();
	    }
#line 2124 "y.tab.c"
    break;

  case 22: /* class_decl: CLASS id scoped_id parent_spec $@2 '{' class_def_body '}'  */
#line 277 "parser.y"
        {
	    /* assume first id is an export spec */
	    ObjectCType* t = new ObjectCType(NULL, (yyvsp[-5].str)->val,
		global.pool.add(local.source_file));
	    (yyval.clazz) = new Class((yyvsp[-7].str)->comment, t, (yyvsp[-4].objtype), (yyvsp[-1].method));
	    (yyval.clazz)->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  class %s %s\n", (yyvsp[-6].str)->val, (yyval.clazz)->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
#line 2142 "y.tab.c"
    break;

  case 23: /* $@3: %empty  */
#line 290 "parser.y"
                      {
	    push_scope(PUBLIC); local.typedefs.clear();
	    }
#line 2150 "y.tab.c"
    break;

  case 24: /* class_decl: STRUCT opt_id $@3 '{' class_def_body '}'  */
#line 293 "parser.y"
        {
	    ObjectCType* t = new ObjectCType(NULL, (yyvsp[-4].str)->val,
		global.pool.add(local.source_file));
	    (yyval.clazz) = new Class((yyvsp[-5].str)->comment, t, NULL, (yyvsp[-1].method));
	    (yyval.clazz)->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  struct %s\n", (yyval.clazz)->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
#line 2167 "y.tab.c"
    break;

  case 25: /* $@4: %empty  */
#line 305 "parser.y"
                     {
	    push_scope(PUBLIC); local.typedefs.clear();
	    }
#line 2175 "y.tab.c"
    break;

  case 26: /* class_decl: UNION opt_id $@4 '{' class_def_body '}'  */
#line 308 "parser.y"
        {
	    ObjectCType* t = new ObjectCType(NULL, (yyvsp[-4].str)->val,
		global.pool.add(local.source_file));
	    (yyval.clazz) = new Class((yyvsp[-5].str)->comment, t, NULL, (yyvsp[-1].method));
	    (yyval.clazz)->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  union %s\n", (yyval.clazz)->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
#line 2192 "y.tab.c"
    break;

  case 27: /* parent_spec: %empty  */
#line 323 "parser.y"
                  { (yyval.objtype) = NULL; }
#line 2198 "y.tab.c"
    break;

  case 28: /* parent_spec: ':' parent_list  */
#line 324 "parser.y"
                        { (yyval.objtype) = (yyvsp[0].objtype); }
#line 2204 "y.tab.c"
    break;

  case 29: /* parent_list: parent_decl  */
#line 328 "parser.y"
                    { (yyval.objtype) = (yyvsp[0].objtype); }
#line 2210 "y.tab.c"
    break;

  case 30: /* parent_list: parent_list ',' parent_decl  */
#line 330 "parser.y"
        {
	    warning(msg("multiple class inheritance ignored"));
	    (yyval.objtype) = (yyvsp[-2].objtype);
	}
#line 2219 "y.tab.c"
    break;

  case 31: /* parent_decl: PUBLIC scoped_id  */
#line 338 "parser.y"
        {
	    (yyval.objtype) = new ObjectCType(NULL, (yyvsp[0].str)->val, NULL);
	}
#line 2227 "y.tab.c"
    break;

  case 32: /* parent_decl: PUBLIC scoped_id '<' type_list '>'  */
#line 342 "parser.y"
        {
	    (yyval.objtype) = new ObjectCType(NULL, (yyvsp[-3].str)->val, NULL);
	    (yyval.objtype)->args = (yyvsp[-1].arg);
	}
#line 2236 "y.tab.c"
    break;

  case 33: /* parent_decl: PRIVATE scoped_id  */
#line 348 "parser.y"
                          { (yyval.objtype) = NULL; }
#line 2242 "y.tab.c"
    break;

  case 34: /* parent_decl: PROTECTED scoped_id  */
#line 349 "parser.y"
                            { (yyval.objtype) = NULL; }
#line 2248 "y.tab.c"
    break;

  case 35: /* class_def_body: scoped_class_def_item  */
#line 353 "parser.y"
                              { (yyval.method) = (yyvsp[0].method); }
#line 2254 "y.tab.c"
    break;

  case 36: /* class_def_body: class_def_body scoped_class_def_item  */
#line 355 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    if ((yyvsp[-1].method))
	    {
	        Method* m = (yyvsp[-1].method);
		while (m->next) m = m->next;
		m->next = (yyvsp[0].method);
		(yyval.method) = (yyvsp[-1].method);
	    }
	}
#line 2269 "y.tab.c"
    break;

  case 37: /* scoped_class_def_item: class_def_item  */
#line 369 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    while ((yyval.method))
	    {
		if (local.public_scope == PROTECTED)
		{
		    (yyval.method)->type->modifiers |= CType::M_PROTECTED;
		}
		if (local.public_scope == PRIVATE)
		{
		    (yyval.method)->type->modifiers |= CType::M_PRIVATE;
		}
		(yyval.method) = (yyval.method)->next;
	    }
	    (yyval.method) = (yyvsp[0].method);
	}
#line 2290 "y.tab.c"
    break;

  case 38: /* class_def_item: scope_type ':'  */
#line 388 "parser.y"
                       { (yyval.method) = NULL; }
#line 2296 "y.tab.c"
    break;

  case 39: /* class_def_item: var_decl opt_bitfield ';'  */
#line 390 "parser.y"
        {
	    (yyval.method) = (yyvsp[-2].method);
	    while ((yyval.method))
	    {
		(yyval.method)->type->modifiers |= CType::M_ATTRIBUTE;
		(yyval.method) = (yyval.method)->next;
	    }
	    (yyval.method) = (yyvsp[-2].method);
	}
#line 2310 "y.tab.c"
    break;

  case 40: /* class_def_item: method_decl  */
#line 400 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	}
#line 2318 "y.tab.c"
    break;

  case 41: /* class_def_item: type_decl  */
#line 407 "parser.y"
        {
	    if ((yyvsp[0].type) && ((yyvsp[0].type)->modifiers & CType::M_ENUM))
	    {
		(yyval.method) = new Method((yyvsp[0].type)->comment, (yyvsp[0].type)->CTypeName(), NULL);
		(yyval.method)->type = (yyvsp[0].type);
		(yyval.method)->notes |= Method::ENUM_DEFINE;
	    }
	    else if ((yyvsp[0].type) && ((yyvsp[0].type)->modifiers & CType::M_TYPEDEF))
	    {
                CType *ct = local.typedefs.get((yyvsp[0].type)->alias);
                if (ct)
		{
		    char buf[512];
		    sprintf(buf, "overriding old typedef %s (%s) with %s (%s)\n",
			ct->alias, ct->CTypeName(), (yyvsp[0].type)->alias, (yyvsp[0].type)->CTypeName());
		    warning(msg(buf));
                }
                local.typedefs.put((yyvsp[0].type)->alias, (yyvsp[0].type));
		(yyval.method) = new Method((yyvsp[0].type)->comment, (yyvsp[0].type)->alias, NULL);
		(yyval.method)->type = (yyvsp[0].type);
	    }
	    else
	    {
		(yyval.method) = NULL;
	    }
	}
#line 2349 "y.tab.c"
    break;

  case 42: /* class_def_item: friend_decl  */
#line 433 "parser.y"
                    { (yyval.method) = (yyvsp[0].method); }
#line 2355 "y.tab.c"
    break;

  case 43: /* class_def_item: class_decl opt_id ';'  */
#line 434 "parser.y"
                              { (yyval.method) = NULL; }
#line 2361 "y.tab.c"
    break;

  case 44: /* class_def_item: TYPEDEF class_decl opt_id ';'  */
#line 435 "parser.y"
                                      { (yyval.method) = NULL; }
#line 2367 "y.tab.c"
    break;

  case 45: /* class_def_item: error  */
#line 436 "parser.y"
              { (yyval.method) = NULL; }
#line 2373 "y.tab.c"
    break;

  case 46: /* opt_bitfield: %empty  */
#line 440 "parser.y"
                  { (yyval.immediate) = 0; }
#line 2379 "y.tab.c"
    break;

  case 47: /* opt_bitfield: ':' NUM  */
#line 441 "parser.y"
                { (yyval.immediate) = (int) (yyvsp[0].number)->val; }
#line 2385 "y.tab.c"
    break;

  case 48: /* opt_id: %empty  */
#line 445 "parser.y"
                  { (yyval.str) = new String(NULL, ""); }
#line 2391 "y.tab.c"
    break;

  case 49: /* opt_id: id  */
#line 446 "parser.y"
           { (yyval.str) = (yyvsp[0].str); }
#line 2397 "y.tab.c"
    break;

  case 50: /* var_decl: type id_list  */
#line 451 "parser.y"
        {
	    char* sep = "|";
	    const char* p = strtok((char*) (yyvsp[0].str)->val, sep);
	    Method* last = NULL;
	    (yyval.method) = NULL;
	    while (p)
	    {
		Method* m = new Method((yyvsp[-1].type)->comment, p, NULL);
		m->type = (yyvsp[-1].type);
		if (!((yyval.method))) (yyval.method) = m;
		if (last) last->next = m;
		last = m;
		p = strtok(NULL, sep);
	    }
	}
#line 2417 "y.tab.c"
    break;

  case 51: /* var_decl: type id array_type_indirections  */
#line 467 "parser.y"
        {
	    (yyval.method) = new Method((yyvsp[-2].type)->comment, (yyvsp[-1].str)->val, NULL);
	    if ((yyvsp[0].immediates)[0] >= 0)
	    {
                (yyval.method)->type = (yyvsp[-2].type);
                while (--(yyvsp[0].immediates)[0]) (yyval.method)->type = new ArrayCType((yyval.method)->type, (yyvsp[0].immediates)[(yyvsp[0].immediates)[0]+1]);
                
                if ((yyvsp[0].immediates)[1] >= 0)
                {
                    (yyval.method)->type = new ArrayCType((yyval.method)->type, (yyvsp[0].immediates)[(yyvsp[0].immediates)[0]+1]);
                }
                else
                {
                    (yyval.method)->type->indirection_count++;
                    (yyval.method)->type->modifiers |= CType::M_POINTER;
                }
	    }
	    else
	    {
		(yyval.method)->type = (yyvsp[-2].type);
		(yyval.method)->type->indirection_count++;
		(yyval.method)->type->modifiers |= CType::M_POINTER;
	    }
	}
#line 2446 "y.tab.c"
    break;

  case 52: /* var_decl: type '(' '*' id ')' '(' culled_arg_list ')'  */
#line 492 "parser.y"
        {
            (yyval.method) = NULL;
        }
#line 2454 "y.tab.c"
    break;

  case 53: /* id_list: id  */
#line 499 "parser.y"
        {
	    (yyval.str) = (yyvsp[0].str);
	}
#line 2462 "y.tab.c"
    break;

  case 54: /* id_list: id ',' id_list  */
#line 503 "parser.y"
        {
	    char buf[TOKEN_BUFFER_SIZE];
	    sprintf(buf, "%s|%s", (yyvsp[-2].str)->val, (yyvsp[0].str)->val);
	    (yyval.str) = new String((yyvsp[-2].str)->comment, buf);
	}
#line 2472 "y.tab.c"
    break;

  case 55: /* friend_decl: FRIEND CLASS scoped_id ';'  */
#line 511 "parser.y"
                                   { (yyval.method) = NULL; }
#line 2478 "y.tab.c"
    break;

  case 56: /* friend_decl: FRIEND STRUCT scoped_id ';'  */
#line 512 "parser.y"
                                    { (yyval.method) = NULL; }
#line 2484 "y.tab.c"
    break;

  case 57: /* friend_decl: FRIEND UNION scoped_id ';'  */
#line 513 "parser.y"
                                   { (yyval.method) = NULL; }
#line 2490 "y.tab.c"
    break;

  case 58: /* friend_decl: FRIEND method_decl  */
#line 515 "parser.y"
        {
            if ((yyvsp[0].method)->is_operator)
            {
                (yyval.method) = (yyvsp[0].method);
                (yyval.method)->type->modifiers |= CType::M_STATIC;
            }
            else
            {
                (yyval.method) = NULL;
            }
        }
#line 2506 "y.tab.c"
    break;

  case 59: /* type_decl: TYPEDEF type id ';'  */
#line 530 "parser.y"
        {
	    (yyval.type) = (yyvsp[-2].type);
	    (yyval.type)->comment = (yyvsp[-3].str)->comment;
	    (yyval.type)->alias = (yyvsp[-1].str)->val;
	    (yyval.type)->modifiers |= CType::M_TYPEDEF;
	}
#line 2517 "y.tab.c"
    break;

  case 60: /* type_decl: TYPEDEF type id array_type_indirections ';'  */
#line 537 "parser.y"
        {
	    if ((yyvsp[-1].immediates) != NULL)
	    {
                (yyval.type) = (yyvsp[-3].type);
                while (--(yyvsp[-1].immediates)[0]) (yyval.type) = new ArrayCType((yyval.type), (yyvsp[-1].immediates)[(yyvsp[-1].immediates)[0]+1]);
                
                if ((yyvsp[-1].immediates)[1] >= 0)
                {
                    (yyval.type) = new ArrayCType((yyval.type), (yyvsp[-1].immediates)[(yyvsp[-1].immediates)[0]+1]);
                }
                else
                {
                    (yyval.type)->indirection_count++;
                    (yyval.type)->modifiers |= CType::M_POINTER;
                }
	    }
	    else
	    {
		(yyvsp[-3].type)->indirection_count++;
		(yyvsp[-3].type)->modifiers |= CType::M_POINTER;
                (yyval.type) = (yyvsp[-3].type);
	    }
            (yyval.type)->alias = (yyvsp[-2].str)->val;
            (yyval.type)->modifiers |= CType::M_TYPEDEF;
	}
#line 2547 "y.tab.c"
    break;

  case 61: /* type_decl: TYPEDEF func_arg ';'  */
#line 563 "parser.y"
        {
            (yyval.type) = (yyvsp[-1].arg)->type;
	    (yyval.type)->comment = (yyvsp[-2].str)->comment;
	    (yyval.type)->alias = (yyvsp[-1].arg)->name;
	    (yyval.type)->modifiers |= CType::M_TYPEDEF;
	}
#line 2558 "y.tab.c"
    break;

  case 62: /* type_decl: enum_decl ';'  */
#line 569 "parser.y"
                      { (yyval.type) = (yyvsp[-1].type); }
#line 2564 "y.tab.c"
    break;

  case 63: /* enum_decl: ENUM scoped_id '{' enum_item_list '}'  */
#line 574 "parser.y"
        {
	    (yyval.type) = new EnumCType((yyvsp[-4].str)->comment, (yyvsp[-3].str)->val, (yyvsp[-1].arg));
	}
#line 2572 "y.tab.c"
    break;

  case 64: /* enum_decl: ENUM '{' enum_item_list '}'  */
#line 578 "parser.y"
        {
	    char buf[32];
	    sprintf(buf, "anon%d", local.serial++);
	    (yyval.type) = new EnumCType((yyvsp[-3].str)->comment, global.pool.add(buf), (yyvsp[-1].arg));
	}
#line 2582 "y.tab.c"
    break;

  case 65: /* enum_item_list: enum_item  */
#line 586 "parser.y"
                  { (yyval.arg) = (yyvsp[0].arg); }
#line 2588 "y.tab.c"
    break;

  case 66: /* enum_item_list: enum_item_list ',' enum_item  */
#line 588 "parser.y"
        {
	    (yyval.arg) = (yyvsp[0].arg);
	    if ((yyvsp[-2].arg))
	    {
	        Arg* a = (yyvsp[-2].arg);
		while (a->next) a = a->next;
		a->next = (yyvsp[0].arg);
		(yyval.arg) = (yyvsp[-2].arg);
	    }
	}
#line 2603 "y.tab.c"
    break;

  case 67: /* enum_item: id opt_var_assign_text  */
#line 602 "parser.y"
        {
	    (yyval.arg) = new Arg((yyvsp[-1].str)->comment, (yyvsp[-1].str)->val, NULL);
	    if ((yyvsp[0].str))
	    {
		(yyval.arg)->default_value = (yyvsp[0].str)->val;
	    }
	}
#line 2615 "y.tab.c"
    break;

  case 68: /* opt_var_assign_text: %empty  */
#line 613 "parser.y"
        {
	    (yyval.str) = NULL;
	}
#line 2623 "y.tab.c"
    break;

  case 69: /* $@5: %empty  */
#line 616 "parser.y"
            { local.copy_enum = 1; }
#line 2629 "y.tab.c"
    break;

  case 70: /* opt_var_assign_text: '=' $@5 CTEXT  */
#line 617 "parser.y"
        {
            (yyval.str) = (yyvsp[0].str);
        }
#line 2637 "y.tab.c"
    break;

  case 71: /* method_decl: TILDE func  */
#line 624 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    if ((yyval.method))
	    {
		(yyval.method)->type = new CreateCType((yyvsp[-1].str)->comment, CType::M_DTOR);
		(yyval.method)->comment = (yyvsp[-1].str)->comment;
	    }
	}
#line 2650 "y.tab.c"
    break;

  case 72: /* method_decl: VIRTUAL TILDE func  */
#line 633 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    if ((yyval.method))
	    {
		(yyval.method)->type = new CreateCType((yyvsp[-2].str)->comment, CType::M_DTOR, CType::M_VIRTUAL);
		(yyval.method)->comment = (yyvsp[-2].str)->comment;
	    }
	}
#line 2663 "y.tab.c"
    break;

  case 73: /* method_decl: func  */
#line 642 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    if ((yyval.method) && !(yyval.method)->is_operator)
	    {
		(yyval.method)->type = new CreateCType((yyvsp[0].method)->comment);
		(yyval.method)->comment = (yyvsp[0].method)->comment;
	    }
	}
#line 2676 "y.tab.c"
    break;

  case 74: /* method_decl: type func  */
#line 651 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    if ((yyval.method))
	    {
		(yyval.method)->type = (yyvsp[-1].type);
		(yyval.method)->comment = (yyvsp[-1].type)->comment;
	    }
	}
#line 2689 "y.tab.c"
    break;

  case 75: /* method_decl: VIRTUAL type func  */
#line 660 "parser.y"
        {
	    (yyval.method) = (yyvsp[0].method);
	    if ((yyval.method))
	    {
		(yyval.method)->type = (yyvsp[-1].type);
		(yyval.method)->type->modifiers |= CType::M_VIRTUAL;
		(yyval.method)->comment = (yyvsp[-2].str)->comment;
	    }
	}
#line 2703 "y.tab.c"
    break;

  case 76: /* func: scoped_id '(' culled_arg_list ')' func_mods func_end  */
#line 673 "parser.y"
        {
	    (yyval.method) = new Method((yyvsp[-5].str)->comment, (yyvsp[-5].str)->val, (yyvsp[-3].arg));
	    if ((yyvsp[-1].immediate)) (yyval.method)->is_const = 1;
	    if ((yyvsp[0].immediate)) (yyval.method)->is_pure = 1;
	}
#line 2713 "y.tab.c"
    break;

  case 77: /* func: opt_scoped_id OPERATOR cpp_operator '(' culled_arg_list ')' func_mods func_end  */
#line 679 "parser.y"
        {
            char buf[TOKEN_BUFFER_SIZE];
            sprintf(buf, "op%s", (yyvsp[-5].str)->val);
	    (yyval.method) = new Method("", global.pool.add(buf), (yyvsp[-3].arg));
            (yyval.method)->is_operator = 1;
	}
#line 2724 "y.tab.c"
    break;

  case 78: /* func: opt_scoped_id OPERATOR type '(' culled_arg_list ')' func_mods func_end  */
#line 686 "parser.y"
        {
	    (yyval.method) = new Method("", "opCast", (yyvsp[-3].arg));
            (yyval.method)->type = (yyvsp[-5].type);
            (yyval.method)->comment = (yyvsp[-5].type)->comment;
            (yyval.method)->is_operator = 1;
	}
#line 2735 "y.tab.c"
    break;

  case 79: /* culled_arg_list: arg_list  */
#line 696 "parser.y"
        {
	    /* remove an arg list which contains a single "void".
	     */
	    (yyval.arg) = (yyvsp[0].arg);
	    if ((yyval.arg) && (yyval.arg)->next == NULL &&
		0 == strcmp((yyval.arg)->type->CTypeName(), "void"))
	    {
		(yyval.arg) = NULL;
	    }
	}
#line 2750 "y.tab.c"
    break;

  case 80: /* cpp_operator: unary_operator  */
#line 710 "parser.y"
        {
	    char buf[2];
	    sprintf(buf, "%c", (yyvsp[0].immediate));
	    (yyval.str) = new String(buf);
        }
#line 2760 "y.tab.c"
    break;

  case 81: /* cpp_operator: binary_operator  */
#line 716 "parser.y"
        {
	    char buf[2];
	    sprintf(buf, "%c", (yyvsp[0].immediate));
	    (yyval.str) = new String(buf);
        }
#line 2770 "y.tab.c"
    break;

  case 82: /* cpp_operator: assignment_operator  */
#line 721 "parser.y"
                            { (yyval.str) = (yyvsp[0].str); }
#line 2776 "y.tab.c"
    break;

  case 83: /* cpp_operator: PLUSPLUS  */
#line 722 "parser.y"
                 { (yyval.str) = new String("++"); }
#line 2782 "y.tab.c"
    break;

  case 84: /* cpp_operator: MINUSMINUS  */
#line 722 "parser.y"
                                                         { (yyval.str) = new String("--"); }
#line 2788 "y.tab.c"
    break;

  case 85: /* cpp_operator: LEFTLEFT  */
#line 723 "parser.y"
                 { (yyval.str) = new String("<<"); }
#line 2794 "y.tab.c"
    break;

  case 86: /* cpp_operator: RIGHTRIGHT  */
#line 723 "parser.y"
                                                         { (yyval.str) = new String(">>"); }
#line 2800 "y.tab.c"
    break;

  case 87: /* cpp_operator: ANDAND  */
#line 724 "parser.y"
               { (yyval.str) = new String("&&"); }
#line 2806 "y.tab.c"
    break;

  case 88: /* cpp_operator: OROR  */
#line 724 "parser.y"
                                                 { (yyval.str) = new String("||"); }
#line 2812 "y.tab.c"
    break;

  case 89: /* cpp_operator: EQUALSEQUALS  */
#line 725 "parser.y"
                     { (yyval.str) = new String("=="); }
#line 2818 "y.tab.c"
    break;

  case 90: /* cpp_operator: NOTEQUALS  */
#line 725 "parser.y"
                                                            { (yyval.str) = new String("!="); }
#line 2824 "y.tab.c"
    break;

  case 91: /* cpp_operator: LEFTEQUALS  */
#line 726 "parser.y"
                   { (yyval.str) = new String("<="); }
#line 2830 "y.tab.c"
    break;

  case 92: /* cpp_operator: RIGHTEQUALS  */
#line 726 "parser.y"
                                                            { (yyval.str) = new String(">="); }
#line 2836 "y.tab.c"
    break;

  case 93: /* cpp_operator: ARROW  */
#line 727 "parser.y"
              { (yyval.str) = new String("->"); }
#line 2842 "y.tab.c"
    break;

  case 94: /* cpp_operator: NEW  */
#line 728 "parser.y"
            { (yyval.str) = new String("new"); }
#line 2848 "y.tab.c"
    break;

  case 95: /* cpp_operator: DELETE  */
#line 728 "parser.y"
                                                 { (yyval.str) = new String("delete"); }
#line 2854 "y.tab.c"
    break;

  case 96: /* cpp_operator: array_type_indirection  */
#line 729 "parser.y"
                               { (yyval.str) = new String("[]"); }
#line 2860 "y.tab.c"
    break;

  case 97: /* unary_operator: '&'  */
#line 733 "parser.y"
            { (yyval.immediate) = '&'; }
#line 2866 "y.tab.c"
    break;

  case 98: /* unary_operator: '*'  */
#line 734 "parser.y"
            { (yyval.immediate) = '*'; }
#line 2872 "y.tab.c"
    break;

  case 99: /* unary_operator: '+'  */
#line 735 "parser.y"
            { (yyval.immediate) = '+'; }
#line 2878 "y.tab.c"
    break;

  case 100: /* unary_operator: '-'  */
#line 736 "parser.y"
            { (yyval.immediate) = '-'; }
#line 2884 "y.tab.c"
    break;

  case 101: /* unary_operator: TILDE  */
#line 737 "parser.y"
              { (yyval.immediate) = '~'; }
#line 2890 "y.tab.c"
    break;

  case 102: /* unary_operator: '!'  */
#line 738 "parser.y"
            { (yyval.immediate) = '!'; }
#line 2896 "y.tab.c"
    break;

  case 103: /* binary_operator: '/'  */
#line 742 "parser.y"
            { (yyval.immediate) = '/'; }
#line 2902 "y.tab.c"
    break;

  case 104: /* binary_operator: '%'  */
#line 743 "parser.y"
            { (yyval.immediate) = '%'; }
#line 2908 "y.tab.c"
    break;

  case 105: /* binary_operator: '|'  */
#line 744 "parser.y"
            { (yyval.immediate) = '|'; }
#line 2914 "y.tab.c"
    break;

  case 106: /* binary_operator: '<'  */
#line 745 "parser.y"
            { (yyval.immediate) = '<'; }
#line 2920 "y.tab.c"
    break;

  case 107: /* binary_operator: '>'  */
#line 746 "parser.y"
            { (yyval.immediate) = '>'; }
#line 2926 "y.tab.c"
    break;

  case 108: /* binary_operator: '^'  */
#line 747 "parser.y"
            { (yyval.immediate) = '^'; }
#line 2932 "y.tab.c"
    break;

  case 109: /* assignment_operator: '='  */
#line 751 "parser.y"
            { (yyval.str) = new String("="); }
#line 2938 "y.tab.c"
    break;

  case 110: /* assignment_operator: STAREQUALS  */
#line 752 "parser.y"
                   { (yyval.str) = new String("*="); }
#line 2944 "y.tab.c"
    break;

  case 111: /* assignment_operator: SLASHEQUALS  */
#line 753 "parser.y"
                    { (yyval.str) = new String("/="); }
#line 2950 "y.tab.c"
    break;

  case 112: /* assignment_operator: '%' '='  */
#line 754 "parser.y"
                { (yyval.str) = new String("%="); }
#line 2956 "y.tab.c"
    break;

  case 113: /* assignment_operator: PLUSEQUALS  */
#line 755 "parser.y"
                   { (yyval.str) = new String("+="); }
#line 2962 "y.tab.c"
    break;

  case 114: /* assignment_operator: MINUSEQUALS  */
#line 756 "parser.y"
                    { (yyval.str) = new String("-="); }
#line 2968 "y.tab.c"
    break;

  case 115: /* assignment_operator: LEFTLEFT '='  */
#line 757 "parser.y"
                      { (yyval.str) = new String("<<="); }
#line 2974 "y.tab.c"
    break;

  case 116: /* assignment_operator: RIGHTRIGHT '='  */
#line 758 "parser.y"
                        { (yyval.str) = new String(">>="); }
#line 2980 "y.tab.c"
    break;

  case 117: /* assignment_operator: ANDEQUALS  */
#line 759 "parser.y"
                   { (yyval.str) = new String("&="); }
#line 2986 "y.tab.c"
    break;

  case 118: /* assignment_operator: '^' '='  */
#line 760 "parser.y"
                 { (yyval.str) = new String("^="); }
#line 2992 "y.tab.c"
    break;

  case 119: /* assignment_operator: OREQUALS  */
#line 761 "parser.y"
                  { (yyval.str) = new String("|="); }
#line 2998 "y.tab.c"
    break;

  case 120: /* id: IDENTIFIER  */
#line 764 "parser.y"
               { (yyval.str) = (yyvsp[0].str); }
#line 3004 "y.tab.c"
    break;

  case 121: /* opt_scoped_id: %empty  */
#line 767 "parser.y"
                  { (yyval.str) = NULL; }
#line 3010 "y.tab.c"
    break;

  case 122: /* opt_scoped_id: id COLONCOLON  */
#line 768 "parser.y"
                      { (yyval.str) = (yyvsp[-1].str); }
#line 3016 "y.tab.c"
    break;

  case 123: /* scoped_id: id  */
#line 772 "parser.y"
           { (yyval.str) = (yyvsp[0].str); }
#line 3022 "y.tab.c"
    break;

  case 124: /* scoped_id: id COLONCOLON scoped_id  */
#line 774 "parser.y"
        {
	    char* buf = new char[8 + strlen((yyvsp[-2].str)->val) + strlen((yyvsp[0].str)->val)];
	    sprintf(buf, "%s::%s", (yyvsp[-2].str)->val, (yyvsp[0].str)->val);
	    (yyval.str) = new String((yyvsp[-2].str)->comment, buf);
	    delete[] buf;
	}
#line 3033 "y.tab.c"
    break;

  case 125: /* func_end: ';'  */
#line 783 "parser.y"
            { (yyval.immediate) = 0; }
#line 3039 "y.tab.c"
    break;

  case 126: /* func_end: func_end ';'  */
#line 784 "parser.y"
                     { (yyval.immediate) = (yyvsp[-1].immediate); }
#line 3045 "y.tab.c"
    break;

  case 127: /* $@6: %empty  */
#line 785 "parser.y"
                          { local.eatc = 1; }
#line 3051 "y.tab.c"
    break;

  case 128: /* func_end: opt_init_list '{' $@6 CTEXT  */
#line 785 "parser.y"
                                                    { (yyval.immediate) = 0; }
#line 3057 "y.tab.c"
    break;

  case 129: /* func_end: '=' NUM ';'  */
#line 786 "parser.y"
                    { (yyval.immediate) = 1; }
#line 3063 "y.tab.c"
    break;

  case 130: /* func_mods: %empty  */
#line 789 "parser.y"
                     { (yyval.immediate) = 0; }
#line 3069 "y.tab.c"
    break;

  case 131: /* func_mods: CONST  */
#line 789 "parser.y"
                                         { (yyval.immediate) = 1; }
#line 3075 "y.tab.c"
    break;

  case 136: /* init_item: type '(' opt_constant_expr_list ')'  */
#line 795 "parser.y"
                                               { }
#line 3081 "y.tab.c"
    break;

  case 137: /* arg_list: %empty  */
#line 798 "parser.y"
                  { (yyval.arg) = NULL; }
#line 3087 "y.tab.c"
    break;

  case 138: /* arg_list: arg  */
#line 799 "parser.y"
            { (yyval.arg) = (yyvsp[0].arg); }
#line 3093 "y.tab.c"
    break;

  case 139: /* arg_list: arg ',' arg_list  */
#line 801 "parser.y"
        {
	    (yyval.arg) = (yyvsp[0].arg);
	    if ((yyvsp[-2].arg))
	    {
		(yyvsp[-2].arg)->next = (yyval.arg);
		(yyval.arg) = (yyvsp[-2].arg);
	    }
	}
#line 3106 "y.tab.c"
    break;

  case 140: /* arg: type opt_var_assign  */
#line 813 "parser.y"
        {
	    char buf[32];
	    sprintf(buf, "temp%d", local.serial++);
	    (yyval.arg) = new Arg((yyvsp[-1].type)->comment, global.pool.add(buf), (yyvsp[-1].type));
	    if ((yyvsp[0].str))
	    {
		(yyval.arg)->default_value = (yyvsp[0].str)->val;
	    }
	}
#line 3120 "y.tab.c"
    break;

  case 141: /* arg: type id opt_var_assign  */
#line 823 "parser.y"
        {
	    (yyval.arg) = new Arg((yyvsp[-2].type)->comment, (yyvsp[-1].str)->val, (yyvsp[-2].type));
	    if ((yyvsp[0].str))
	    {
		(yyval.arg)->default_value = (yyvsp[0].str)->val;
	    }
	}
#line 3132 "y.tab.c"
    break;

  case 142: /* arg: type opt_id array_type_indirections  */
#line 831 "parser.y"
        {
	    if ((yyvsp[0].immediates) != NULL)
	    {
                while (--(yyvsp[0].immediates)[0]) (yyvsp[-2].type) = new ArrayCType((yyvsp[-2].type), (yyvsp[0].immediates)[(yyvsp[0].immediates)[0]+1]);
                
                if ((yyvsp[0].immediates)[1] >= 0)
                {
                    (yyvsp[-2].type) = new ArrayCType((yyvsp[-2].type), (yyvsp[0].immediates)[(yyvsp[0].immediates)[0]+1]);
                }
                else
                {
                    (yyvsp[-2].type)->indirection_count++;
                    (yyvsp[-2].type)->modifiers |= CType::M_POINTER;
                }
	    }
	    else
	    {
		(yyvsp[-2].type)->indirection_count++;
		(yyvsp[-2].type)->modifiers |= CType::M_POINTER;
	    }
            if (strlen((yyvsp[-1].str)->val) == 0)
            {
                char buf[32];
                sprintf(buf, "temp%d", local.serial++);
                (yyvsp[-1].str)->val = global.pool.add(buf);
            }
            (yyval.arg) = new Arg((yyvsp[-2].type)->comment, (yyvsp[-1].str)->val, (yyvsp[-2].type));
	}
#line 3165 "y.tab.c"
    break;

  case 143: /* arg: func_arg  */
#line 860 "parser.y"
        {
	    (yyval.arg) = (yyvsp[0].arg);
	}
#line 3173 "y.tab.c"
    break;

  case 144: /* arg: ELLIPSIS  */
#line 864 "parser.y"
        {
	    CType* ct = new CType("", CType::M_VARARG);
	    (yyval.arg) = new Arg("", "...", ct);
	}
#line 3182 "y.tab.c"
    break;

  case 145: /* func_arg: type id '(' culled_arg_list ')'  */
#line 872 "parser.y"
        {
	    FunctionCType* ct = new FunctionCType((yyvsp[-4].type), (yyvsp[-1].arg));
            ct->indirection_count -= (yyvsp[-4].type)->indirection_count;
	    (yyval.arg) = new Arg((yyvsp[-4].type)->comment, (yyvsp[-3].str)->val, ct);
	}
#line 3192 "y.tab.c"
    break;

  case 146: /* func_arg: type '(' id ')' '(' culled_arg_list ')'  */
#line 878 "parser.y"
        {
	    FunctionCType* ct = new FunctionCType((yyvsp[-6].type), (yyvsp[-1].arg));
            ct->indirection_count -= (yyvsp[-6].type)->indirection_count;
	    (yyval.arg) = new Arg((yyvsp[-6].type)->comment, (yyvsp[-4].str)->val, ct);
	}
#line 3202 "y.tab.c"
    break;

  case 147: /* func_arg: type '(' '*' id ')' '(' culled_arg_list ')'  */
#line 884 "parser.y"
        {
	    FunctionCType* ct = new FunctionCType((yyvsp[-7].type), (yyvsp[-1].arg));
            ct->indirection_count -= (yyvsp[-7].type)->indirection_count - 1;
	    (yyval.arg) = new Arg((yyvsp[-7].type)->comment, (yyvsp[-4].str)->val, ct);
	}
#line 3212 "y.tab.c"
    break;

  case 148: /* opt_var_assign: %empty  */
#line 893 "parser.y"
        {
	    (yyval.str) = NULL;
	}
#line 3220 "y.tab.c"
    break;

  case 149: /* opt_var_assign: '=' constant_expr  */
#line 897 "parser.y"
        {
	    char buf[128];
	    sprintf(buf, "%d", (yyvsp[0].expr));
	    (yyval.str) = new String(NULL, buf);
	}
#line 3230 "y.tab.c"
    break;

  case 150: /* type_list: type  */
#line 906 "parser.y"
        {
	    (yyval.arg) = new Arg((yyvsp[0].type)->comment, "", (yyvsp[0].type));
	}
#line 3238 "y.tab.c"
    break;

  case 151: /* type_list: type ',' type_list  */
#line 910 "parser.y"
        {
	    (yyval.arg) = new Arg((yyvsp[-2].type)->comment, "", (yyvsp[-2].type));
	    (yyval.arg)->next = (yyvsp[0].arg);
	}
#line 3247 "y.tab.c"
    break;

  case 152: /* type: type_red1  */
#line 917 "parser.y"
                  { (yyval.type) = (yyvsp[0].type); }
#line 3253 "y.tab.c"
    break;

  case 153: /* type: type_mods type_red1  */
#line 919 "parser.y"
        {
	    (yyval.type) = (yyvsp[0].type);
	    (yyval.type)->comment = (yyvsp[-1].type)->comment;
	    (yyval.type)->modifiers |= (yyvsp[-1].type)->modifiers;
	}
#line 3263 "y.tab.c"
    break;

  case 154: /* type: type_red1 type_mods  */
#line 925 "parser.y"
        {
	    (yyval.type) = (yyvsp[-1].type);
	    if ( ((yyval.type)->modifiers && CType::M_POINTER) && ((yyvsp[0].type)->modifiers & CType::M_CONST) )
		(yyval.type)->modifiers |= CType::M_PTR_CONST;
	    else
		(yyval.type)->modifiers |= (yyvsp[0].type)->modifiers;
	}
#line 3275 "y.tab.c"
    break;

  case 155: /* type: type_mods type_red1 type_mods  */
#line 933 "parser.y"
        {
	    (yyval.type) = (yyvsp[-1].type);
	    (yyval.type)->comment = (yyvsp[-2].type)->comment;
	    if ( ((yyval.type)->modifiers && CType::M_POINTER) && ((yyvsp[0].type)->modifiers & CType::M_CONST) )
		(yyval.type)->modifiers |= CType::M_PTR_CONST;
	    else
		(yyval.type)->modifiers |= (yyvsp[0].type)->modifiers;
	}
#line 3288 "y.tab.c"
    break;

  case 156: /* type_mods: type_mod  */
#line 944 "parser.y"
                 { (yyval.type) = (yyvsp[0].type); }
#line 3294 "y.tab.c"
    break;

  case 157: /* type_mods: type_mods type_mod  */
#line 946 "parser.y"
        {
	    (yyval.type) = (yyvsp[0].type);
	    (yyval.type)->comment = (yyvsp[-1].type)->comment;
	    (yyval.type)->modifiers |= (yyvsp[-1].type)->modifiers;
	}
#line 3304 "y.tab.c"
    break;

  case 158: /* type_mod: CONST  */
#line 954 "parser.y"
              { (yyval.type) = new CType((yyvsp[0].str)->comment, CType::M_CONST); }
#line 3310 "y.tab.c"
    break;

  case 159: /* type_mod: STATIC  */
#line 955 "parser.y"
               { (yyval.type) = new CType((yyvsp[0].str)->comment, CType::M_STATIC); }
#line 3316 "y.tab.c"
    break;

  case 160: /* type_mod: VOLATILE  */
#line 956 "parser.y"
                 { (yyval.type) = new CType((yyvsp[0].str)->comment, CType::M_VOLATILE); }
#line 3322 "y.tab.c"
    break;

  case 161: /* type_mod: REGISTER  */
#line 957 "parser.y"
                 { (yyval.type) = new CType((yyvsp[0].str)->comment, CType::M_REGISTER); }
#line 3328 "y.tab.c"
    break;

  case 162: /* type_mod: INLINE  */
#line 958 "parser.y"
               { (yyval.type) = new CType((yyvsp[0].str)->comment); }
#line 3334 "y.tab.c"
    break;

  case 163: /* type_red1: type_primitive  */
#line 962 "parser.y"
                       { (yyval.type) = (yyvsp[0].type); }
#line 3340 "y.tab.c"
    break;

  case 164: /* type_red1: CLASS type_red1  */
#line 963 "parser.y"
                        { (yyval.type) = (yyvsp[0].type); }
#line 3346 "y.tab.c"
    break;

  case 165: /* type_red1: type_red1 type_indirection  */
#line 965 "parser.y"
        {
	    (yyval.type) = (yyvsp[-1].type);
	    (yyval.type)->modifiers |= (yyvsp[0].immediate);
	    (yyval.type)->indirection_count++;
	}
#line 3356 "y.tab.c"
    break;

  case 166: /* array_type_indirections: array_type_indirection  */
#line 974 "parser.y"
        {
            /* Array_type_indirections is a pointer to an array of
             * ints specifying the size of each dimension of the
             * array; the number of dimensions is stored in the first
             * element.
             */
            (yyval.immediates) = (int*)malloc(sizeof(int)*2);
            (yyval.immediates)[0] = 1;
            (yyval.immediates)[1] = (yyvsp[0].immediate);
        }
#line 3371 "y.tab.c"
    break;

  case 167: /* array_type_indirections: array_type_indirections array_type_indirection  */
#line 985 "parser.y"
        {
            (yyval.immediates) = (int*)realloc((yyvsp[-1].immediates), sizeof(int)*(1+(++(yyvsp[-1].immediates)[0])));
            (yyval.immediates)[(yyval.immediates)[0]] = (yyvsp[0].immediate);
        }
#line 3380 "y.tab.c"
    break;

  case 168: /* array_type_indirection: '[' opt_constant_expr ']'  */
#line 992 "parser.y"
                                  { (yyval.immediate) = (yyvsp[-1].expr); }
#line 3386 "y.tab.c"
    break;

  case 169: /* type_primitive: integral_type_primitive  */
#line 996 "parser.y"
                                { (yyval.type) = (yyvsp[0].type); }
#line 3392 "y.tab.c"
    break;

  case 170: /* type_primitive: UNSIGNED integral_type_primitive  */
#line 998 "parser.y"
        {
	    (yyval.type) = (yyvsp[0].type);
	    (yyval.type)->comment = (yyvsp[-1].str)->comment;
	    (yyval.type)->modifiers |= CType::M_UNSIGNED;
	}
#line 3402 "y.tab.c"
    break;

  case 171: /* type_primitive: FLOAT  */
#line 1003 "parser.y"
               { (yyval.type) = new PrimitiveCType((yyvsp[0].str)->comment, PrimitiveCType::T_FLOAT); }
#line 3408 "y.tab.c"
    break;

  case 172: /* type_primitive: DOUBLE  */
#line 1004 "parser.y"
               { (yyval.type) = new PrimitiveCType((yyvsp[0].str)->comment, PrimitiveCType::T_DOUBLE); }
#line 3414 "y.tab.c"
    break;

  case 173: /* type_primitive: BOOL  */
#line 1005 "parser.y"
               { (yyval.type) = new PrimitiveCType((yyvsp[0].str)->comment, PrimitiveCType::T_BOOL); }
#line 3420 "y.tab.c"
    break;

  case 174: /* type_primitive: VOID  */
#line 1006 "parser.y"
               { (yyval.type) = new VoidCType((yyvsp[0].str)->comment); }
#line 3426 "y.tab.c"
    break;

  case 175: /* type_primitive: object_type_primitive  */
#line 1007 "parser.y"
                              { (yyval.type) = (yyvsp[0].type); }
#line 3432 "y.tab.c"
    break;

  case 176: /* type_primitive: enum_decl  */
#line 1008 "parser.y"
                  { (yyval.type) = (yyvsp[0].type); }
#line 3438 "y.tab.c"
    break;

  case 177: /* object_type_primitive: scoped_id  */
#line 1013 "parser.y"
        {
	    (yyval.type) = new ObjectCType((yyvsp[0].str)->comment, (yyvsp[0].str)->val,
		global.pool.add(local.source_file));
	}
#line 3447 "y.tab.c"
    break;

  case 178: /* object_type_primitive: STRUCT scoped_id  */
#line 1018 "parser.y"
        {
	    (yyval.type) = new ObjectCType((yyvsp[0].str)->comment, (yyvsp[0].str)->val,
		global.pool.add(local.source_file));
	}
#line 3456 "y.tab.c"
    break;

  case 179: /* object_type_primitive: UNION scoped_id  */
#line 1023 "parser.y"
        {
	    (yyval.type) = new ObjectCType((yyvsp[0].str)->comment, (yyvsp[0].str)->val,
		global.pool.add(local.source_file));
	}
#line 3465 "y.tab.c"
    break;

  case 180: /* object_type_primitive: scoped_id '<' type_list '>'  */
#line 1028 "parser.y"
        {
	    ObjectCType* t = new ObjectCType((yyvsp[-3].str)->comment, (yyvsp[-3].str)->val,
		global.pool.add(local.source_file));
	    t->args = (yyvsp[-1].arg); // make it a template expansion
	    (yyval.type) = t;
	}
#line 3476 "y.tab.c"
    break;

  case 181: /* type_indirection: '*'  */
#line 1037 "parser.y"
            { (yyval.immediate) = CType::M_POINTER; }
#line 3482 "y.tab.c"
    break;

  case 182: /* type_indirection: '&'  */
#line 1038 "parser.y"
            { (yyval.immediate) = CType::M_REFERENCE; }
#line 3488 "y.tab.c"
    break;

  case 183: /* integral_type_primitive: CHAR  */
#line 1043 "parser.y"
        {
	    (yyval.type) = new PrimitiveCType((yyvsp[0].str)->comment, PrimitiveCType::T_CHAR);
	}
#line 3496 "y.tab.c"
    break;

  case 184: /* integral_type_primitive: INT  */
#line 1047 "parser.y"
        {
	    (yyval.type) = new PrimitiveCType((yyvsp[0].str)->comment, PrimitiveCType::T_INT);
	}
#line 3504 "y.tab.c"
    break;

  case 185: /* integral_type_primitive: SHORT optional_int  */
#line 1051 "parser.y"
        {
	    (yyval.type) = new PrimitiveCType((yyvsp[-1].str)->comment, PrimitiveCType::T_SHORT);
	}
#line 3512 "y.tab.c"
    break;

  case 186: /* integral_type_primitive: LONG optional_long  */
#line 1055 "parser.y"
        {
	    (yyval.type) = new PrimitiveCType((yyvsp[-1].str)->comment, PrimitiveCType::T_LONG);
	}
#line 3520 "y.tab.c"
    break;

  case 188: /* optional_int: INT  */
#line 1060 "parser.y"
                              {}
#line 3526 "y.tab.c"
    break;

  case 190: /* optional_long: INT  */
#line 1061 "parser.y"
                               {}
#line 3532 "y.tab.c"
    break;

  case 191: /* optional_long: LONG  */
#line 1061 "parser.y"
                                         {}
#line 3538 "y.tab.c"
    break;

  case 192: /* scope_type: PUBLIC  */
#line 1064 "parser.y"
               { local.public_scope = PUBLIC; }
#line 3544 "y.tab.c"
    break;

  case 193: /* scope_type: PROTECTED  */
#line 1065 "parser.y"
                  { local.public_scope = PROTECTED; }
#line 3550 "y.tab.c"
    break;

  case 194: /* scope_type: PRIVATE  */
#line 1066 "parser.y"
                { local.public_scope = PRIVATE; }
#line 3556 "y.tab.c"
    break;

  case 195: /* opt_constant_expr: %empty  */
#line 1070 "parser.y"
                  { (yyval.expr) = -1; }
#line 3562 "y.tab.c"
    break;

  case 196: /* opt_constant_expr: constant_expr  */
#line 1071 "parser.y"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 3568 "y.tab.c"
    break;

  case 197: /* constant_expr_list: constant_expr  */
#line 1074 "parser.y"
                      {}
#line 3574 "y.tab.c"
    break;

  case 198: /* constant_expr_list: constant_expr_list ',' constant_expr  */
#line 1075 "parser.y"
                                             {}
#line 3580 "y.tab.c"
    break;

  case 204: /* constant_expr: logic_or_expr '?' c_expr ':' constant_expr  */
#line 1086 "parser.y"
                                                   { (yyval.expr) = -1; }
#line 3586 "y.tab.c"
    break;

  case 206: /* logic_or_expr: logic_or_expr OROR logic_and_expr  */
#line 1091 "parser.y"
                                          { (yyval.expr) = (yyvsp[-2].expr) || (yyvsp[0].expr) ? 1 : 0; }
#line 3592 "y.tab.c"
    break;

  case 208: /* logic_and_expr: logic_and_expr ANDAND inclusive_or_expr  */
#line 1096 "parser.y"
                                                { (yyval.expr) = (yyvsp[-2].expr) && (yyvsp[0].expr) ? 1 : 0; }
#line 3598 "y.tab.c"
    break;

  case 210: /* inclusive_or_expr: inclusive_or_expr '|' exclusive_or_expr  */
#line 1101 "parser.y"
                                                { (yyval.expr) = (yyvsp[-2].expr) | (yyvsp[0].expr); }
#line 3604 "y.tab.c"
    break;

  case 212: /* exclusive_or_expr: exclusive_or_expr '^' and_expr  */
#line 1106 "parser.y"
                                       { (yyval.expr) = (yyvsp[-2].expr) ^ (yyvsp[0].expr); }
#line 3610 "y.tab.c"
    break;

  case 214: /* and_expr: and_expr '&' equality_expr  */
#line 1111 "parser.y"
                                   { (yyval.expr) = (yyvsp[-2].expr) & (yyvsp[0].expr); }
#line 3616 "y.tab.c"
    break;

  case 216: /* equality_expr: equality_expr EQUALSEQUALS relational_expr  */
#line 1116 "parser.y"
                                                   { (yyval.expr) = (yyvsp[-2].expr) == (yyvsp[0].expr) ? 1 : 0; }
#line 3622 "y.tab.c"
    break;

  case 217: /* equality_expr: equality_expr NOTEQUALS relational_expr  */
#line 1117 "parser.y"
                                                { (yyval.expr) = (yyvsp[-2].expr) != (yyvsp[0].expr) ? 1 : 0; }
#line 3628 "y.tab.c"
    break;

  case 219: /* relational_expr: relational_expr '<' shift_expr  */
#line 1122 "parser.y"
                                       { (yyval.expr) = (yyvsp[-2].expr) < (yyvsp[0].expr) ? 1 : 0; }
#line 3634 "y.tab.c"
    break;

  case 220: /* relational_expr: relational_expr '>' shift_expr  */
#line 1123 "parser.y"
                                       { (yyval.expr) = (yyvsp[-2].expr) > (yyvsp[0].expr) ? 1 : 0; }
#line 3640 "y.tab.c"
    break;

  case 221: /* relational_expr: relational_expr LEFTEQUALS shift_expr  */
#line 1124 "parser.y"
                                              { (yyval.expr) = (yyvsp[-2].expr) << (yyvsp[0].expr); }
#line 3646 "y.tab.c"
    break;

  case 222: /* relational_expr: relational_expr RIGHTEQUALS shift_expr  */
#line 1125 "parser.y"
                                               { (yyval.expr) = (yyvsp[-2].expr) >> (yyvsp[0].expr); }
#line 3652 "y.tab.c"
    break;

  case 224: /* shift_expr: shift_expr LEFTLEFT additive_expr  */
#line 1130 "parser.y"
                                          { (yyval.expr) = (yyvsp[-2].expr) << (yyvsp[0].expr); }
#line 3658 "y.tab.c"
    break;

  case 225: /* shift_expr: shift_expr RIGHTRIGHT additive_expr  */
#line 1131 "parser.y"
                                            { (yyval.expr) = (yyvsp[-2].expr) >> (yyvsp[0].expr); }
#line 3664 "y.tab.c"
    break;

  case 227: /* additive_expr: additive_expr '+' multiplicative_expr  */
#line 1136 "parser.y"
                                              { (yyval.expr) = (yyvsp[-2].expr) + (yyvsp[0].expr); }
#line 3670 "y.tab.c"
    break;

  case 228: /* additive_expr: additive_expr '-' multiplicative_expr  */
#line 1137 "parser.y"
                                              { (yyval.expr) = (yyvsp[-2].expr) - (yyvsp[0].expr); }
#line 3676 "y.tab.c"
    break;

  case 230: /* multiplicative_expr: multiplicative_expr '*' cast_expr  */
#line 1143 "parser.y"
                                          { (yyval.expr) = (yyvsp[-2].expr) * (yyvsp[0].expr); }
#line 3682 "y.tab.c"
    break;

  case 231: /* multiplicative_expr: multiplicative_expr '/' cast_expr  */
#line 1144 "parser.y"
                                          { (yyval.expr) = (yyvsp[-2].expr) / (yyvsp[0].expr); }
#line 3688 "y.tab.c"
    break;

  case 232: /* multiplicative_expr: multiplicative_expr '%' cast_expr  */
#line 1145 "parser.y"
                                          { (yyval.expr) = (yyvsp[-2].expr) % (yyvsp[0].expr); }
#line 3694 "y.tab.c"
    break;

  case 234: /* cast_expr: '(' type ')' cast_expr  */
#line 1150 "parser.y"
                               { (yyval.expr) = (yyvsp[0].expr); }
#line 3700 "y.tab.c"
    break;

  case 236: /* unary_expr: PLUSPLUS unary_expr  */
#line 1155 "parser.y"
                            { (yyval.expr) = (yyvsp[0].expr)++; }
#line 3706 "y.tab.c"
    break;

  case 237: /* unary_expr: MINUSMINUS unary_expr  */
#line 1156 "parser.y"
                              { (yyval.expr) = (yyvsp[0].expr)--; }
#line 3712 "y.tab.c"
    break;

  case 238: /* unary_expr: unary_operator cast_expr  */
#line 1158 "parser.y"
        {
	    switch((yyvsp[-1].immediate)) {
		case '&':
		case '*':
		    (yyval.expr) = -1;
		    break;

		case TILDE:
		    (yyval.expr) = ~ (yyvsp[0].expr);
		    break;

		case '!':
		    (yyval.expr) = ! (yyvsp[0].expr);
		    break;

		case '+':
		    (yyval.expr) = (yyvsp[0].expr);
		    break;

		case '-':
		    (yyval.expr) = - (yyvsp[0].expr);
		    break;
		
		default:
		    printf("  cxxwrap internal error at \"%s\":%d, token %c\n",
			__FILE__, __LINE__, (yyvsp[-1].immediate));
		    (yyval.expr) = -1;
		    break;
	    }
	}
#line 3747 "y.tab.c"
    break;

  case 239: /* unary_expr: SIZEOF unary_expr  */
#line 1188 "parser.y"
                          { (yyval.expr) = -1; }
#line 3753 "y.tab.c"
    break;

  case 240: /* unary_expr: SIZEOF '(' type ')'  */
#line 1189 "parser.y"
                            { (yyval.expr) = -1; }
#line 3759 "y.tab.c"
    break;

  case 242: /* postfix_expr: postfix_expr array_type_indirection  */
#line 1194 "parser.y"
                                            { (yyval.expr) = -1; }
#line 3765 "y.tab.c"
    break;

  case 243: /* postfix_expr: postfix_expr '(' opt_c_expr_list ')'  */
#line 1195 "parser.y"
                                             { (yyval.expr) = -1; }
#line 3771 "y.tab.c"
    break;

  case 244: /* postfix_expr: postfix_expr '.' id  */
#line 1196 "parser.y"
                            { (yyval.expr) = -1; }
#line 3777 "y.tab.c"
    break;

  case 245: /* postfix_expr: postfix_expr ARROW id  */
#line 1197 "parser.y"
                              { (yyval.expr) = -1; }
#line 3783 "y.tab.c"
    break;

  case 246: /* postfix_expr: postfix_expr PLUSPLUS  */
#line 1198 "parser.y"
                              { (yyval.expr) = (yyvsp[-1].expr)++; }
#line 3789 "y.tab.c"
    break;

  case 247: /* postfix_expr: postfix_expr MINUSMINUS  */
#line 1199 "parser.y"
                                { (yyval.expr) = (yyvsp[-1].expr)--; }
#line 3795 "y.tab.c"
    break;

  case 248: /* primary_expr: scoped_id  */
#line 1204 "parser.y"
        {
	    /* handle NULL as "0", even though this is not
	       strictly true by ANSI rules, it is always the case
	       on "all" systems. */

	    if (0 == strcmp((yyvsp[0].str)->val, "NULL"))
	    {
		(yyval.expr) = 0;
	    }
	    else
	    {
		(yyval.expr) = -1;
	    }
	}
#line 3814 "y.tab.c"
    break;

  case 249: /* primary_expr: NEW scoped_id  */
#line 1218 "parser.y"
                      { (yyval.expr) = -1; }
#line 3820 "y.tab.c"
    break;

  case 250: /* primary_expr: NUM  */
#line 1219 "parser.y"
            { (yyval.expr) = (int) (yyvsp[0].number)->val; }
#line 3826 "y.tab.c"
    break;

  case 251: /* primary_expr: STRING  */
#line 1221 "parser.y"
        {
	    /* Map string constants to NULL, as well...
	       Not much useful we can do with them here. */

	    (yyval.expr) = 0;
	}
#line 3837 "y.tab.c"
    break;

  case 252: /* primary_expr: '(' constant_expr ')'  */
#line 1227 "parser.y"
                              { (yyval.expr) = (yyvsp[-1].expr); }
#line 3843 "y.tab.c"
    break;

  case 253: /* c_expr_list: assignment_expr  */
#line 1231 "parser.y"
                        {}
#line 3849 "y.tab.c"
    break;

  case 254: /* c_expr_list: c_expr_list ',' assignment_expr  */
#line 1232 "parser.y"
                                        {}
#line 3855 "y.tab.c"
    break;


#line 3859 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1240 "parser.y"


static struct {
  char *s;
  int n;
} lextable[] = {
  {"if", IF},
  {"else", ELSE},
  {"then", THEN},
  {"enum", ENUM},
  {"class", CLASS},
  {"public", PUBLIC},
  {"private", PRIVATE},
  {"protected", PROTECTED},
  {"new", NEW},
  {"delete", DELETE},
  {"virtual", VIRTUAL},
  {"volatile", VOLATILE},
  {"register", REGISTER},
  {"template", TEMPLATE},
  {"int", INT},
  {"float", FLOAT},
  {"short", SHORT},
  {"long", LONG},
  {"double", DOUBLE},
  {"void", VOID},
  {"char", CHAR},
  {"bool", BOOL},
  {"const", CONST},
  {"operator", OPERATOR},
  {"typedef", TYPEDEF},
  {"unsigned", UNSIGNED},
  {"friend", FRIEND},
  {"static", STATIC},
  {"sizeof", SIZEOF},
  {"struct", STRUCT},
  {"union", UNION},
  {"return", RETURN},
  {"inline", INLINE},
  {"extern", EXTERN},
  {NULL, 0}
};

/* Get the next character from the lexical source.
 */
int Lexer::get()
{
      lastc = c;
      c = EOF;

      switch(type)
      {
	case SRC_FILE:
	  if (ungetp > 0)
	  {
	    c = ungetbuf[--ungetp];
	  }
	  else
	  {
	    c = fgetc(source.f);
	  }
	  if(c != EOF) (ch)++;
	  break;

	case SRC_STRING:
	  if(eof || source.s[ch] == '\0') {
	    if(eof)
	      c = EOF;
	    else {
	      /* add an extra newline the first time */
	      c = '\n';
	      eof = 1;
	    }
	  }
	  else {
	    c = source.s[(ch)++];
	  }
	  break;
      }

      if(c == '\n') (line)++;

    return c;
}

void Lexer::unget(int cc)
{
	if(type == SRC_FILE)
	{
	    ungetbuf[ungetp++] = cc;
	    //ungetc(cc, source.f);
	}

	if(cc == '\n') --(line);

	if (eof) 
	  eof = 0;
	else
	  --(ch);
}

int Lexer::getNumber(double *d)
{
	int cc, occ = 0, l = 0;

	static const char* base10 = "+-.e01234567890fFuUlL";
	static const char* base16 = "+-01234567890ABCDEFabcdef";
	const char* ok = base10;

	while((cc = get()) != EOF && l < (LEXER_BUFLEN - 2))
	{
	    if (l == 1 && occ == '0' && (cc == 'x' || cc == 'X'))
	    {
		ok = base16;
	    }
	    else if (!strchr(ok, cc))
	    {
		break;
	    }

	    resultbuf[l++] = cc;
	    resultbuf[l] = '\0'; // always keep it terminated
	    if (l == 3 && strcmp("...", resultbuf) == 0)
		return 0;

	    occ = cc;
	}

	if (cc != EOF) unget(cc);

	resultbuf[l] = '\0';

	if (ok == base10)
	{
	    if (sscanf(resultbuf, "%lf", d) == 1) return 1;
	}
	else 
	{
	    int r;
	    if (sscanf(resultbuf, "%x", &r) == 1)
	    {
		*d = r;
		return 1;
	    }
	}

	return 0;
}

int Lexer::matchKeyword(char *keyword)
{
	int i;
	unsigned int l = strlen(keyword);

	for (i = 0; lextable[i].s != NULL; i++)
	{
	    unsigned int tl = strlen(lextable[i].s);
	    if (l == tl && strncmp(keyword, lextable[i].s, l) == 0)
	    {
		return i;
	    }
	}

	return -1;
}

#define isokforid(CHAR) (isalnum(CHAR) || (CHAR) == '_' || (CHAR) == '.')

static int lex_kernel(YYSTYPE* val)
{
    int i, c, which;
    double sign = 1.0;

    static int buflen = 0;
    static char *buf = NULL;

    /* eat white space */
    while ((c = local.lex.get()) == ' ' || c == '\t' || c == '\n');

    val->str = NULL;

    if (c == EOF) return 0;

    if (c == '#' && local.lex.last() == '\n')
    {
	i = 0;
	while ((c = local.lex.get()) != EOF && c != '\n')
	{
	    if (i >= (buflen - 2)) {
		buflen += 64;
		buf = (char*) realloc(buf, buflen * sizeof(char));
	    }
	    buf[i++] = c;
	}
	buf[i] = '\0';
	val->str = new String(NULL, buf);
	return CPP_DIRECTIVE;
    }

    // lexical tie-in for generating CTEXT operand...

    if (local.eatc)
    {
	int depth = 1;
	while (depth > 0 && c != EOF)
	{
	    if (c == '{') depth++;
	    if (c == '}') depth--;
	    c = local.lex.get();
	}

	local.eatc = 0; // automatically shut it off

	val->str = NULL;
	return CTEXT;
    }

    // Get enums as text
    if (local.copy_enum)
    {
        int i = 0, last = 0;
        char enum_buf[TOKEN_BUFFER_SIZE];
        while (c != '}' && c != ',' && c != EOF)
        {
            // Replace '\n' and '\t' with ' '
            if (c == '\n' || c == '\t') c = ' ';

            // Skip consecutive spaces
            if (last == ' ' && c == ' ')
            {
                c = local.lex.get();
                continue;
            }

            enum_buf[i++] = c;
            last = c;
            c = local.lex.get();
        }

        // If there is a trailing space, remove it
        if (i > 0 && enum_buf[i-1] == ' ') i--;
        
        enum_buf[i] = '\0'; // Strings are NULL-terminated
        local.lex.unget(c); // The parser also needs to see the terminating char
        local.copy_enum = 0; // automatically shut it off

        val->str = new String(NULL, global.pool.add(enum_buf));
        return CTEXT;
    }

    // oh, I need these to get comments on destructors, kachunk...

    if (c == '~')
    {
	val->str = new String(NULL, "~");
	return TILDE;
    }

    /* These can start a number, or be codes of their own.
     */
    if (c == '+' || c == '-') 
    {
        int csave = c;
        c = local.lex.get();
        if(c == '.' || isdigit(c)) {
	    sign = (csave == '-' ? -1.0 : + 1.0);
        }
	else if (csave == '-' && c == '>')
	{
	    // the arrow can be detected lexically
	    return ARROW;
	}
	else if (csave == c)
	{
	    return (c == '+' ? PLUSPLUS : MINUSMINUS);
	}
	else if (c == '=')
	{
	    return (csave == '+' ? PLUSEQUALS : MINUSEQUALS);
	}
        else
	{
	    local.lex.unget(c);
	    return csave;
        }
    }

    if (c == '>' || c == '<')
    {
	int csave = c;
	c = local.lex.get();
	if (c == csave)
	{
	    return (c == '<' ? LEFTLEFT : RIGHTRIGHT);
	}
	if (c == '=')
	{
	    return (csave == '<' ? LEFTEQUALS : RIGHTEQUALS);
	}
	local.lex.unget(c);
	return csave;
    }

    if (c == '&' || c == '|')
    {
	int csave = c;
	c = local.lex.get();
	if (c == csave)
	{
	    return (c == '&' ? ANDAND : OROR);
	}
	if (c == '=')
	{
	    return (csave == '&' ? ANDEQUALS : OREQUALS);
	}
	local.lex.unget(c);
	return csave;
    }

    if (c == '=')
    {
	int csave = c;
	c = local.lex.get();
	if (c == '=')
	{
	    return EQUALSEQUALS;
	}
	local.lex.unget(c);
	return csave;
    }

    if (c == ':')
    {
	int csave = c;
	c = local.lex.get();
	if (c == ':')
	{
	    return COLONCOLON;
	}
	local.lex.unget(c);
	return csave;
    }

    if (c == '!')
    {
	int csave = c;
	c = local.lex.get();
	if (c == '=')
	{
	    return NOTEQUALS;
	}
	local.lex.unget(c);
	return csave;
    }

    if (c == '.' || isdigit(c))
    {
	local.lex.unget(c);
	double d;
	if(local.lex.getNumber(&d) == 1)
	{
	    d *= sign;
	    val->number = new Number(NULL, d, local.lex.resultbuf);
	    return NUM;
	}
	else if (strcmp("...", local.lex.resultbuf) == 0)
	{
	    return ELLIPSIS;
	}
	return c; // try to revover...
    }

    if (c == '/')
    {
	int csave = c;
	c = local.lex.get();
	if (c == '=')
	{
	    return SLASHEQUALS;
	}
	if (c == '/')
	{
	    i = 0;
	    for(;;)
	    {
		// build comment text from the line
		while ((c = local.lex.get()) != EOF && c != '\n')
		{
		    if (i >= (buflen - 2)) {
			buflen += 64;
			buf = (char*) realloc(buf, buflen * sizeof(char));
		    }
		    buf[i++] = c;
		}

		// eat white space to see if we get another slash-slash
		while ((c = local.lex.get()) == ' ' || c == '\t');
		if (c != '/')
		{
		    local.lex.unget(c);
		    break;
		}
		else
		{
		    if ((c = local.lex.get()) != '/')
		    {
			local.lex.unget(c);
			local.lex.unget('/');
			break;
		    }
		    if (i >= (buflen - 2)) {
			buflen += 64;
			buf = (char*) realloc(buf, buflen * sizeof(char));
		    }
		    buf[i++] = '\n';
		}
	    }
	    local.lex.unget('\n'); // put back to help parsing cpp directives
	    buf[i] = '\0';
	    val->str = new String(NULL, buf);
	    return COMMENT;
	}
	else if (c == '*')
	{
	    i = 0;
	    int sawstar = 0;
	    while ((c = local.lex.get()) != EOF)
	    {
		if (sawstar && c == '/') break;
		if (i >= buflen) {
		    buflen += 64;
		    buf = (char*) realloc(buf, buflen * sizeof(char));
		}
		buf[i++] = c;
		sawstar = c == '*' ? 1 : 0;
	    }
	    buf[i - 1] = '\0';
	    val->str = new String(NULL, buf);
	    return COMMENT;
	}
	else
	{
	    local.lex.unget(c);
	    return csave;
	}
    }

    if (c == '*')
    {
	int csave = c;
	c = local.lex.get();
	if (c == '=')
	{
	    return STAREQUALS;
	}
	local.lex.unget(c);
	return csave;
    }

    if(isalpha(c) || c == '_')
    {
	i = 0;
	while (c != EOF && isokforid(c))
	{
	    if (i >= buflen) {
		buflen += 64;
		buf = (char*) realloc(buf, buflen * sizeof(char));
	    }
	    buf[i++] = c;
	    c = local.lex.get();
	}

	local.lex.unget(c);
	buf[i] = '\0';

	/* If this is a keyword, return the proper code */
	if ((which = local.lex.matchKeyword(buf)) >= 0) {
	    val->str = new String(NULL, buf);
	    return lextable[which].n;
	}

	/* Finally, it must be a new identifier */
	val->str = new String(NULL, buf);
	return IDENTIFIER;
    }

    if(c == '"')
    {
	i = 0;
	do {
	    c = local.lex.get();
	    if(i >= buflen) {
		buflen += 64;
		buf = (char*) realloc(buf, buflen * sizeof(char));
	    }

	    if(c == '\\') {
		int cc = local.lex.get();
		switch(cc) {
		    case 't': cc = '\t'; break;
		    case 'n': cc = '\n'; break;
		    default: break;
		}
		buf[i++] = cc;
	    }
	    else
	    {
		buf[i++] = c;
	    }

	} while(c != EOF && c != '"');

	buf[i - 1] = '\0';
	val->str = new String(NULL, buf);
	return STRING;
    }

    return c;
}

static int lex_no_context(YYSTYPE* val)
{
    String* latest_comment = NULL;
    int s;
    while ((s = lex_kernel(val)) == COMMENT || s == CPP_DIRECTIVE) 
    {
	if (s == COMMENT)
	{
	    latest_comment = val->str;
	    if (local.trace)
	    {
		printf("{comment} ");
	    }
	}
    }

    if (local.trace)
    {
	int i;
	int found = 0;
	for (i = 0; lextable[i].s != NULL; i++) {
	    if(s == lextable[i].n) {
		printf("[%s] ", lextable[i].s);
		found = 1;
		break;
	    }
	}
	
	if (!found) {
	    switch (s) {
	    case STRING: printf("[\"%s\"] ", val->str->val); break;
	    case IDENTIFIER: printf("<%s> ", val->str->val); break;
	    case NUM: printf("#%s=%.2f ", val->number->strval, val->number->val); break;
	    case TILDE: printf("(~) "); break;
	    case ARROW: printf("(->) "); break;
	    case ELLIPSIS: printf("(...) "); break;
	    case COMMENT: printf("{comment} "); break;
	    case CPP_DIRECTIVE: printf("[cpp %s] ", val->str->val); break;
	    case CTEXT: printf("[ctext] "); break;
	    case COLONCOLON: printf("(::) "); break;
	    case PLUSEQUALS: printf("(+=) "); break;
	    case MINUSEQUALS: printf("(-=) "); break;
	    case PLUSPLUS: printf("(++) "); break;
	    case MINUSMINUS: printf("(--) "); break;
	    case STAREQUALS: printf("(*=)"); break;
	    case SLASHEQUALS: printf("(/=)"); break;
	    case LEFTEQUALS: printf("(<=)"); break;
	    case RIGHTEQUALS: printf("(>=)"); break;
	    case LEFTLEFT: printf("(<<)"); break;
	    case RIGHTRIGHT: printf("(>>)"); break;
	    case ANDAND: printf("(&&)"); break;
	    case OROR: printf("(||)"); break;
	    case OREQUALS: printf("(|=)"); break;
	    case ANDEQUALS: printf("(&=)"); break;
	    default: printf("(%c) ", s); break;
	    }
	}
	fflush(stdout);
    }

    if (latest_comment)
    {
	switch(s) {
	    case NUM:
		val->number->comment = latest_comment->val;
		break;

	    default:
		if (val->str)
		    val->str->comment = latest_comment->val; 
		break;
	}
	if (local.trace) printf("{assigned comment} ");
    }

    return s;
}

static int myyylex(void)
{ 
    return lex_no_context(&yylval);
}

static const char* msg(const char *s)
{
    static char buf[256];

    if (local.lex.type == Lexer::SRC_STRING)
    {
        int first, last, c;
        first = local.lex.ch - 16;
        if(first < 0) first = 0;
        for(last = first; last < first + 32; last++)
	    if (local.lex.source.s[last] == '\0' ||
		local.lex.source.s[last] == '\n') break;
        c = local.lex.source.s[last];
        local.lex.source.s[last] = '\0';

        sprintf(buf, "%s (line %d) near \"%s\"\n",
	    s, local.lex.line + 1, local.lex.source.s + first);
        local.lex.source.s[last] = c;
    }
    else
    {
        sprintf(buf, "%s (\"%s\":%d)\n",
	    s, local.source_file, local.lex.line + 1);
    }

    return buf;
}

static void myyyerror(const char* /* s */)
{
    local.nerror++;
    if (global.verbose || local.trace)
    {
	/* ignore the passed string, which says "parse error"
	 * or some such.
	 */
	printf("  %s", msg("syntax not parsed correctly"));
    }
}

static void global_reset()
{
    local.public_scope = PRIVATE;
    local.class_nest = 0;
}

static void push_scope(int s)
{
    if (local.class_nest > MAX_CLASS_NEST)
    {
	printf("  %s", msg("classes nested too deep"));
    }
    else
    {
	local.saved_public_scope[local.class_nest++] = local.public_scope;
	local.public_scope = s;
    }
}

static void pop_scope()
{
    if (local.class_nest < 1)
    {
	printf("  cxxwrap internal error \"%s\":%d\n", __FILE__, __LINE__);
    }
    else
    {
	local.public_scope = local.saved_public_scope[--local.class_nest];
    }
}

static int check_new_classdef(Class* c)
{
    const char* fn = c->fullName();
    if (global.classJar.findClassByName(fn))
    {
	char buf[256];
	sprintf(buf, "duplicate definition for class \"%s\" ignored.\n", fn);
	warning(msg(buf));
	return -1;
    }
    return 0;
}

static int do_scan_cxx_file(FILE *f)
{
    local.serial = 1;
    local.lex.type = Lexer::SRC_FILE;
    local.lex.source.f = f;
    local.lex.line = local.lex.ch = 0;
    local.lex.eof = 0;
    local.lex.c = '\n'; /* to be stored as the last read character */
    local.lex.ungetp = 0;
    local.nerror = 0;
    local.nclassdef = 0;
    local.eatc = 0;
    local.copy_enum = 0;
    local.typedefs.clear();

    global_reset();
    set_warning_context("parsing");

    yyparse();

    set_warning_context(NULL);

    if (local.nclassdef == 0)
	return -1;

    return 0;
}

int scan_cxx_file(const char* root, const char* fn, int trace)
{
    char ffn[FILENAME_MAX];
    if (root)
    {
	sprintf(ffn, "%s%c%s", root, PATHSEP, fn);
    }
    else
    {
	strcpy(ffn, fn);
    }
    FILE* f = fopen(ffn, "r");
    if (!f)
    {
	printf("cannot open file \"%s\"\n", ffn);
	return -1;
    }

    local.trace = trace;

    if (global.verbose)
    {
	printf("scanning \"%s\":\n", fn);
    }
    local.source_file = fn;
    if (do_scan_cxx_file(f) != 0)
    {
	if (global.verbose)
	{
	    printf("  %s", msg("no classes found"));
	}
    }
    fclose(f);
    fflush(stdout);
    return 0;
}
