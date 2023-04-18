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
 * $Id: JNIEmitter.cxx,v 1.60 2006/12/17 04:08:11 deaven Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Class.h"
#include "EnumCType.h"
#include "VoidCType.h"
#include "JNICType.h"
#include "CommentParser.h"
#include "JNIEmitter.h"

JNIEmitter::JNIEmitter(ClassJar* jar_, const char* classpath_) :
    Emitter(jar_),
    classpath(classpath_),
    import_hook(NULL),
    begin_hook(NULL),
    end_hook(NULL),
    wrap_operators(0),
    emit_defaulted(0),
    emitted(NULL)
{
}

JNIEmitter::~JNIEmitter()
{
}

int JNIEmitter::emit(int argc, char** argv)
{
    FILE* fmake = NULL;
    const char* makefile = argval_in("--jni-makefile", argc - 1, argv + 1);
    if (makefile && strlen(makefile) > 0)
    {
	fmake = open_file(makefile, "w");
	if (!fmake)
	{
	    fprintf(stderr, "cannot open \"%s\"\n", makefile);
	    return -1;
	}
	fprintf(fmake, "JNIOBJ = \"\\\n");
    }

    StringPool* package_pool = NULL;
    const char* packages = argval_in("--jni-packages", argc - 1, argv + 1);
    if (packages)
    {
	package_pool = new StringPool();
	char* copy = strdup(packages);
	char* s = copy;
	while ((s = strtok(s, ",")))
	{
	    package_pool->add(s);
	    s = NULL; // for next strtok()
	}
	free(copy);
    }

    wrap_operators = arg_in("--jni-operators", argc - 1, argv + 1);
    
    emit_defaulted = arg_in("--jni-expand-defaults", argc - 1, argv + 1);

    const char* fn = argval_in("--jni-import-hook", argc - 1, argv + 1);
    if (fn)
    {
	import_hook = pool.add(read_file(fn));
    }

    fn = argval_in("--jni-begin-hook", argc - 1, argv + 1);
    if (fn)
    {
	begin_hook = pool.add(read_file(fn));
    }

    fn = argval_in("--jni-end-hook", argc - 1, argv + 1);
    if (fn)
    {
	end_hook = pool.add(read_file(fn));
    }

    int status = 0;
    int count = 0;

    Class* c;
    ClassJarIterator iter(jar->iterator());
    while ((c = iter.next()))
    {
	expand_if_needed(c);

	if (package_pool == NULL ||
	    package_pool->contains(c->dottedPackageName()))
	{
	    analyze_class(c);
	    int s = emit_class(c);
	    if (s == 0)
	    {
		count++;
		if (fmake)
		{
		    fprintf(fmake, "\t%s_jni.o \\\n", c->type()->JavaTypeName());
		}
	    }
	}
    }

    if (fmake)
    {
	fprintf(fmake, "\t\"\n");
	fclose(fmake);
	printf("wrote makefile \"%s\"\n", makefile);
    }

    printf("generated %d class definition%s.\n", count,
	count == 1 ? "" : "s");

    if (package_pool)
    {
	delete package_pool;
    }

    set_warning_context(NULL);

    return status;
}

const char* JNIEmitter::path_of_class(Class* c)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s", c->packageName());

    /* change all dots and slashes of any type into path separators
     */
    char* p;
    for (p = buf; *p != '\0'; p++)
    {
	if (*p == '.' ||
	    *p == '/' ||
	    *p == '\\')
	{
	    *p = PATHSEP;
	}
    }

    if (classpath && strlen(classpath) > 0)
    {
	char buf2[TOKEN_BUFFER_SIZE];
	sprintf(buf2, "%s%c%s", classpath, PATHSEP, buf);
	return pool.add(buf2);
    }

    return pool.add(buf);
}

static void append_underscores(const char* s, char* buf)
{
    int len = strlen(buf);
    buf += len;
    while (s && *s != '\0')
    {
	if(*s == '_') 
	{
	    *(buf++) = '_';
	    *(buf++) = '1';
	}
	else
	{
	    *(buf++) = *s;
	}
	s++;
    }
    *buf = '\0';
}

const char* JNIEmitter::jni_prefix_of_class(Class* c)
{
    char buf[TOKEN_BUFFER_SIZE];
    int len = strlen(c->packageName());

    /* Handle conversion of class name and package into the C++
     * name-mangled prefix used by JNI.
     *
     * One_Two --> One_1Two
     * One_Two_Three --> One_1Two_1Three
     * One_ --> One_1
     */
    sprintf(buf, "Java%s", len > 0 ? "_" : "");
    append_underscores(c->packageName(), buf);
    strcat(buf, "_");
    append_underscores(c->type()->JavaTypeName(), buf);

    char* p;
    for (p = buf; *p != '\0'; p++)
    {
	if (*p == '/' || *p == '\\' || *p == '.') *p = '_';
    }

    return pool.add(buf);
}

static const char* why_not_wrappable = "";
static int is_wrappable(CType* type)
{
    /* the "type" construct/destroy is always OK
     */
    if (type->modifiers & CType::M_CTOR) return 1;

    if (type->JNITypeName() == NULL)
    {
	why_not_wrappable = "no JNI representation";
	return 0;
    }
    if (type->JavaTypeName() == NULL)
    {
	why_not_wrappable = "no java representation";
	return 0;
    }
    if (type->indirection_count > 2 && strcmp(type->classname(),"ArrayCType")) 
    {
	why_not_wrappable = "too many levels of indirection";
	return 0;
    }
    return 1;
}

static const char* why_not_wrappable_cb = "";
static int is_wrappable_cb(CType* type)
{
    if (type->JNIFunctionTypeName() == NULL)
    {
        why_not_wrappable_cb = "no java representation";
	return 0;
    }
    return 1;
}

int JNIEmitter::emit_jni_begin(FILE* fcxx, Arg* a)
{
    while (a)
    {
	fprintf(fcxx, a->type->JNIBegin(a->name));
	a = a->next;
    }
    return 0;
}

int JNIEmitter::emit_jni_end(FILE* fcxx, Arg* a)
{
    while (a)
    {
	fprintf(fcxx, a->type->JNIEnd(a->name));
	a = a->next;
    }
    return 0;
}

int JNIEmitter::emit_cb_jni(FILE* fcxx, Arg* a)
{
    FunctionCType* fct = (FunctionCType*)a->type;
    fprintf(fcxx,
        "\n__cb_data* __cbd;\n"
        "if (%s != NULL) {\n"
        "  __cbd = (__cb_data*)malloc(sizeof(__cb_data));\n"
        "  __env->GetJavaVM(&__cbd->pJavaVM);\n"
        "  __cbd->jObject = __env->NewGlobalRef(%s);\n"
        "  __cbd->jMethID = __env->GetMethodID(__env->GetObjectClass(__cbd->jObject), \"%s\", \"(",
        a->name,
        a->name,
        fct->alias
    );

    for (a = fct->args; a; a = a->next)
    {
        /* The void pointer is not part of the java method signature
         */
        if (a->type->is_void()) continue;
	fprintf(fcxx, a->type->JavaTypeID());
    }
    
    fprintf(fcxx,
        ")%s\");\n",
        fct->rtype->JavaTypeID()
    );

    fprintf(fcxx,
        "} else {\n"
        "  __cbd = NULL;\n"
        "}\n\n"
    );

    return 0;
}

int JNIEmitter::emit_cb_begin(FILE* fcxx, Arg* a)
{
    while (a)
    {
	fprintf(fcxx, a->type->CBBegin(a->name));
	a = a->next;
    }
    return 0;
}

int JNIEmitter::emit_cb_end(FILE* fcxx, Arg* a)
{
    while (a)
    {
	fprintf(fcxx, a->type->CBEnd(a->name));
	a = a->next;
    }
    return 0;
}

int JNIEmitter::emit_enum(FILE* fjava, EnumCType* ect)
{
    /* emit the enumeration constant into thin air.
     */
    emit_comment(fjava, ect->comment);

    /* If there is a default value set in the C++ code, we just copy
     * that as a string (C++ and Java have the same operators). If
     * not, we assign last+1 (the first value will be '0' if there is
     * no default).
     */
    Arg *a, *la;
    int count = 0;
    for (a = ect->getSymbols(); a != NULL; a = a->next)
    {
	emit_comment(fjava, a->comment);
	if (a->default_value)
        {
            char* s = (char*) strstr(a->default_value, "::");
            if (s != NULL)
            {
                *s = '\0';
                Class* c = jar->findClassByName(a->default_value);
                if (c)
                {
                    char buf[TOKEN_BUFFER_SIZE];
                    const char* dpn = c->dottedPackageName();
                    const char* opt_dot = *dpn == '\0' ? "" : ".";
                    
                    sprintf(buf, "%s%s%s.%s", dpn, opt_dot, c->name(), s+2);
                    a->default_value = global.pool.add(buf);
                }
            }
            count = 1;
        }
        else
        {
            char buf[TOKEN_BUFFER_SIZE];
            if (count == 0)
            {
                sprintf(buf, "%d", count++);
            }
            else
            {
                sprintf(buf, "%s + 1", la->name);
            }
            a->default_value = global.pool.add(buf);
        }
	fprintf(fjava, "public final static int %s = %s;\n",
	    a->name, a->default_value);
        la = a;
    }

    return 0;
}

static int implicit(Arg* a)
{
    if (!a) return 0;
    const char* jn = a->type->JavaTypeName();
    if (!jn) return 0;
    if (*jn != '!') return 0;
    return 1;
}

static void subst_typedefs(Method* m, Class* c)
{
    /* typedefs within classes are registered as methods, and if it is
     * a FunctionCType its arguments are expanded here
     */
    if (m->type->modifiers & CType::M_TYPEDEF)
    {
        Class::subst_typedef_fct_args(m->type, c);
        return;
    }

    // method type
    m->type = Class::subst_typedef(m->type, c);

    // args
    Arg* a;
    for (a = m->args; a != NULL; a = a->next)
    {
        a->type = Class::subst_typedef(a->type, c);
    }
}

static Arg* last_default(Method* m, int remove)
{
    Arg* a;
    Arg* lasta = NULL;
    for (a = m->args; a != NULL; a = a->next)
    {
	/* Void types are excepted since they are not visible in Java
	 * anyway (only callback registration functions will match)
	 */
	if (a->type->is_void()) continue;

	if (a->next == NULL && a->default_value != NULL)
	{
	    // This is the last argument, and is has a default value
	    if (remove) {
		if (lasta) lasta->next = NULL;
		else       m->args = NULL;
	    }
	    return a;
	}

	lasta = a;
    }

    return NULL;
}

int JNIEmitter::emit_method(FILE* fjava, FILE* fcxx, Class* c, Method* m)
{
    // we can't represent typedefs in java
    if (m->type->modifiers & CType::M_TYPEDEF) return -1;

    // enums should not be wrapped as methods
    if (!strcmp(m->type->classname(),"EnumCType")) return -1;

    // operators are optionally wrapped
    if (m->is_operator && (!wrap_operators)) return -1;

    m->type = Class::subst_enum(m->type, c, jar);

    if (m->type->modifiers & CType::M_ATTRIBUTE)
    {
	// only if allowed
	if (!global.attributes) return -1;
        if (m->type->is_object()) m->type->modifiers |= CType::M_POINTER;
    }

    if (m->is_operator && !strcmp(m->name,"opCast") && m->type->OperatorSuffix() == NULL)
    {
        if (global.verbose)
        {
            printf("%s::%s() cast operator type \"%s\" cannot be wrapped\n",
                   c->name(), m->name, m->type->CTypeName() );
        }
        return -1;
    }

    if (!is_wrappable(m->type))
    {
        if (global.verbose)
        {
            printf("%s::%s() return type \"%s\" cannot be wrapped (%s)\n", 
                   c->name(), m->name, m->type->CTypeName(),
                   why_not_wrappable);
        }
        return -1;
    }
    
    /* extra filter to prevent "char **" as a return type (since we do
     * not have a way to figure out the length of the array cleanly).
     */
    const char* jtn = m->type->JavaTypeName();
    if (jtn && !strcmp(jtn, "String[]"))
    {
	if (global.verbose)
	{
	    printf("%s::%s() cannot be wrapped (no array length known for return type \"char**\")\n", 
		c->name(), m->name);
	}
	return -1;
    }

    /* We do not currently support returning of arrays, even if we know
     * the dimensions.
     */
    if (!strcmp(m->type->classname(),"ArrayCType"))
    {
	if (global.verbose)
	{
	    printf("%s::%s() cannot be wrapped (returning of arrays not supported).\n", 
		c->name(), m->name);
	}
	return -1;
    }

    Arg* a;
    int first;
    for (a = m->args; a != NULL; a = a->next)
    {
	a->type = Class::subst_enum(a->type, c, jar);
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void()) continue;
	if (!is_wrappable(a->type))
	{
	    if (global.verbose)
	    {
		printf("%s::%s() argument %s (type %s) cannot be wrapped (%s)\n", 
		    c->name(), m->name, a->name, a->type->CTypeName(),
		    why_not_wrappable);
	    }
	    return -1;
	}
    }

    char wbuf[256];
    sprintf(wbuf, "%s::%s", c->name(), m->name);
    set_warning_context(wbuf);

    /* Construct the java method declaration.  If it is the same as
     * one we have already emitted for this class, skip this one.
     * This handles the common case in C++ where multiple versions
     * of a method with pointers or references are supplied, usually
     * with inline converters.
     */
    {
	char buf[TOKEN_BUFFER_SIZE];
        sprintf(buf, "%s(", m->getName() );
	for (first = 1, a = m->args; a != NULL; a = a->next)
	{
	    if (!implicit(a))
	    {
		const char* jtna = a->type->JavaTypeName();
		sprintf(buf + strlen(buf),
		    "%s%s %s", first ? "" : ", ", jtna, a->name);
		first = 0;
	    }
	}

	if (emitted->contains(buf))
	{
	    if (global.verbose)
	    {
		printf("duplicate java version method %s::%s() not wrapped\n",
		    c->name(), m->name);
	    }
	    set_warning_context(NULL);
	    return -1;
	}

	emitted->add(buf);
    }

    int r;
    if (m->type->modifiers & CType::M_CTOR)
    {
	r = emit_ctor(fjava, fcxx, c, m);
    }
    else
    {
	r =  emit_noctor(fjava, fcxx, c, m);
    }
    set_warning_context(NULL);

    /* If the last argument has a default value, remove it and emit
     * definition of the resulting method.  This is because Java does
     * not allow setting default values to arguments, who can then be
     * omitted when calling the method.
     */
    if (emit_defaulted)
    {
	if (last_default(m, 0))
	{
	    m = m->clone();
	    a = last_default(m, 1);
	    delete a;
	    m->serial = c->next_serial();
	    emit_method(fjava, fcxx, c, m); // Recursive call
	}    
    }

    return r;
}

int JNIEmitter::emit_ctor(FILE* fjava, FILE* fcxx, Class* c, Method* m)
{
    Arg* a;
    int first;
    Class* parent = c->parentClass();

    /* Java method (simple pass-through to native method)
     */
    const char* add = c->isAbstract() ? 
        "\n\nThis constructor produces a object in which some methods may be extended "
        "in java.\n" : "";
    emit_comment(fjava, m->comment, add);
    fprintf(fjava, "public %s(",
	c->type()->JavaTypeName());
    for (first = 1, a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void()) continue;
	if (!implicit(a))
	{
	    fprintf(fjava, "%s%s %s", first ? "": ", ",
		a->type->JavaTypeName(), a->name);
	    first = 0;
	}
    }
    fprintf(fjava, ")\n{\n");
    if (parent) 
    {
        fprintf(fjava,"super((Long) null);\n");
    }
    if (begin_hook && !(m->notes & Method::NO_JNI_HOOK)) fputs(begin_hook, fjava);
    fprintf(fjava, "implementation = new Long(__c%d(", m->serial);
    for (first = 1, a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void()) continue;
	if (!implicit(a))
	{
            // Output extra jobject argument if reference to pointer
            if ((a->type->indirection_count == 2) 
                && (a->type->modifiers & CType::M_REFERENCE))
            {
                fprintf(fjava, "%s%s", first ? "" : ", ", a->name);
                first = 0;
            }

	    fprintf(fjava, "%s%s",
		first ? "" : ", ", a->type->JavaCast(a->name));
	    first = 0;
	}
    }
    fprintf(fjava, "));\n");
    if (c->isAbstract()) fprintf(fjava, "extensible = true;\n");
    if (end_hook && !(m->notes & Method::NO_JNI_HOOK)) fputs(end_hook, fjava);
    fprintf(fjava, "}\n");

    fprintf(fjava, "private native long __c%d(", m->serial);
    for (first = 1, a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void()) continue;
	if (!implicit(a))
	{
	    // Output extra jobject argument if reference to pointer
	    if ((a->type->indirection_count == 2)
		&& (a->type->modifiers & CType::M_REFERENCE))
		
            {
                fprintf(fjava, "%sObject __jobj_%s", first ? "" : ", ", a->name);
                first = 0;
            }

	    fprintf(fjava, "%s%s %s",
		first ? "" : ", ", a->type->PrivateJavaTypeName(), a->name);
	    first = 0;
	}
    }
    fprintf(fjava, ");\n");

    /* C++ method.
     */
    fprintf(fcxx, "JNIEXPORT jlong JNICALL %s__1_1c%d("
		  "JNIEnv* __env, jobject __jobj",
		  jni_prefix_of_class(c),
		  m->serial);
    for (a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void())
        {
            a->name = "__cbd";
            continue;
        }
	if (!implicit(a))
	{
	    // Output extra jobject argument if reference to pointer
	    if ((a->type->indirection_count == 2)
		&& (a->type->modifiers & CType::M_REFERENCE))
		fprintf(fcxx, ", jobject __jobj_%s", a->name);

	    fprintf(fcxx, ", %s %s",
		a->type->JNITypeName(), a->name);
	}
    }
    fprintf(fcxx, ")\n{\n");

    // If this method registers a callback, find the Arg with the FunctionCType, if any
    Arg* fct_arg = NULL;
    for (a = m->args; m->register_callback && !fct_arg && a; a = a->next)
    {
        if (!strcmp(a->type->classname(),"FunctionCType")) fct_arg = a;
    }

    emit_jni_begin(fcxx, m->args);
    if (fct_arg) emit_cb_jni(fcxx, fct_arg);
    if (c->isAbstract())
    {
	fprintf(fcxx, "%s* __obj = new cxxwrapAdapt_%s(__env, __jobj\n",
	    c->fullName(), c->name());
	for (a = m->args; a != NULL; a = a->next)
	{
	    if (!implicit(a))
	    {
		fprintf(fcxx, ", %s", a->type->JNICast(a->name));
	    }
	}
    }
    else
    {
	fprintf(fcxx, "%s* __obj = new %s(", c->fullName(), c->fullName());
	for (first = 1, a = m->args; a != NULL; a = a->next)
	{
	    if (!implicit(a))
	    {
		fprintf(fcxx, "%s%s",
		    first ? "": ", ", a->type->JNICast(a->name));
		first = 0;
	    }
	    else
	    {
		fprintf(fcxx, "%s__env",
		    first ? "": ", ");
		first = 0;
	    }
	}
    }
    fprintf(fcxx, ");\n");
    emit_jni_end(fcxx, m->args);
    fprintf(fcxx, "return (jlong) __obj;\n}\n\n");

    return 0;
}

int JNIEmitter::emit_noctor(FILE* fjava, FILE* fcxx, Class* c, Method* m)
{
    const char* method_name = m->name;

    /* We must re-write the method name of any operators, as these
     * contain special characters not allowed in a normal
     * function/method name.
     */
    if (m->is_operator)
    {
        if (!strcmp(m->name,"opCast"))
        {
            // Casting operator
            char buf[TOKEN_BUFFER_SIZE];
            sprintf(buf, "opCast%s", m->type->OperatorSuffix());
            method_name = global.pool.add(buf);
        }
        else
        {
            method_name = m->getTextName();
        }
    }

    /* Since all java objects derive from java.lang.Object, any
     * of this class's methods which coincide with the name of
     * a java.lang.Object name must be renamed.
     */
    static char* subs[] = {
	  "getClass"
	, "hashCode"
	, "equals"
	, "clone"
	, "toString"
	, "notify"
	, "notifyAll"
	, "wait"
	, "finalize"
	, NULL
    };

    char buf[TOKEN_BUFFER_SIZE];
    int i;
    for (i = 0; subs[i] != NULL; i++)
    {
	if (!strcmp(subs[i], method_name))
	{
	    sprintf(buf, "__%s", method_name);
	    method_name = buf;
	}
    }

    /* Java method (simple pass-through to native method).  In the case
     * where this object is extensible, bypass invoking native implementations,
     * which are "pure virtual", seen here as feedback loops which will just
     * turn around and invoke java again.
     */
    const char* add = (c->isAbstract() && do_adapter(m)) ?
        "\n\nThis method may be extended by Java subclasses.\n" :
        "\n\nThis method may <var>not</var> be extended in Java.\n";
    emit_comment(fjava, m->comment, add);
    const char* implval =
	m->type->modifiers & CType::M_STATIC ? "0" : "implementation.longValue()";
    fprintf(fjava, "public %s%s %s(",
	m->type->modifiers & CType::M_STATIC ? "static " : "",
	m->type->JavaTypeName(), method_name);
    Arg* a;
    int first;
    for (first = 1, a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void()) continue;
        if (!implicit(a))
	{
	    const char* jtna = a->type->JavaTypeName();
	    fprintf(fjava, "%s%s %s", first ? "": ", ", jtna, a->name);
	    first = 0;
	}
    }
    if (m->type->is_void())
    {
	fprintf(fjava, ")\n{\n%s",
	    m->is_pure ? "if (!extensible) {\n" : "");
	if (begin_hook && !(m->notes & Method::NO_JNI_HOOK)) fputs(begin_hook, fjava);
	fprintf(fjava, "__m%d(%s",
	    m->serial, implval);
	for (a = m->args; a != NULL; a = a->next)
	{
            /* We can ignore void pointers if this method registers a callback function
             */
            if (m->register_callback && a->type->is_void()) continue;
	    if (!implicit(a))
	    {
		// Output extra jobject argument if reference to pointer
		if ((a->type->indirection_count == 2) 
		    && (a->type->modifiers & CType::M_REFERENCE))
		    fprintf(fjava, ", %s", a->name);
		fprintf(fjava, ", %s", a->type->JavaCast(a->name));
	    }
	}
	fprintf(fjava, ");\n");
	if (end_hook && !(m->notes & Method::NO_JNI_HOOK)) fputs(end_hook, fjava);
	fprintf(fjava, "%s}\n",
	    m->is_pure ? "}\n" : "");
    }
    else
    {
	fprintf(fjava, ")\n{\n");
	fprintf(fjava, "%s __retval = %s;\n",
	    m->type->PrivateJavaTypeName(), m->type->DefaultPrivateJavaValue());
	if (m->is_pure)
	{
	    fprintf(fjava, "if (!extensible) {\n");
	}
	if (begin_hook && !(m->notes & Method::NO_JNI_HOOK)) fputs(begin_hook, fjava);
	fprintf(fjava, "__retval = __m%d(%s",
	    m->serial, implval);
	for (a = m->args; a != NULL; a = a->next)
	{
            /* We can ignore void pointers if this method registers a callback function
             */
            if (m->register_callback && a->type->is_void()) continue;
	    if (!implicit(a))
	    {
		// Output extra jobject argument if reference to pointer
		if ((a->type->indirection_count == 2) 
		    && (a->type->modifiers & CType::M_REFERENCE))
		    fprintf(fjava, ", %s", a->name);
		fprintf(fjava, ", %s", a->type->JavaCast(a->name));
	    }
	}
	fprintf(fjava, ");\n");
	if (end_hook && !(m->notes & Method::NO_JNI_HOOK)) fputs(end_hook, fjava);
	fprintf(fjava, "%sreturn %s;\n}\n",
	    m->is_pure ? "}\n" : "",
	    m->type->JavaUpCast("__retval"));
    }

    fprintf(fjava, "private native %s%s __m%d(long __imp",
	m->type->modifiers & CType::M_STATIC ? "static " : "",
	m->type->PrivateJavaTypeName(), m->serial);
    for (a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void()) continue;
	if (!implicit(a))
	{
	    const char* jtna = a->type->PrivateJavaTypeName();
	    // Output extra jobject argument if reference to pointer
	    if ((a->type->indirection_count == 2)
		&& (a->type->modifiers & CType::M_REFERENCE))
		fprintf(fjava, ", Object __jobj_%s", a->name);
	    fprintf(fjava, ", %s %s", jtna, a->name);
	}
    }
    fprintf(fjava, ");\n");

    /* C++ method.
     * Obtain the native implementation, cast it to the native object,
     * and invoke the proper native method.  All object args are
     * passed by reference, so must be converted from pointer to
     * pointer, object, or constant reference.
     */
    fprintf(fcxx, "JNIEXPORT %s JNICALL %s__1_1m%d("
		  "JNIEnv* __env, %s, jlong __imp",
		  m->type->JNITypeName(),
		  jni_prefix_of_class(c),
		  m->serial,
		  m->type->modifiers & CType::M_STATIC ? "jclass" : "jobject");

    for (a = m->args; a != NULL; a = a->next)
    {
        /* We can ignore void pointers if this method registers a callback function
         */
        if (m->register_callback && a->type->is_void())
        {
            //voidp_name = a->name;
            a->name = "__cbd";
            continue;
        }
	if (!implicit(a))
	{
	    const char* jtna = a->type->JNITypeName();
	    // Output extra jobject argument if reference to pointer
	    if ((a->type->indirection_count == 2)
		&& (a->type->modifiers & CType::M_REFERENCE))
		fprintf(fcxx, ", jobject __jobj_%s", a->name);
	    fprintf(fcxx, ", %s %s", jtna, a->name);
	}
    }
    fprintf(fcxx, ")\n{\n%s* __obj = (%s*) __imp;\n", c->fullName(), c->fullName());

    // If this method registers a callback, find the Arg with the FunctionCType, if any
    Arg* fct_arg = NULL;
    for (a = m->args; m->register_callback && !fct_arg && a; a = a->next)
    {
        if (!strcmp(a->type->classname(),"FunctionCType")) fct_arg = a;
    }

    /* Static operators are global (not class members) */
    char obj_deref[16];
    sprintf(obj_deref, "%s", (m->is_operator && m->type->modifiers & CType::M_STATIC) ? "" : "__obj->");

    /* If no default assignment operator exists, we need to create one */
    if (m->is_operator && !strcmp(method_name,"opEq") && !strcmp(m->args->name,"__defOpEq"))
    {
        fprintf(fcxx, "*__obj = *(%s*)%s;\n", c->name(), m->args->name);
        fprintf(fcxx, "}\n\n");
    }
    else if (m->type->is_void())
    {
	emit_jni_begin(fcxx, m->args);
        if (fct_arg) emit_cb_jni(fcxx, fct_arg);
	fprintf(fcxx, "%s%s(", obj_deref, m->getName());
	for (first = 1, a = m->args; a != NULL; a = a->next)
	{
	    if (!implicit(a))
	    {
		fprintf(fcxx, "%s%s",
		    first ? "": ", ", a->type->JNICast(a->name));
		first = 0;
	    }
	    else
	    {
		 fprintf(fcxx, "%s__env",
		    first ? "": ", ");
		first = 0;
	    }
	}
	fprintf(fcxx, ");\n");
	emit_jni_end(fcxx, m->args);
	fprintf(fcxx, "}\n\n");
    }
    else if (m->type->is_object() &&
	     (m->type->modifiers & (CType::M_POINTER | CType::M_REFERENCE)) == 0)

    {
	/* special case for methods returning objects, which would 
	 * be allocated on the stack and thus be non-transportable to
	 * Java.  We'll use new and assume a copy constructor exists.
	 *
	 * We have to make an explicit temporary variable to avoid a
	 * severe bug in Microsoft's Visual C++ 6.0 compiler.
	 */
	const char* const_pfx = m->type->modifiers & CType::M_CONST ?  "const " : "";
	emit_jni_begin(fcxx, m->args);
        if (fct_arg) emit_cb_jni(fcxx, fct_arg);
	fprintf(fcxx, "%s __retval_temp(%s%s(",
	    m->type->BareCTypeName(), obj_deref, m->getName());
	for (first = 1, a = m->args; a != NULL; a = a->next)
	{
	    if (!implicit(a))
	    {
		fprintf(fcxx, "%s%s",
		    first ? "": ", ", a->type->JNICast(a->name));
		first = 0;
	    }
	    else
	    {
		fprintf(fcxx, "%s__env",
		    first ? "": ", ");
		first = 0;
	    }
	}
	fprintf(fcxx, "));\n%s%s%s* __retval = new %s(__retval_temp);\n",
	    const_pfx, m->type->BareCTypeName(), m->type->CTypeNameSuffix(),
	    m->type->BareCTypeName());
	emit_jni_end(fcxx, m->args);
	fprintf(fcxx, "return (jlong) __retval;\n}\n\n");
    }
    else
    {
        if (m->type->modifiers & CType::M_ATTRIBUTE)
        {
            char v[TOKEN_BUFFER_SIZE];
            sprintf(v, "%s%s%s", (m->type->is_object() && m->type->indirection_count == 0?"&":""), obj_deref, m->getName());
            fprintf(fcxx, "return %s;\n}\n\n", m->type->JNIUpCast(v));
        }
        else
        {
            const char* const_pfx = m->type->modifiers & CType::M_CONST ?  "const " : "";
            emit_jni_begin(fcxx, m->args);
            if (fct_arg) emit_cb_jni(fcxx, fct_arg);
            fprintf(fcxx, "%s%s%s __retval = %s%s(",
                    const_pfx,
                    m->type->BareCTypeName(), m->type->CTypeNameSuffix(), obj_deref, m->getName());
            for (first = 1, a = m->args; a != NULL; a = a->next)
            {
                if (!implicit(a))
                {
                    fprintf(fcxx, "%s%s",
                            first ? "": ", ", a->type->JNICast(a->name));
                    first = 0;
                }
                else
                {
                    fprintf(fcxx, "%s__env", 
                            first ? "": ", ");
                    first = 0;
                }
            }
            fprintf(fcxx, ");\n");
            emit_jni_end(fcxx, m->args);
            fprintf(fcxx, "return %s;\n}\n\n", m->type->JNIUpCast("__retval"));
        }
    }

    return 0;
}

CType* JNIEmitter::subst_jni(CType* t)
{
    if (t->is_object())
    {
	ObjectCType* oct = (ObjectCType*) t;
	const char* jn = oct->objname;
	if (jn)
	{
	    if (0 == strcmp("JNIEnv", jn) &&
		1 == oct->indirection_count)
	    {
		JNICType* j = new JNICType(oct, JNICType::T_ENV);
		return j;
	    }

	    if (0 == strcmp("jobject", jn) &&
		0 == oct->indirection_count)
	    {
		JNICType* j = new JNICType(oct, JNICType::T_OBJECT);
		return j;
	    }
	}
    }

    return t;
}

int JNIEmitter::expand_if_needed(Class* c)
{
    Method* m;
    Arg* a;

    // enumerate all class references
    // substitute JNI types where found in interface arguments
    for (m = c->methods; m; m = m->next)
    {
	jar->findClassByType(m->type, c); // expand templates
	for (a = m->args; a; a = a->next)
	{
	    jar->findClassByType(a->type, c);
	    a->type = subst_jni(a->type);
	}
    }

    return 0;
}

static void add_headers(Class* c, StringPool* headers)
{
    headers->add(c->type()->source_file);

    /* If this is a template expansion, make sure we include
     * the template definition.
     */
    Class* tc = global.classJar.findClassByName(c->name());
    if (tc) headers->add(tc->type()->source_file);
}

int JNIEmitter::emit_required_includes(FILE* fcxx, Class* c)
{
    StringPool headers;
    Method* m;
    Arg* a;

    add_headers(c, &headers);

    /* Include headers for all methods return types, and 
     * method arguments.
     */
    for (m = c->methods; m; m = m->next)
    {
	Class *rc = jar->findClassByType(m->type, c);
	if (rc) add_headers(rc, &headers);
	for (a = m->args; a; a = a->next)
	{
	    Class *rc = jar->findClassByType(a->type, c);
	    if (rc) add_headers(rc, &headers);
	}
    }

    string* all = headers.elements();
    int i;
    for (i = 0; all[i]; i++)
    {
	fprintf(fcxx, "#include \"%s\"\n", all[i]);
    }
    delete[] all;

    return 0;
}

int JNIEmitter::emit_required_imports(FILE* fjava, Class* c)
{
    StringPool headers;
    Method* m;
    Arg* a;

    // the parent class is required
    Class *rc = c->parentClass();
    if (rc) headers.add(rc->dottedPackageName());

    // enumerate all class references
    for (m = c->methods; m; m = m->next)
    {
	rc = jar->findClassByType(m->type, c);
	if (rc) headers.add(rc->dottedPackageName());
	for (a = m->args; a; a = a->next)
	{
	    rc = jar->findClassByType(a->type, c);
	    if (rc) headers.add(rc->dottedPackageName());
	}
    }

    string* all = headers.elements();
    int i;
    for (i = 0; all[i]; i++)
    {
	if (strlen(all[i]) > 0 && strcmp(all[i], c->dottedPackageName()))
	{
	    fprintf(fjava, "import %s.*;\n", all[i]);
	}
    }
    delete[] all;

    return 0;
}

void JNIEmitter::analyze_class(Class* c) 
{
    Method* m;
    for (m = c->methods; m; m = m->next)
    {
        if (m->comment != NULL) 
	{
	    if (strstr(m->comment,"<! NO_JNI_HOOK !>"))
	    {
	        printf(" JNI begin/end hooks suppressed in %s::%s\n", c->name(), m->name);
		m->notes |= Method::NO_JNI_HOOK;
	    }
	}
    }
    
}

int JNIEmitter::do_adapter(Method* m)
{
    if (!(m->is_pure)) return 0;
    if (m->type->modifiers & CType::M_PRIVATE) return 0;
    if (NULL == m->type->JavaTypeName()) return 0;

    // only pointers as object return values, please
    if (m->type->is_object() && (m->type->indirection_count != 1)) return 0;

    // Limitation for now (see conversion code XXX below)
    if (m->type->is_object()) return 0;

    // check JNI method type specification (must exist)
    const char* s = jni_type_spec(m);
    if (!s) return 0;

    // exclude arrays for now
    if (strstr(s, "[")) return 0;

    return 1;
}

int JNIEmitter::emit_adapter_def(FILE* fcxx, Class* c)
{
    Method* m;
    Arg* a;
    int first;

    /* In JDK 1.2, the API definition changed.  This is needed
     * to compile properly, but should be run-time compatible with
     * other JVMs from the same vendor.
     */
    fprintf(fcxx,
	"#if defined(JDK1_2) || defined(JDK1_3)\n"
	"#define CXXWRAP_ATTACH JNIEnv* env; cxxwrap_jvm->AttachCurrentThread((void**)&env, (void*) 0)\n"
	"#else\n"
	"#define CXXWRAP_ATTACH JNIEnv* env; cxxwrap_jvm->AttachCurrentThread(&env, (void*) 0)\n"
	"#endif\n");

    fprintf(fcxx, 
	"class cxxwrapAdapt_%s : public %s {\n"
	"private:\n"
	"JavaVM* cxxwrap_jvm;\n"
	"jclass  cxxwrap_class;\n"
	"jobject cxxwrap_obj;\n"
	"public:\n"
	"~cxxwrapAdapt_%s() {\n"
	"CXXWRAP_ATTACH;\n"
	"env->DeleteGlobalRef((jobject) cxxwrap_class);\n"
	"env->DeleteGlobalRef(cxxwrap_obj);\n"
	"}\n",
	c->name(), c->fullName(), c->name());

    int num_ctors = 0;
    Method* mlist = c->completeMethodList();
    for (m = mlist; m; m = m->next)
    {
	if ((m->type->modifiers & CType::M_CTOR) &&
	    !(m->type->modifiers & CType::M_PRIVATE))
	{
	    num_ctors++;

	    fprintf(fcxx,
		"cxxwrapAdapt_%s(JNIEnv* __env, jobject __jobj", c->name());
	    for (a = m->args; a != NULL; a = a->next)
	    {
		if (!implicit(a))
		{
		    fprintf(fcxx, ", %s %s",
			a->type->CTypeName(), a->name);
		}
	    }
	    fprintf(fcxx, ") :\n%s(", c->fullName());
	    for (first = 1, a = m->args; a != NULL; a = a->next)
	    {
		if (!implicit(a))
		{
		    fprintf(fcxx, "%s%s", first ? "": ", ", a->name);
		    first = 0;
		}
		else
		{
		    fprintf(fcxx, "%s__env", first ? "": ", ");
		    first = 0;
		}
	    }
	    fprintf(fcxx,
		") {\n"
		"__env->GetJavaVM(&cxxwrap_jvm);\n"
		"cxxwrap_obj = __env->NewGlobalRef(__jobj);\n"
		"cxxwrap_class = (jclass) __env->NewGlobalRef(__env->GetObjectClass(cxxwrap_obj));\n"
		"}\n");
	}
    }

    /* emit the default ctor if needed.
     */
    if (num_ctors == 0)
    {
	fprintf(fcxx,
	    "cxxwrapAdapt_%s(JNIEnv* env, jobject obj) :\n"
	    "    %s() {\n"
	    "    env->GetJavaVM(&cxxwrap_jvm);\n"
	    "    cxxwrap_obj = env->NewGlobalRef(obj);\n"
	    "    cxxwrap_class = (jclass) env->NewGlobalRef(env->GetObjectClass(cxxwrap_obj));\n"
	    "    }\n"
	    "public:\n",
	    c->name(), c->fullName());
    }

    for (m = mlist; m; m = m->next)
    {
	if (do_adapter(m))
	{
	    if (!strcmp("void", m->type->JavaTypeName()))
	    {
		fprintf(fcxx, "void %s(", m->name);
		for (first = 1, a = m->args; a != NULL; a = a->next)
		{
		    if (!implicit(a))
		    {
			fprintf(fcxx, "%s%s %s",
			    first ? "": ", ", a->type->CTypeName(), a->name);
			first = 0;
		    }
		    else
		    {
			fprintf(fcxx, "%sJNIEnv* __env", first ? "": ", ");
			first = 0;
		    }
		}
		fprintf(fcxx, ")%s {\n"
		    "CXXWRAP_ATTACH;\n", m->is_const ? " const" : "");
		for (a = m->args; a != NULL; a = a->next)
		{
		    fprintf(fcxx, "%s", a->type->JNIInvokeBegin(a->name));
		}
		fprintf(fcxx, 
		    "jmethodID mid = env->GetMethodID(cxxwrap_class,\n"
		    "    \"%s\", \"%s\");\n"
		    "if (mid) env->CallVoidMethod(cxxwrap_obj, mid",
		    m->name, jni_type_spec(m));
		for (a = m->args; a != NULL; a = a->next)
		{
		    fprintf(fcxx, ", %s", a->type->JNIInvokeUpCast(a->name));
		}
		fprintf(fcxx, ");\n");
	    }
	    else
	    {
		char jni_function[128];
		char return_type[128];
		if (m->type->is_object())
		{
		    strcpy(jni_function, "Object");
		    strcpy(return_type, "jobject");
		}
		else
		{
		    strcpy(jni_function, m->type->JavaTypeName());
		    jni_function[0] = toupper(jni_function[0]);
		    sprintf(return_type, "%s%s", m->type->BareCTypeName(),
			m->type->CTypeNameSuffix());
		}

		fprintf(fcxx, "%s %s(", m->type->CTypeName(), m->name);
		for (first = 1, a = m->args; a != NULL; a = a->next)
		{
		    if (!implicit(a))
		    {
			fprintf(fcxx, "%s%s %s",
			    first ? "": ", ", a->type->CTypeName(), a->name);
			first = 0;
		    }
		    else
		    {
			fprintf(fcxx, "%sJNIEnv* __env",
			     first ? "": ", ");
			first = 0;
		    }
		}
		fprintf(fcxx, ")%s {\n"
		    "CXXWRAP_ATTACH;\n", m->is_const ? " const" : "");
		for (a = m->args; a != NULL; a = a->next)
		{
		    fprintf(fcxx, "%s", a->type->JNIInvokeBegin(a->name));
		}
		fprintf(fcxx,
		    "jmethodID mid = env->GetMethodID(cxxwrap_class,\n"
		    "    \"%s\", \"%s\");\n"
		    "%s retval = mid ? (%s) env->Call%sMethod(cxxwrap_obj, mid",
		    m->name, jni_type_spec(m),
		    return_type, return_type, jni_function);
		for (a = m->args; a != NULL; a = a->next)
		{
		    fprintf(fcxx, ", %s", a->type->JNIInvokeUpCast(a->name));
		}
		fprintf(fcxx, ") : (%s) 0;\n", return_type);

		if (m->type->is_object())
		{
		    // XXX NEED CODE TO PULL OUT C++ IMPL
		    warning("JNI code to return object in virtual "
		            "adapter is not implemented");
		}
		else
		{
		    fprintf(fcxx, "return retval;\n");
		}
	    }

	    fprintf(fcxx, "}\n");

	    if (global.verbose)
	    {
		printf("generating C++ to Java adapter for %s::%s()\n",
		    c->fullName(), m->name);
	    }
	}
	else if (m->is_pure)
	{
	    /* we still have to implement something, or else the
	     * C++ definition is whacked.
	     */
	    fprintf(fcxx, "%s %s(", m->type->CTypeName(), m->name);
	    for (first = 1, a = m->args; a != NULL; a = a->next)
	    {
		if (!implicit(a))
		{
		    fprintf(fcxx, "%s%s %s",
			first ? "": ", ", a->type->CTypeName(), a->name);
		    first = 0;
		}
		else
		{
		    fprintf(fcxx, "%sJNIEnv* __env",
			first ? "": ", ");
		    first = 0;
		}
	    }
	    fprintf(fcxx, ")%s {\n",
		m->is_const ? " const" : "");
	    if ((!m->type->is_void()) || (m->type->indirection_count > 0))
	    {
		/* it's difficult to come up with a "default value"
		 * for any old data type.  For objects, we'll assume
		 * a default constructor exists.
		 */
		if (m->type->is_object() && (m->type->indirection_count == 0))
		{
		    fprintf(fcxx, "return %s();\n",
			m->type->BareCTypeName());
		}
		else
		{
		    fprintf(fcxx, "return (%s%s) 0;\n",
			m->type->BareCTypeName(),
			m->type->CTypeNameSuffix());
		}
	    }
	    fprintf(fcxx, "}\n");
	}
    }
    delete mlist;

    fprintf(fcxx, "};\n");
    return 0;
}

int JNIEmitter::emit_class(Class* c)
{
    // silently ignore template definitions
    if (c->parameters()) return -1;

    // and cached classes
    if (c->cached()) return -1;

    if (!is_wrappable(c->type()))
    {
	if (global.verbose)
	{
	    printf("class %s cannot be wrapped (%s)\n", 
		c->name(), why_not_wrappable);
	}
	return -1;
    }

    const char* basename = c->type()->JavaTypeName();
    char buf[TOKEN_BUFFER_SIZE];
    
    /* ensure that the path for JNI .cxx and .java files
     * exists, and open these files.
     */
    const char* path = path_of_class(c);
    if (strlen(path) > 0)
    {
	make_path(path);
    }

    /* Substitute typedef expansions for this class. This must be
     * complete before we can analyze the methods' arguments.  This
     * analysis may result in emitting some extra code before emitting
     * the class itself (like callbacks).
     */
    Method* m;
    for (m = c->methods; m; m = m->next)
    {
	subst_typedefs(m, c);
    }

#if 0
    /* If this class does not have an assignment operator defined, we
     * need to generate a Java method using the C++ default assignment
     * operator.
     */
    if (wrap_operators)
    {
	int assignment_operator = 0;
	Method* prevm = NULL;
	for (m = c->methods; m && !assignment_operator; m = m->next)
	{
	    if (m->is_operator && !strcmp(m->name,"op=")) assignment_operator = 1;
	    prevm = m;
	}
	if (!assignment_operator)
	{
	    if (global.verbose)
	    {
		printf("generating default assignment operator for class \"%s\"\n",
		       c->type()->JavaTypeName());
	    }
	    CType* ct = new ObjectCType("", c->name(), NULL);
	    Arg* a = new Arg("", "__defOpEq", ct);
	    m = new Method("", "op=", a);
	    m->type = new VoidCType("");
	    m->is_operator = 1;
	    m->serial = c->next_serial();
	    m->next = prevm ? prevm->next : NULL;
	    if (prevm) prevm->next = m;
	}
    }
#endif
    
    /* Check if there is any callback functions in any of the argument
     * lists.  We only generate a callback interface for functions
     * which take one void* and one function pointer (to a function
     * also taking one void*). All other arguments to the function
     * registering the callback must be wrappable, and arguments to
     * the callback function must be wrappable back to java.
     */
    int generate_callbacks = 0;
    if (global.callbacks)
    {
        for (m = c->methods; m; m = m->next)
        {
            // Set the warning context
            char wbuf[256];
            sprintf(wbuf, "%s::%s", c->name(), m->name);
            set_warning_context(wbuf);

            // Check if we can wrap the return type.
            if (!is_wrappable(m->type)) continue;

            int voidp_cnt = 0, function_cnt = 0;
            Arg* fa;            
            for (Arg* a = m->args; a; a = a->next)
            {
                if (a->type->is_void() && a->type->indirection_count == 1) voidp_cnt++;
                else if (!strcmp(a->type->classname(),"FunctionCType") && a->type->indirection_count == 1)
                {
                    fa = a;
                    function_cnt++;
                }
                else
                {
                    // Check if we can wrap this type
                    if (!is_wrappable(a->type))
                    {
                        function_cnt = 0;
                        break;
                    }
                }
            }
            
            if (voidp_cnt == 1 && function_cnt == 1)
            {
                /* We can generate a callback if all arguments to the
                 * callback function are wrappable back to java
                 */
                FunctionCType* fct = (FunctionCType*) fa->type;;

                // Check the return value from the callback function
                if (!is_wrappable(fct->rtype))
                {
                    sprintf(buf, "Cannot create callback, return type %s cannot be wrapped (%s)",
                            fct->rtype->CTypeName(), why_not_wrappable);
                    warning(buf);
                    continue;
                }

                // Check arguments
                int nowrap = 0;
                int voidp_cnt = 0;
                for (Arg* a = fct->args; a; a = a->next)
                {
                    if (a->type->is_void() && a->type->indirection_count == 1)
                    {
                        voidp_cnt++;
                        continue;
                    }
                            
                    if (!is_wrappable_cb(a->type))
                    {
                        sprintf(buf, "Cannot create callback, type %s cannot be wrapped (%s)\n",
                                a->type->CTypeName(), why_not_wrappable_cb);
                        warning(buf);
                        nowrap = 1;
                        break;
                    }
                }
                if (nowrap) continue;
                if (voidp_cnt != 1)
                {
                    warning("Cannot create callback, the callback function must take exactly one void*");
                    continue;
                }

                // OK, generate callback interface
                generate_callbacks = 1;
                m->register_callback = 1;
                fct->can_wrap = 1;

                if (fct->alias == NULL) fct->alias = fa->name;
                c->callbacks.put(fct->alias, fct);

                // Write the java callback interface
                const char* cb_name = fct->alias;
                sprintf(buf, "%s%s%sCallback.java",
                        path, strlen(path) > 0 ? PATHSEPSTR : "", cb_name);
                FILE* fjava_cb = open_file(buf, "w");
                if (!fjava_cb)
                {
                    fprintf(stderr, "cannot open \"%s\"\n", buf);
                    return -1;
                }
                if (global.verbose) printf("Writing %sCallback.java\n", cb_name);
                emit_callback_interface(fjava_cb, c, cb_name, fct);
                fclose(fjava_cb);
            }
            else if (function_cnt > 0)
            {
                // This can not be wrapped, print warning
                if (function_cnt > 1)
                {
                    warning("This function registers more than one callback, not supported");
                }
                else
                {
                    warning("The register callback function must take exactly one void*");
                }
            }
        }
        set_warning_context(NULL);
    }
    
    sprintf(buf, "%s%s%s.java",
	path, strlen(path) > 0 ? PATHSEPSTR : "", basename);
    FILE* fjava = open_file(buf, "w");
    if (!fjava)
    {
	fprintf(stderr, "cannot open \"%s\"\n", buf);
	return -1;
    }

    sprintf(buf, "%s%s%s_jni.cxx",
	path, strlen(path) > 0 ? PATHSEPSTR : "", basename);
    FILE* fcxx = open_file(buf, "w");
    if (!fcxx)
    {
	fprintf(stderr, "cannot open \"%s\"\n", buf);
	return -1;
    }

    if (global.verbose)
    {
	printf("writing %s%s%s{.java,_jni.cxx}\n", 
	     path, strlen(path) > 0 ? PATHSEPSTR : "", basename);
    }

    /* Java file header.
     */
    fprintf(fjava, "// created by cxxwrap -- DO NOT EDIT\n");

    if (strlen(c->dottedPackageName()) > 0)
    {
	fprintf(fjava, "package %s;\n", c->dottedPackageName());
    }

    emit_required_imports(fjava, c);

    if (import_hook)
    {
	fputs(import_hook, fjava);
    }

    Class* parent = c->parentClass();
    emit_comment(fjava, c->comment);
    const char* sp = parent ? parent->type()->JavaTypeName() : "Object";
    if (!sp)
    {
	printf("internal error %s\n", parent->type()->CTypeName());
	sp = "Object";
    }
    fprintf(fjava, "public class %s extends %s {\n",
	c->type()->JavaTypeName(), sp);

    /* C++ file header.
     */
    fprintf(fcxx,
	"// created by cxxwrap -- DO NOT EDIT\n"
	"#include \"jni.h\"\n");

    emit_required_includes(fcxx, c);

    /* Emit code to support callback functions
     */
    if (generate_callbacks)
    {
        // stdlib.h is required for malloc(), string.h for strdup()
        fprintf(fcxx,
            "#include <stdlib.h>\n"
            "#include <string.h>\n"
            "typedef struct __cb_data\n"
            "{\n"
            "  JavaVM* pJavaVM;\n"
            "  jobject jObject;\n"
            "  jmethodID jMethID;\n"
            "};\n");
        for (int i=c->callbacks.size()-1; i >= 0; i--)
        {
            /* Emit a callback function with the correct function signature
             */
            FunctionCType* fct = (FunctionCType*)c->callbacks.get(i);

            fprintf(fcxx, "\n");
            fprintf(fcxx, "%s __%s__callback(",
                fct->rtype->CTypeName(), fct->alias);
            
            Arg* a;
            int first;
            const char* vp_name;
            for (first = 1, a = fct->args; a; a = a->next)
            {
                if (a->type->is_void() && a->type->indirection_count == 1) vp_name = a->name;
                if (!implicit(a))
                {
                    const char* ctna = a->type->CTypeName();
                    fprintf(fcxx, "%s%s %s", first ? "": ", ", ctna, a->name);
                    first = 0;
                }
            }
            fprintf(fcxx, ")");

            /* If we have emitted the body of this function before,
             * only a prototype for the function should be emitted
             */
            if (callbacks.get(fct->alias) != NULL)
            {
                fprintf(fcxx, ";\n");
                continue;
            }
            callbacks.put(fct->alias, fct);

            fprintf(fcxx, "\n{\n");
            fprintf(fcxx,
                "  __cb_data* __cb_data_ptr = (__cb_data*)%s;\n"
                "  JNIEnv* __env;\n",
                vp_name);

            /* This should really only be needed if the native code
             * uses multiple threads.
             *
             * In JDK 1.2, the API definition changed.  This is needed
             * to compile properly, but should be run-time compatible with
             * other JVMs from the same vendor.
             */
            fprintf(fcxx,
                "#if defined(JDK1_2) || defined(JDK1_3)\n"
                "  __cb_data_ptr->pJavaVM->AttachCurrentThread((void**)&__env, (void*) 0);\n"
                "#else\n"
                "  __cb_data_ptr->pJavaVM->AttachCurrentThread(&__env, (void*) 0);\n"
                "#endif\n");

            // Emit code for converting from native to Java types
            emit_cb_begin(fcxx, fct->args);

            if (!fct->rtype->is_void())
            {
                fprintf(fcxx, "  %s __retval = (%s) ",
                    fct->rtype->JavaJNITypeName(), fct->rtype->JavaJNITypeName());
            }
            
            fprintf(fcxx,    
                "  __env->Call%sMethod(__cb_data_ptr->jObject, __cb_data_ptr->jMethID",
                fct->rtype->JNIFunctionTypeName()
            );

            // Print the argument list for the java method call
            for (a = fct->args; a; a = a->next)
            {
                // Skip the void pointer
                if (a->type->is_void() && a->type->indirection_count == 1) continue;
                if (!implicit(a))
                {
                    fprintf(fcxx, ", %s", a->type->CBCast(a->name));
                }
            }
            fprintf(fcxx, ");\n");

            if (!fct->rtype->is_void())
            {
                fprintf(fcxx, "%s", fct->rtype->CBUpCast("__retval"));
            }

            /* Emit code for freeing any resources allocated before
             * the Java method call
             */
            emit_cb_end(fcxx, fct->args);

            fprintf(fcxx, "}\n");
        }
    }

    /* Emit adapter class to handle all pure virtual functions.
     */
    if (c->isAbstract())
    {
	emit_adapter_def(fcxx, c);
    }

    fprintf(fcxx,
	"#ifdef __cplusplus\n"
	"extern \"C\" {\n"
	"#endif\n");

    /* Process methods of this class, emitting code as we go.
     */
    const char* dtor_comment = NULL;
    int num_ctors = 0;
    int dtor_protected = 0;
    emitted = new StringPool();

    long privpro = CType::M_PROTECTED | CType::M_PRIVATE;

    for (m = c->methods; m; m = m->next)
    {
	if (m->type->modifiers & CType::M_CTOR)
	{
	    num_ctors++;
	}
	else if (m->type->modifiers & CType::M_DTOR)
	{
	    dtor_comment = m->comment;

	    if (m->type->modifiers & privpro)
	    {
		dtor_protected = 1;
	    }

	    continue; // skip destructors here
	}

	if (m->notes & Method::ENUM_DEFINE)
	{
	    EnumCType* ect = (EnumCType*) m->type; // XXX
	    emit_enum(fjava, ect);
	}
	else if(!(m->type->modifiers & privpro))
	{
	    emit_method(fjava, fcxx, c, m);
	}
    }

    /* If there were no constructors, supply a "default" constructor,
     * which uses the one generated by the C++ compiler.
     */
    if (num_ctors == 0)
    {
	if (global.verbose)
	{
	    printf("generating default constructor for class \"%s\"\n",
		c->type()->JavaTypeName());
	}
	const char* super_part = 
	    c->parentClass() ? "super((Long) null); " : "";
	fprintf(fjava, 
	    "public %s() {\n"
	    "%s%s\n"
	    "implementation = new Long(__cdefault());\n"
	    "%s\n}\n"
	    "private native long __cdefault();\n",
	    c->type()->JavaTypeName(),
	    super_part,
	    begin_hook ? begin_hook : "",
	    end_hook ? end_hook : "");

	if (c->isAbstract())
	{
	fprintf(fcxx,
	    "JNIEXPORT jlong JNICALL %s__1_1cdefault(JNIEnv* __env, jobject __jobj)\n"
	    "{\n"
	    "%s* __obj = new cxxwrapAdapt_%s(__env, __jobj);\n"
	    "return (jlong) __obj;\n"
	    "}\n\n",
	    jni_prefix_of_class(c),
	    c->fullName(), c->name());
	}
	else
	{
	fprintf(fcxx,
	    "JNIEXPORT jlong JNICALL %s__1_1cdefault(JNIEnv* __env, jobject)\n"
	    "{\n"
	    "%s* __obj = new %s();\n"
	    "return (jlong) __obj;\n"
	    "}\n\n",
	    jni_prefix_of_class(c),
	    c->fullName(), c->fullName());
	}
    }

    /* All generated classes have a "pointer" constructor,
     * for internal use by cxxwrap code.
     */

    /* The root class always gets a destructor and an
     * implementation.  Derived clases can have a destructor if there 
     * is a comment in the C++ code to capture.
     */
    if (c->parentClass())
    {
	fprintf(fjava,
	    "// cxxwrap ctor, do not use\n"
	    "public %s(Long __imp) { super(__imp); }\n",
	    c->type()->JavaTypeName());

	if (dtor_comment && !dtor_protected)
	{
	    emit_comment(fjava, dtor_comment);
	    fprintf(fjava, 
		"public void delete() { super.delete(); }\n");
	}
    }
    else
    {
	fprintf(fjava,
	    "// cxxwrap ctor, do not use\n"
	    "public %s(Long __imp) { implementation = __imp; }\n"
	    "protected Long implementation = null;\n"
	    "protected boolean extensible = false;\n"
	    "public Long getCxxwrapImpl() { return implementation; }\n",
	    c->type()->JavaTypeName());

        /* Override the equals() method from Object.  We only compare
         * the value of the implementation field; i.e. equals() will
         * return true if the two objects point to the same object on
         * the native side. This is of course not ideal, as we really
         * should return true only if the two native objects can be
         * considered "equal".
         */
	fprintf(fjava,
	    "// override equals() from Object, compare the implementation value\n"
	    "public boolean equals(Object o) {\n"
            "  if (!(o instanceof %s)) return false;\n"
            "  return implementation.equals(((%s)o).implementation);\n"
            "}\n",
            c->type()->JavaTypeName(),
            c->type()->JavaTypeName());

        /* Override the hashCode() method from Object.  When
         * overriding equals(), hashCode() should also be overridden,
         * so as to maintain the general contract for the hashCode
         * method, which states that equal objects must have equal
         * hash codes.
         */
	fprintf(fjava,
	    "// override hashCode() from Object, return the implementation values hashCode()\n"
            "public int hashCode() { return implementation.hashCode(); }\n");

        
	if (!dtor_protected)
	{
	    emit_comment(fjava, dtor_comment);
	    fprintf(fjava,
		"public void delete()\n{\n%s\n__d(implementation.longValue());\n%s implementation = null;\n}\n"
		"private native void __d(long __imp);\n",
		(begin_hook) ? begin_hook : "",
		(end_hook) ? end_hook : "");

	    fprintf(fcxx,
		"JNIEXPORT void JNICALL %s__1_1d(JNIEnv* __env, jobject, jlong __imp)\n"
		"{\n"
		"%s* __obj = (%s*) __imp;\n"
		"delete __obj;\n"
		"}\n\n",
		jni_prefix_of_class(c),
		c->fullName(), c->fullName());
	}

    }

    delete emitted;
    emitted = NULL;

    // close java class definition
    fprintf(fjava, "};\n");
    fclose(fjava);

    // close C++ scope rules
    fprintf(fcxx,
	"#ifdef __cplusplus\n"
	"}\n"
	"#endif\n");
    fclose(fcxx);

    return 0;
}

int
JNIEmitter::emit_callback_interface(FILE* fjava_cb, Class* c, const char* cb_name, FunctionCType* fct)
{
    /* Java file header.
     */
    fprintf(fjava_cb, "// created by cxxwrap -- DO NOT EDIT\n");

    if (strlen(c->dottedPackageName()) > 0)
    {
	fprintf(fjava_cb, "package %s;\n", c->dottedPackageName());
    }

    emit_required_imports(fjava_cb, c);

    if (import_hook)
    {
	fputs(import_hook, fjava_cb);
    }

    fprintf(fjava_cb, "public interface %sCallback\n{\n", cb_name);
    fprintf(fjava_cb, "  public %s %s(",
	fct->rtype->JavaTypeName(), cb_name);

    Arg* a;
    int first;
    for (first = 1, a = fct->args; a != NULL; a = a->next)
    {
        // Skip the void pointer
        if (a->type->is_void() && a->type->indirection_count == 1) continue;
	if (!implicit(a))
	{
	    const char* jtna = a->type->JavaTypeName();
	    fprintf(fjava_cb, "%s%s %s", first ? "": ", ", jtna, a->name);
	    first = 0;
	}
    }
    fprintf(fjava_cb, ");\n}\n");

    return 0;
}

int
JNIEmitter::emit_comment(FILE* fjava, const char* comment, const char* add)
{
    /* If there's no comment, skip it.
     */
    if (!comment || strlen(comment) < 1)
    {
	if (add)
	{
	    fprintf(fjava, "/**%s*/\n", add);
	}
	return 0;
    }

    /* If this comment is a javadoc style comment, pass it directly
     * through.
     */
    if (comment[0] == '*')
    {
	fprintf(fjava, "/**%s%s*/\n", comment + 1, add ? add : "");
	return 0;
    }

    /* Otherwise, make it into a javadoc comment.
     */
    fprintf(fjava, "/**%s%s*/\n", comment, add ? add : "");
    return 0;
}

const char* JNIEmitter::jni_type_spec_of_type(CType* t)
{
    const char* jn = t->JavaTypeName();
    if (!jn) return NULL;

    /* Objects.
     */
    if (t->is_object())
    {
	Class* c = jar->findClassByType(t);
	if (!c)
	{
	    printf("cxxwrap internal error %s:%d\n", __FILE__, __LINE__);
	    return NULL;
	}

	char buf[TOKEN_BUFFER_SIZE];
	const char* poc = c->slashedPackageName();
	sprintf(buf, "L%s%s%s;", poc, poc[0] == '\0' ? "" : "/", jn);
	return global.pool.add(buf);
    }

    /* Primitive types.
     */
    if (t->indirection_count > 0) return NULL;
    if (!strcmp(jn, "boolean")) return "Z";
    if (!strcmp(jn, "byte")) return "B";
    if (!strcmp(jn, "char")) return "C";
    if (!strcmp(jn, "short")) return "S";
    if (!strcmp(jn, "int")) return "I";
    if (!strcmp(jn, "long")) return "J";
    if (!strcmp(jn, "float")) return "F";
    if (!strcmp(jn, "double")) return "D";
    if (!strcmp(jn, "void")) return "V";

    return NULL;
}

const char* JNIEmitter::jni_type_spec(Method* m)
{
    char buf[TOKEN_BUFFER_SIZE];
    Arg* a;
    const char* t;

    sprintf(buf, "(");
    for (a = m->args; a; a = a->next)
    {
	t = jni_type_spec_of_type(a->type);
	if (!t) return NULL;
	strcat(buf, t);
    }
    strcat(buf, ")");

    t = jni_type_spec_of_type(m->type);
    if (!t) return NULL;
    strcat(buf, t);

    return global.pool.add(buf);
}

