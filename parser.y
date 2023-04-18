%{
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

%}

%union {
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
}

%token<str> CLASS
%token<str> STRUCT
%token<str> UNION
%token<str> TEMPLATE
%token<str> PUBLIC
%token<str> PRIVATE
%token<str> PROTECTED
%token<str> VIRTUAL
%token<str> IF
%token<str> THEN
%token<str> ELSE
%token<str> STRING
%token<str> ENUM
%token<str> NEW
%token<str> DELETE
%token<number> NUM
%token PLUSPLUS
%token MINUSMINUS
%token EQUALSEQUALS
%token PLUSEQUALS
%token MINUSEQUALS
%token STAREQUALS
%token SLASHEQUALS
%token LEFTEQUALS
%token RIGHTEQUALS
%token LEFTLEFT
%token RIGHTRIGHT
%token ANDAND
%token OROR
%token OREQUALS
%token ANDEQUALS
%token COLONCOLON
%token ELLIPSIS
%token<str> CTEXT
%token<str> TILDE
%token<str> IDENTIFIER
%token<str> INT
%token<str> FLOAT
%token<str> SHORT
%token<str> LONG
%token<str> DOUBLE
%token<str> VOID
%token<str> CHAR
%token<str> BOOL
%token<str> COMMENT
%token<str> CPP_DIRECTIVE
%token<str> CONST
%token<str> VOLATILE
%token<str> REGISTER
%token<str> OPERATOR
%token<str> UNSIGNED
%token<str> FRIEND
%token<str> STATIC
%token<str> SIZEOF
%token<str> TYPEDEF
%token<str> INLINE
%token<str> EXTERN
%token<str> RETURN

%right '='
%right ANDEQUALS OREQUALS
%right PLUSEQUALS MINUSEQUALS
%right STAREQUALS SLASHEQUALS
%right '?' ':'
%left OROR ANDAND
%left ARROW
%left '|'
%left '^'
%left '&'
%left EQUALSEQUALS NOTEQUALS
%left LEFTLEFT RIGHTRIGHT
%left '+' '-'
%left '*' '/' '%'
%right PLUSPLUS MINUSMINUS
%left '(' '[' 
%left SHORT, LONG

/* Add precedence rules to solve dangling else s/r conflict */
%nonassoc IF
%nonassoc ELSE

%type<str> id, opt_id, opt_scoped_id, scoped_id, id_list, opt_var_assign, opt_var_assign_text, cpp_operator, assignment_operator
%type<method> class_def_body, class_def_item, scoped_class_def_item, friend_decl, method_decl, var_decl, func
%type<arg> arg_list, culled_arg_list, type_list, arg, func_arg, enum_item, enum_item_list, template_par, template_par_list
%type<type> type, type_mod, type_mods, type_red1, object_type_primitive, type_primitive, integral_type_primitive, type_decl, enum_decl
%type<objtype> parent_spec, parent_decl, parent_list
%type<clazz> class_decl
%type<immediate> array_type_indirection, type_indirection, func_end, func_mods, unary_operator, binary_operator, opt_bitfield
%type<immediates> array_type_indirections
%type<expr> constant_expr, opt_constant_expr, unary_expr, postfix_expr, primary_expr, cast_expr, multiplicative_expr, additive_expr, shift_expr, relational_expr
%type<expr> equality_expr, and_expr, inclusive_or_expr, exclusive_or_expr, logic_and_expr, logic_or_expr, assignment_expr, c_expr

%%

module: /* nil */ | module file_scope_item ;

file_scope_item:
	class_def { }
    |   type_decl { if ($1->alias) global.typedefs.put($1->alias, $1); }
    |	var_decl opt_var_assign ';' { }
    |	method_decl { }
    |   error { /* best recovery is with no action */ }
    ;

class_def:
        TEMPLATE '<' template_par_list '>' CLASS id ';' { }
    |   TEMPLATE '<' template_par_list '>' class_decl ';'
	{
	    $5->comment = $1->comment;
	    $5->setParameters($3);
	    if (0 == check_new_classdef($5))
	    {
		global.classJar.add($5);
		local.nclassdef++;
	    }
	}
    |	class_decl ';'
	{
	    if (0 == check_new_classdef($1))
	    {
		global.classJar.add($1);
		local.nclassdef++;
	    }
	}
    |	STRUCT scoped_id ';' { }
    |	UNION scoped_id ';' { }
    |	CLASS scoped_id ';' { }
    ;

template_par_list:
	template_par
	{
	    $$ = $1;
	}
    |   template_par ',' template_par_list
	{
	    $$ = $1;
	    $$->next = $3;
	}
    ;

template_par:
	CLASS id
	{
	    CType* t = new ObjectCType($1->comment, $2->val, NULL);
	    $$ = new Arg($1->comment, $2->val, t);
	}
    |	id
	{
	    CType* t = new ObjectCType($1->comment, $1->val, NULL);
	    $$ = new Arg($1->comment, $1->val, t);
	}
    ;

class_decl:
        CLASS scoped_id parent_spec {
	    push_scope(PRIVATE); local.typedefs.clear();
	    } '{' class_def_body '}'
	{
	    ObjectCType* t = new ObjectCType(NULL, $2->val,
		global.pool.add(local.source_file));
	    $$ = new Class($1->comment, t, $3, $6);
	    $$->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  class %s\n", $$->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
    |	CLASS id scoped_id parent_spec {
	    push_scope(PRIVATE); local.typedefs.clear();
	    } '{' class_def_body '}'
	{
	    /* assume first id is an export spec */
	    ObjectCType* t = new ObjectCType(NULL, $3->val,
		global.pool.add(local.source_file));
	    $$ = new Class($1->comment, t, $4, $7);
	    $$->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  class %s %s\n", $2->val, $$->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
    |	STRUCT opt_id {
	    push_scope(PUBLIC); local.typedefs.clear();
	    } '{' class_def_body '}'
	{
	    ObjectCType* t = new ObjectCType(NULL, $2->val,
		global.pool.add(local.source_file));
	    $$ = new Class($1->comment, t, NULL, $5);
	    $$->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  struct %s\n", $$->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
    |	UNION opt_id {
	    push_scope(PUBLIC); local.typedefs.clear();
	    } '{' class_def_body '}'
	{
	    ObjectCType* t = new ObjectCType(NULL, $2->val,
		global.pool.add(local.source_file));
	    $$ = new Class($1->comment, t, NULL, $5);
	    $$->typedefs.copy(local.typedefs);
	    if (global.verbose)
	    {
		printf("  union %s\n", $$->name());
	    }
	    pop_scope();
	    local.typedefs.clear();
	}
    ;

parent_spec:
	/* nil */ { $$ = NULL; }
    |   ':' parent_list { $$ = $2; }
    ;

parent_list:
	parent_decl { $$ = $1; }
    |   parent_list ',' parent_decl
	{
	    warning(msg("multiple class inheritance ignored"));
	    $$ = $1;
	} 
    ;

parent_decl:
        PUBLIC scoped_id
	{
	    $$ = new ObjectCType(NULL, $2->val, NULL);
	}
    |   PUBLIC scoped_id '<' type_list '>'
	{
	    $$ = new ObjectCType(NULL, $2->val, NULL);
	    $$->args = $4;
	}
	/* ignore private or protected parents...
	 */
    |	PRIVATE scoped_id { $$ = NULL; }
    |	PROTECTED scoped_id { $$ = NULL; }
    ;

class_def_body:
    	scoped_class_def_item { $$ = $1; }
    |   class_def_body scoped_class_def_item
	{
	    $$ = $2;
	    if ($1)
	    {
	        Method* m = $1;
		while (m->next) m = m->next;
		m->next = $2;
		$$ = $1;
	    }
	}
    ;

scoped_class_def_item:
	class_def_item
	{
	    $$ = $1;
	    while ($$)
	    {
		if (local.public_scope == PROTECTED)
		{
		    $$->type->modifiers |= CType::M_PROTECTED;
		}
		if (local.public_scope == PRIVATE)
		{
		    $$->type->modifiers |= CType::M_PRIVATE;
		}
		$$ = $$->next;
	    }
	    $$ = $1;
	}
    ;

class_def_item:
	scope_type ':' { $$ = NULL; }
    |   var_decl opt_bitfield ';'
	{
	    $$ = $1;
	    while ($$)
	    {
		$$->type->modifiers |= CType::M_ATTRIBUTE;
		$$ = $$->next;
	    }
	    $$ = $1;
	}
    |   method_decl
	{
	    $$ = $1;
	} 
	/* Assign class enums and typedefs to "methods", since they
	 * operate at class level and we don't have a better model.
	 */
    |   type_decl
	{
	    if ($1 && ($1->modifiers & CType::M_ENUM))
	    {
		$$ = new Method($1->comment, $1->CTypeName(), NULL);
		$$->type = $1;
		$$->notes |= Method::ENUM_DEFINE;
	    }
	    else if ($1 && ($1->modifiers & CType::M_TYPEDEF))
	    {
                CType *ct = local.typedefs.get($1->alias);
                if (ct)
		{
		    char buf[512];
		    sprintf(buf, "overriding old typedef %s (%s) with %s (%s)\n",
			ct->alias, ct->CTypeName(), $1->alias, $1->CTypeName());
		    warning(msg(buf));
                }
                local.typedefs.put($1->alias, $1);
		$$ = new Method($1->comment, $1->alias, NULL);
		$$->type = $1;
	    }
	    else
	    {
		$$ = NULL;
	    }
	}
    |   friend_decl { $$ = $1; }
    |	class_decl opt_id ';' { $$ = NULL; }
    |	TYPEDEF class_decl opt_id ';' { $$ = NULL; }
    |   error { $$ = NULL; }
    ;

opt_bitfield:
	/* nil */ { $$ = 0; }
    |   ':' NUM { $$ = (int) $2->val; }
    ;

opt_id:
	/* nil */ { $$ = new String(NULL, ""); }
    |	id { $$ = $1; }
    ;

var_decl:
	type id_list
	{
	    char* sep = "|";
	    const char* p = strtok((char*) $2->val, sep);
	    Method* last = NULL;
	    $$ = NULL;
	    while (p)
	    {
		Method* m = new Method($1->comment, p, NULL);
		m->type = $1;
		if (!($$)) $$ = m;
		if (last) last->next = m;
		last = m;
		p = strtok(NULL, sep);
	    }
	}
    |   type id array_type_indirections
        {
	    $$ = new Method($1->comment, $2->val, NULL);
	    if ($3[0] >= 0)
	    {
                $$->type = $1;
                while (--$3[0]) $$->type = new ArrayCType($$->type, $3[$3[0]+1]);
                
                if ($3[1] >= 0)
                {
                    $$->type = new ArrayCType($$->type, $3[$3[0]+1]);
                }
                else
                {
                    $$->type->indirection_count++;
                    $$->type->modifiers |= CType::M_POINTER;
                }
	    }
	    else
	    {
		$$->type = $1;
		$$->type->indirection_count++;
		$$->type->modifiers |= CType::M_POINTER;
	    }
	}
    |   type '(' '*' id ')' '(' culled_arg_list ')'
        {
            $$ = NULL;
        }
    ;

id_list:
	id 
	{
	    $$ = $1;
	}
    |   id ',' id_list
	{
	    char buf[TOKEN_BUFFER_SIZE];
	    sprintf(buf, "%s|%s", $1->val, $3->val);
	    $$ = new String($1->comment, buf);
	}
    ;

friend_decl:
	FRIEND CLASS scoped_id ';' { $$ = NULL; }
    |	FRIEND STRUCT scoped_id ';' { $$ = NULL; }
    |	FRIEND UNION scoped_id ';' { $$ = NULL; }
    |	FRIEND method_decl
        {
            if ($2->is_operator)
            {
                $$ = $2;
                $$->type->modifiers |= CType::M_STATIC;
            }
            else
            {
                $$ = NULL;
            }
        }
    ;

type_decl:
	TYPEDEF type id ';'
	{
	    $$ = $2;
	    $$->comment = $1->comment;
	    $$->alias = $3->val;
	    $$->modifiers |= CType::M_TYPEDEF;
	}
    |   TYPEDEF type id array_type_indirections ';'
	{
	    if ($4 != NULL)
	    {
                $$ = $2;
                while (--$4[0]) $$ = new ArrayCType($$, $4[$4[0]+1]);
                
                if ($4[1] >= 0)
                {
                    $$ = new ArrayCType($$, $4[$4[0]+1]);
                }
                else
                {
                    $$->indirection_count++;
                    $$->modifiers |= CType::M_POINTER;
                }
	    }
	    else
	    {
		$2->indirection_count++;
		$2->modifiers |= CType::M_POINTER;
                $$ = $2;
	    }
            $$->alias = $3->val;
            $$->modifiers |= CType::M_TYPEDEF;
	}
    |   TYPEDEF func_arg ';'
	{
            $$ = $2->type;
	    $$->comment = $1->comment;
	    $$->alias = $2->name;
	    $$->modifiers |= CType::M_TYPEDEF;
	}
    |   enum_decl ';' { $$ = $1; }
    ;

enum_decl:
        ENUM scoped_id '{' enum_item_list '}'
	{
	    $$ = new EnumCType($1->comment, $2->val, $4);
	}
    |   ENUM '{' enum_item_list '}'
	{
	    char buf[32];
	    sprintf(buf, "anon%d", local.serial++);
	    $$ = new EnumCType($1->comment, global.pool.add(buf), $3);
	}
    ;

enum_item_list:
	enum_item { $$ = $1; }
    |   enum_item_list ',' enum_item
	{
	    $$ = $3;
	    if ($1)
	    {
	        Arg* a = $1;
		while (a->next) a = a->next;
		a->next = $3;
		$$ = $1;
	    }
	}
    ;

enum_item:
        id opt_var_assign_text
	{
	    $$ = new Arg($1->comment, $1->val, NULL);
	    if ($2)
	    {
		$$->default_value = $2->val;
	    }
	}
    ;

opt_var_assign_text:
	/* nil */
	{
	    $$ = NULL;
	}
    |   '=' { local.copy_enum = 1; } CTEXT
        {
            $$ = $3;
        }
    ;

method_decl:
        TILDE func
	{
	    $$ = $2;
	    if ($$)
	    {
		$$->type = new CreateCType($1->comment, CType::M_DTOR);
		$$->comment = $1->comment;
	    }
	}
    |   VIRTUAL TILDE func
	{
	    $$ = $3;
	    if ($$)
	    {
		$$->type = new CreateCType($1->comment, CType::M_DTOR, CType::M_VIRTUAL);
		$$->comment = $1->comment;
	    }
	}
    |   func
	{
	    $$ = $1;
	    if ($$ && !$$->is_operator)
	    {
		$$->type = new CreateCType($1->comment);
		$$->comment = $1->comment;
	    }
	}
    |   type func
        {
	    $$ = $2;
	    if ($$)
	    {
		$$->type = $1;
		$$->comment = $1->comment;
	    }
	}
    |   VIRTUAL type func
        {
	    $$ = $3;
	    if ($$)
	    {
		$$->type = $2;
		$$->type->modifiers |= CType::M_VIRTUAL;
		$$->comment = $1->comment;
	    }
	}
    ;

func:
	scoped_id '(' culled_arg_list ')' func_mods func_end
	{
	    $$ = new Method($1->comment, $1->val, $3);
	    if ($5) $$->is_const = 1;
	    if ($6) $$->is_pure = 1;
	}
    |	opt_scoped_id OPERATOR cpp_operator '(' culled_arg_list ')' func_mods func_end
	{
            char buf[TOKEN_BUFFER_SIZE];
            sprintf(buf, "op%s", $3->val);
	    $$ = new Method("", global.pool.add(buf), $5);
            $$->is_operator = 1;
	}
    |	opt_scoped_id OPERATOR type '(' culled_arg_list ')' func_mods func_end
	{
	    $$ = new Method("", "opCast", $5);
            $$->type = $3;
            $$->comment = $3->comment;
            $$->is_operator = 1;
	}
    ;

culled_arg_list:
	arg_list
	{
	    /* remove an arg list which contains a single "void".
	     */
	    $$ = $1;
	    if ($$ && $$->next == NULL &&
		0 == strcmp($$->type->CTypeName(), "void"))
	    {
		$$ = NULL;
	    }
	}
    ;

cpp_operator:
	unary_operator
        {
	    char buf[2];
	    sprintf(buf, "%c", $1);
	    $$ = new String(buf);
        }
    |	binary_operator
        {
	    char buf[2];
	    sprintf(buf, "%c", $1);
	    $$ = new String(buf);
        }
    |   assignment_operator { $$ = $1; }
    |   PLUSPLUS { $$ = new String("++"); } | MINUSMINUS { $$ = new String("--"); }
    |   LEFTLEFT { $$ = new String("<<"); } | RIGHTRIGHT { $$ = new String(">>"); }
    |   ANDAND { $$ = new String("&&"); } | OROR { $$ = new String("||"); }
    |   EQUALSEQUALS { $$ = new String("=="); } | NOTEQUALS { $$ = new String("!="); }
    |   LEFTEQUALS { $$ = new String("<="); } | RIGHTEQUALS { $$ = new String(">="); }
    |   ARROW { $$ = new String("->"); }
    |   NEW { $$ = new String("new"); } | DELETE { $$ = new String("delete"); }
    |   array_type_indirection { $$ = new String("[]"); }
    ;

unary_operator:
	'&' { $$ = '&'; }
    |   '*' { $$ = '*'; }
    |   '+' { $$ = '+'; }
    |   '-' { $$ = '-'; }
    |   TILDE { $$ = '~'; }
    |   '!' { $$ = '!'; }
    ;

binary_operator:
        '/' { $$ = '/'; }
    |   '%' { $$ = '%'; }
    |   '|' { $$ = '|'; }
    |   '<' { $$ = '<'; }
    |   '>' { $$ = '>'; }
    |   '^' { $$ = '^'; }
    ;

assignment_operator:
        '=' { $$ = new String("="); }
    |   STAREQUALS { $$ = new String("*="); }
    |   SLASHEQUALS { $$ = new String("/="); }
    |   '%' '=' { $$ = new String("%="); }
    |   PLUSEQUALS { $$ = new String("+="); }
    |   MINUSEQUALS { $$ = new String("-="); }
    |   LEFTLEFT '='  { $$ = new String("<<="); }
    |   RIGHTRIGHT '='  { $$ = new String(">>="); }
    |   ANDEQUALS  { $$ = new String("&="); }
    |   '^' '='  { $$ = new String("^="); }
    |   OREQUALS  { $$ = new String("|="); }
    ;

id: IDENTIFIER { $$ = $1; } ;

opt_scoped_id:
        /* nil */ { $$ = NULL; }
    |   id COLONCOLON { $$ = $1; }
    ;

scoped_id:
	id { $$ = $1; }
    |   id COLONCOLON scoped_id
	{
	    char* buf = new char[8 + strlen($1->val) + strlen($3->val)];
	    sprintf(buf, "%s::%s", $1->val, $3->val);
	    $$ = new String($1->comment, buf);
	    delete[] buf;
	}
    ;

func_end:
	';' { $$ = 0; }
    |   func_end ';' { $$ = $1; }
    |   opt_init_list '{' { local.eatc = 1; } CTEXT { $$ = 0; }
    |   '=' NUM ';' { $$ = 1; }
    ;

func_mods: /* nil */ { $$ = 0; } | CONST { $$ = 1; } ;

opt_init_list : /* nil */ | ':' init_list ;

init_list: init_item | init_list ',' init_item ;

init_item: type '(' opt_constant_expr_list ')' { } ;

arg_list:
	/* nil */ { $$ = NULL; }
    |	arg { $$ = $1; }
    |   arg ',' arg_list
	{
	    $$ = $3;
	    if ($1)
	    {
		$1->next = $$;
		$$ = $1;
	    }
	}
    ;

arg:
        type opt_var_assign
	{
	    char buf[32];
	    sprintf(buf, "temp%d", local.serial++);
	    $$ = new Arg($1->comment, global.pool.add(buf), $1);
	    if ($2)
	    {
		$$->default_value = $2->val;
	    }
	}
    |   type id opt_var_assign
	{
	    $$ = new Arg($1->comment, $2->val, $1);
	    if ($3)
	    {
		$$->default_value = $3->val;
	    }
	}
    |   type opt_id array_type_indirections
	{
	    if ($3 != NULL)
	    {
                while (--$3[0]) $1 = new ArrayCType($1, $3[$3[0]+1]);
                
                if ($3[1] >= 0)
                {
                    $1 = new ArrayCType($1, $3[$3[0]+1]);
                }
                else
                {
                    $1->indirection_count++;
                    $1->modifiers |= CType::M_POINTER;
                }
	    }
	    else
	    {
		$1->indirection_count++;
		$1->modifiers |= CType::M_POINTER;
	    }
            if (strlen($2->val) == 0)
            {
                char buf[32];
                sprintf(buf, "temp%d", local.serial++);
                $2->val = global.pool.add(buf);
            }
            $$ = new Arg($1->comment, $2->val, $1);
	}
    |	func_arg
	{
	    $$ = $1;
	}
    |   ELLIPSIS
	{
	    CType* ct = new CType("", CType::M_VARARG);
	    $$ = new Arg("", "...", ct);
	}
    ;

func_arg:
        type id '(' culled_arg_list ')'
	{
	    FunctionCType* ct = new FunctionCType($1, $4);
            ct->indirection_count -= $1->indirection_count;
	    $$ = new Arg($1->comment, $2->val, ct);
	}
    |   type '(' id ')' '(' culled_arg_list ')'
	{
	    FunctionCType* ct = new FunctionCType($1, $6);
            ct->indirection_count -= $1->indirection_count;
	    $$ = new Arg($1->comment, $3->val, ct);
	}
    |   type '(' '*' id ')' '(' culled_arg_list ')'
	{
	    FunctionCType* ct = new FunctionCType($1, $7);
            ct->indirection_count -= $1->indirection_count - 1;
	    $$ = new Arg($1->comment, $4->val, ct);
	}
	;

opt_var_assign:
	/* nil */
	{
	    $$ = NULL;
	}
    |   '=' constant_expr
	{
	    char buf[128];
	    sprintf(buf, "%d", $2);
	    $$ = new String(NULL, buf);
	}
    ;

type_list:
	type
	{
	    $$ = new Arg($1->comment, "", $1);
	}
    |   type ',' type_list
	{
	    $$ = new Arg($1->comment, "", $1);
	    $$->next = $3;
	}
    ;

type:
	type_red1 { $$ = $1; }
    |   type_mods type_red1
	{
	    $$ = $2;
	    $$->comment = $1->comment;
	    $$->modifiers |= $1->modifiers;
	}
    |   type_red1 type_mods
	{
	    $$ = $1;
	    if ( ($$->modifiers && CType::M_POINTER) && ($2->modifiers & CType::M_CONST) )
		$$->modifiers |= CType::M_PTR_CONST;
	    else
		$$->modifiers |= $2->modifiers;
	}
    |   type_mods type_red1 type_mods
	{
	    $$ = $2;
	    $$->comment = $1->comment;
	    if ( ($$->modifiers && CType::M_POINTER) && ($3->modifiers & CType::M_CONST) )
		$$->modifiers |= CType::M_PTR_CONST;
	    else
		$$->modifiers |= $3->modifiers;
	}
    ;

type_mods: 
	type_mod { $$ = $1; }
    |   type_mods type_mod
	{
	    $$ = $2;
	    $$->comment = $1->comment;
	    $$->modifiers |= $1->modifiers;
	}
    ;

type_mod:
     	CONST { $$ = new CType($1->comment, CType::M_CONST); } 
    |   STATIC { $$ = new CType($1->comment, CType::M_STATIC); }
    |   VOLATILE { $$ = new CType($1->comment, CType::M_VOLATILE); }
    |   REGISTER { $$ = new CType($1->comment, CType::M_REGISTER); }
    |   INLINE { $$ = new CType($1->comment); }
    ;

type_red1:
	type_primitive { $$ = $1; }
    |   CLASS type_red1 { $$ = $2; }
    |   type_red1 type_indirection 
	{
	    $$ = $1;
	    $$->modifiers |= $2;
	    $$->indirection_count++;
	}
    ;

array_type_indirections:
        array_type_indirection
        {
            /* Array_type_indirections is a pointer to an array of
             * ints specifying the size of each dimension of the
             * array; the number of dimensions is stored in the first
             * element.
             */
            $$ = (int*)malloc(sizeof(int)*2);
            $$[0] = 1;
            $$[1] = $1;
        }
    |   array_type_indirections array_type_indirection
        {
            $$ = (int*)realloc($1, sizeof(int)*(1+(++$1[0])));
            $$[$$[0]] = $2;
        }
    ;

array_type_indirection:
	'[' opt_constant_expr ']' { $$ = $2; }
    ;

type_primitive: 
	integral_type_primitive { $$ = $1; }
    |   UNSIGNED integral_type_primitive
	{
	    $$ = $2;
	    $$->comment = $1->comment;
	    $$->modifiers |= CType::M_UNSIGNED;
	}
    |   FLOAT  { $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_FLOAT); }
    |	DOUBLE { $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_DOUBLE); }
    |	BOOL   { $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_BOOL); }
    |	VOID   { $$ = new VoidCType($1->comment); }
    |   object_type_primitive { $$ = $1; }
    |	enum_decl { $$ = $1; }
    ;

object_type_primitive:
     	scoped_id
	{
	    $$ = new ObjectCType($1->comment, $1->val,
		global.pool.add(local.source_file));
	}
    | 	STRUCT scoped_id
	{
	    $$ = new ObjectCType($2->comment, $2->val,
		global.pool.add(local.source_file));
	}
    | 	UNION scoped_id
	{
	    $$ = new ObjectCType($2->comment, $2->val,
		global.pool.add(local.source_file));
	}
    |   scoped_id '<' type_list '>'
	{
	    ObjectCType* t = new ObjectCType($1->comment, $1->val,
		global.pool.add(local.source_file));
	    t->args = $3; // make it a template expansion
	    $$ = t;
	}
    ;

type_indirection: 
        '*' { $$ = CType::M_POINTER; } 
    |	'&' { $$ = CType::M_REFERENCE; } 
    ;

integral_type_primitive:
     	CHAR
	{
	    $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_CHAR);
	}
    |	INT
	{
	    $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_INT);
	}
    |	SHORT optional_int
	{
	    $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_SHORT);
	}
    |   LONG optional_long
	{
	    $$ = new PrimitiveCType($1->comment, PrimitiveCType::T_LONG);
	}
    ;

optional_int: /* nil */ | INT {} ;
optional_long: /* nil */ | INT {} | LONG {} ;

scope_type:
	PUBLIC { local.public_scope = PUBLIC; }
    |	PROTECTED { local.public_scope = PROTECTED; }
    |   PRIVATE { local.public_scope = PRIVATE; }
    ;

opt_constant_expr:
	/* nil */ { $$ = -1; }
    |   constant_expr { $$ = $1; } ;

constant_expr_list:
	constant_expr {}
    |	constant_expr_list ',' constant_expr {}
    ;

opt_constant_expr_list: /* nil */ | constant_expr_list ;

c_expr: assignment_expr ;

assignment_expr: constant_expr ;

constant_expr:
	logic_or_expr
    |	logic_or_expr '?' c_expr ':' constant_expr { $$ = -1; }
    ;

logic_or_expr:
	logic_and_expr
    |	logic_or_expr OROR logic_and_expr { $$ = $1 || $3 ? 1 : 0; }
    ;

logic_and_expr:
	inclusive_or_expr
    |	logic_and_expr ANDAND inclusive_or_expr { $$ = $1 && $3 ? 1 : 0; }
    ;

inclusive_or_expr:
	exclusive_or_expr
    |	inclusive_or_expr '|' exclusive_or_expr { $$ = $1 | $3; }
    ;

exclusive_or_expr:
	and_expr
    |	exclusive_or_expr '^' and_expr { $$ = $1 ^ $3; }
    ;

and_expr:
	equality_expr
    |   and_expr '&' equality_expr { $$ = $1 & $3; }
    ;

equality_expr:
	relational_expr
    |	equality_expr EQUALSEQUALS relational_expr { $$ = $1 == $3 ? 1 : 0; }
    |	equality_expr NOTEQUALS relational_expr { $$ = $1 != $3 ? 1 : 0; }
    ;

relational_expr:
	shift_expr
    |	relational_expr '<' shift_expr { $$ = $1 < $3 ? 1 : 0; }
    |	relational_expr '>' shift_expr { $$ = $1 > $3 ? 1 : 0; }
    |	relational_expr LEFTEQUALS shift_expr { $$ = $1 << $3; }
    |	relational_expr RIGHTEQUALS shift_expr { $$ = $1 >> $3; }
    ;

shift_expr:
	additive_expr
    |	shift_expr LEFTLEFT additive_expr { $$ = $1 << $3; }
    |	shift_expr RIGHTRIGHT additive_expr { $$ = $1 >> $3; }
    ;

additive_expr:
	multiplicative_expr
    |	additive_expr '+' multiplicative_expr { $$ = $1 + $3; }
    |	additive_expr '-' multiplicative_expr { $$ = $1 - $3; }
    ;


multiplicative_expr:
	cast_expr
    |	multiplicative_expr '*' cast_expr { $$ = $1 * $3; }
    |	multiplicative_expr '/' cast_expr { $$ = $1 / $3; }
    |	multiplicative_expr '%' cast_expr { $$ = $1 % $3; }
    ;

cast_expr:
	unary_expr
    | 	'(' type ')' cast_expr { $$ = $4; }
    ;

unary_expr:
	postfix_expr
    |	PLUSPLUS unary_expr { $$ = $2++; }
    |	MINUSMINUS unary_expr { $$ = $2--; }
    |	unary_operator cast_expr
	{
	    switch($1) {
		case '&':
		case '*':
		    $$ = -1;
		    break;

		case TILDE:
		    $$ = ~ $2;
		    break;

		case '!':
		    $$ = ! $2;
		    break;

		case '+':
		    $$ = $2;
		    break;

		case '-':
		    $$ = - $2;
		    break;
		
		default:
		    printf("  cxxwrap internal error at \"%s\":%d, token %c\n",
			__FILE__, __LINE__, $1);
		    $$ = -1;
		    break;
	    }
	}
    |	SIZEOF unary_expr { $$ = -1; }
    |	SIZEOF '(' type ')' { $$ = -1; }
    ;

postfix_expr:
	primary_expr
    |	postfix_expr array_type_indirection { $$ = -1; }
    |	postfix_expr '(' opt_c_expr_list ')' { $$ = -1; }
    |	postfix_expr '.' id { $$ = -1; }
    |	postfix_expr ARROW id { $$ = -1; }
    |	postfix_expr PLUSPLUS { $$ = $1++; }
    |	postfix_expr MINUSMINUS { $$ = $1--; }
    ;

primary_expr:
	scoped_id
	{
	    /* handle NULL as "0", even though this is not
	       strictly true by ANSI rules, it is always the case
	       on "all" systems. */

	    if (0 == strcmp($1->val, "NULL"))
	    {
		$$ = 0;
	    }
	    else
	    {
		$$ = -1;
	    }
	}
    |	NEW scoped_id { $$ = -1; }
    |  	NUM { $$ = (int) $1->val; }
    |	STRING
	{
	    /* Map string constants to NULL, as well...
	       Not much useful we can do with them here. */

	    $$ = 0;
	}
    |	'(' constant_expr ')' { $$ = $2; }
    ;

c_expr_list:
	assignment_expr {}
    |	c_expr_list ',' assignment_expr {}
    ;

opt_c_expr_list:
	/* nil */
    |	c_expr_list
    ;

%%

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
