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
 * $Id: PrimitiveCType.cxx,v 1.30 2006/12/17 04:08:11 deaven Exp $
 */

#include <ctype.h>

#include "defs.h"
#include "PrimitiveCType.h"
#include <string.h>

static PrimitiveCTypeAssignment default_assignments[] = {
    { PrimitiveCType::T_CHAR, "char", sizeof(char), 1 },
    { PrimitiveCType::T_SHORT, "short", sizeof(short), 2 },
    { PrimitiveCType::T_INT, "int", sizeof(int), 4 },
    { PrimitiveCType::T_LONG, "long", sizeof(long), 8 },
    { PrimitiveCType::T_FLOAT, "float", sizeof(float), 4 },
    { PrimitiveCType::T_DOUBLE, "double", sizeof(double), 8 },
    { -1, NULL, 0, 0 }
};

static PrimitiveCTypeAssignment* find_assignment(PrimitiveCType::Type t)
{
    PrimitiveCTypeAssignment* r = default_assignments;
    while (r->type >= 0)
    {
	if (t == r->type) return r;
	r++;
    }
    return NULL;
}

PrimitiveCTypeAssignment*
PrimitiveCType::getAssignment()
{
    if (!_assignment)
    {
	_assignment = find_assignment(type);
    }
    return _assignment;
}

int
PrimitiveCType::writeTo(FILE* f)
{
    CType::writeTo(f);
    writeIntTo(type, f);
    return 0;
}

PrimitiveCType*
PrimitiveCType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    PrimitiveCType* a = new PrimitiveCType(*t);
    delete t;
    a->type = (Type) readIntFrom(f);
    a->_assignment = NULL;
    return a;
}

static const char* skip_unsigned(const char* base)
{
    // skip over any "unsigned" qualifiers
    static char* us = "unsigned ";
    if (strncmp(base, us, strlen(us)) == 0)
    {
	base += strlen(us);
    }

    return base;
}

PrimitiveCType::PrimitiveCType(const char* _comment, Type t) :
    CType(_comment), type(t), _assignment(NULL)
{
}

const char*
PrimitiveCType::JavaTypeName()
{
    const char* base = BareCTypeName();

    // handle chars specially
    if (is_string())
    {
	if (indirection_count > 2) return NULL;
	if (indirection_count == 2) return "String[]";
	return "String"; // only indirection_count == 1 gets here
    }
	
    if (indirection_count > 1) return NULL;

    // bools are different
    if (type == T_BOOL) base = "boolean";

    // handle chars as bytes
    if (type == T_CHAR) base = "byte";

    // skip over any "unsigned" qualifiers
    base = skip_unsigned(base);

    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s%s", base, 
	(modifiers & (M_REFERENCE | M_POINTER)) ? "[]" : "");

    return global.pool.add(buf);
}

const char*
PrimitiveCType::DefaultPrivateJavaValue()
{
    // handle chars specially
    if (is_string()) return "null";

    if (indirection_count > 0) return "null";

    // bools are different
    if (type == T_BOOL) return "false";

    // floats are different
    if (type == T_FLOAT) return "(float) 0.0";
    if (type == T_DOUBLE) return "0.0";

    return "0";
}

const char*
PrimitiveCType::jni_base_name()
{
    switch(type) {
	case T_FLOAT:  return "jfloat";
	case T_DOUBLE: return "jdouble";
	case T_BOOL:   return "jboolean";
	case T_SHORT:  return "jshort";
	case T_INT:    return "jint";
	case T_LONG:   return "jlong";
	case T_CHAR:
	{
	    if (is_string())
	    {
		if (indirection_count > 2) return NULL;
		if (indirection_count == 2) return "jobject";
		return "jstring";
	    }
	    return "jbyte";
	}
    }

    return NULL;
}

const char*
PrimitiveCType::JNITypeName()
{
    if (!JavaTypeName()) return NULL;

    char buf[TOKEN_BUFFER_SIZE];
    strcpy(buf, jni_base_name());

    if ((!is_string() && indirection_count > 0) ||
	(is_string() && indirection_count == 2))
    {
	strcat(buf, "Array");
    }

    return global.pool.add(buf);
}

const char*
PrimitiveCType::jni_array_name()
{
    switch(type) {
	case T_FLOAT:  return "FloatArray";
	case T_DOUBLE: return "DoubleArray";
	case T_BOOL:   return "BooleanArray";
	case T_SHORT:  return "ShortArray";
	case T_INT:    return "IntArray";
	case T_LONG:   return "LongArray";
	case T_CHAR:
	{
	    if (is_string())
	    {
		if (indirection_count == 2)
		{
		    return "ObjectArray";
		}
		else
		{
		    return "StringUTF";
		}
	    }
	    return "ByteArray";
	}
    }

    return NULL;
}

int
PrimitiveCType::is_string()
{
    if (type == T_CHAR &&
	(!(modifiers & M_UNSIGNED)) &&
	(modifiers & M_POINTER))
    {
	return 1;
    }
    return 0;
}

int
PrimitiveCType::is_wrong_size()
{
    PrimitiveCTypeAssignment* a = getAssignment();
    if (a && a->csize != a->jsize) return 1;
    return 0;
}

const char*
PrimitiveCType::JNIBegin(const char* expr)
{
    if (indirection_count > 0)
    {
	char buf[CODE_BUFFER_SIZE];
	if (is_string() && indirection_count == 1)
	{
	    sprintf(buf, "const char* __T%s = %s == 0 ? NULL : __env->GetStringUTFChars(%s, NULL);\n",
	    expr, expr, expr);
	}
	else
	{
	    if (is_string() && indirection_count == 2)
	    {
		/* need a local char* array
		 */
		sprintf(buf,
		    "int __len_%s = (%s == 0 ? 0 : __env->GetArrayLength(%s));\n"
		    "char** __T%s = __len_%s > 0 ? new char*[__len_%s] : NULL;\n"
		    "char** __cT%s = __len_%s > 0 ? new char*[__len_%s] : NULL;\n"
		    "{ for (int i = 0; i < __len_%s; i++) {\n"
		    " jobject __tmp = (%s == 0 ? 0 : __env->GetObjectArrayElement(%s, i));\n"
		    " __T%s[i] = __cT%s[i] = (__tmp == 0 ? NULL : (char*) __env->GetStringUTFChars((jstring) __tmp, NULL)); }}\n",
		    expr, expr, expr, expr, expr,
		    expr, expr, expr, expr, expr,
		    expr, expr, expr, expr);
	    }
	    else
	    {
		/* If there's an array type mis-match, resolve it by
		 * copying the elements here.
		 */
		if (is_wrong_size())
		{
		    static int warned = 0;
		    const char* jbn = jni_base_name();
		    const char* jan = jni_array_name();
		    const char* bct = BareCTypeName();

		    if (global.verbose && !warned)
		    {
			char wbuf[256];
			sprintf(wbuf, "type size mis-match in C++ "
			    "type %s (!= %s), converting in JNI code",
			    bct, jbn);
			warning(wbuf);
			warned++;
		    }
		    sprintf(buf,
			"%s* ___T%s = __env->Get%sElements(%s, NULL);\n"
			"int ___TL%s = __env->GetArrayLength(%s);\n"
			"%s* __T%s = new %s[___TL%s];\n"
			"{ int i; for(i=0; i<___TL%s; i++) __T%s[i] = ___T%s[i]; }\n",
			jbn, expr, jan, expr,
			expr, expr,
			bct, expr, bct, expr,
			expr, expr, expr);
		}
		else
		{
		    sprintf(buf, "%s* __T%s = __env->Get%sElements(%s, NULL);\n",
			jni_base_name(), expr, jni_array_name(), expr);
		}

	    }

	}
	return global.pool.add(buf);
    }

    return "";
}

const char*
PrimitiveCType::JNIEnd(const char* expr)
{
    if ((modifiers & M_POINTER) || (modifiers & M_REFERENCE))
    {
	char buf[CODE_BUFFER_SIZE];
	if (is_string())
	{
	    if (indirection_count == 2)
	    {
		sprintf(buf,
		    "{ for (int i = 0; i < __len_%s; i++)"
		    " { if (%s != 0) {\n"
		    "    __env->ReleaseStringUTFChars((jstring)__env->GetObjectArrayElement(%s, i), __cT%s[i]);\n"
		    "    if (__T%s[i] != __cT%s[i]) {\n"
		    "      __env->SetObjectArrayElement(%s, i,  __T%s[i] ? __env->NewStringUTF(__T%s[i]) : 0); }\n"
		    "    }\n"
		    "  }\n"
		    " delete[] __T%s; delete[] __cT%s; }\n",
		    expr, expr, expr, expr, expr, expr, expr, expr, expr, expr, expr);
	    }
	    else
	    {
		sprintf(buf, "if (%s != 0) __env->ReleaseStringUTFChars(%s, __T%s);\n",
		    expr, expr, expr);
	    }
	}
	else
	{
	    /* If there's an array type mis-match, resolve it by
	     * copying the elements here.
	     */
	    if (is_wrong_size())
	    {
		sprintf(buf,
		    "{ int i; for(i=0; i<___TL%s; i++) ___T%s[i] = __T%s[i]; }\n"
		    "delete[] __T%s;\n"
		    "__env->Release%sElements(%s, ___T%s, 0);\n",
		    expr, expr, expr,
		    expr,
		    jni_array_name(), expr, expr);
	    }
	    else
	    {
		sprintf(buf,
		    "__env->Release%sElements(%s, __T%s, 0);\n",
		    jni_array_name(), expr, expr);
	    }
	}

	return global.pool.add(buf);
    }

    return "";
}

const char*
PrimitiveCType::JNICast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE] = "";

    if (!(modifiers & M_POINTER) && (modifiers & M_REFERENCE))
    {
        strcat(buf, "*(");
    }

    strcat(buf, "(");
    strcat(buf, CTypeNamePrefix());
    strcat(buf, BareCTypeName());

    int indir_count_temp = indirection_count;
    if (modifiers & M_POINTER)
    {
	/* If this is a reference to a pointer, temporarily decrease
	 * the indirection count, so as to not output to many
	 * asterisks.
	 */
	if (modifiers & M_REFERENCE) indir_count_temp--;
	int i;
	for (i = 0; i < indir_count_temp; i++)
	{
	    strcat(buf, "*");
	}
    }
    else if (modifiers & M_REFERENCE)
    {
	int i;
	for (i = 0; i < indir_count_temp; i++)
	{
	    strcat(buf, "*");
	}
    }
    
    if (modifiers & M_TEMPLATE) strcat(buf, "{template}");

    int refptr = (modifiers & M_POINTER) || (modifiers & M_REFERENCE);
    sprintf(buf+strlen(buf), ")%s%s", (refptr?"__T":""), expr);

    if (!(modifiers & M_POINTER) && (modifiers & M_REFERENCE))
    {
        strcat(buf, ")");
    }

    return global.pool.add(buf);
}

const char*
PrimitiveCType::JNIUpCast(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    if ((modifiers & M_POINTER) || (modifiers & M_REFERENCE))
    {
	if (is_string())
	{
	    sprintf(buf, "(%s == NULL? ((jstring) 0):(__env->NewStringUTF(%s)))", expr,expr);
	}
	else
	{
	    char wbuf[256];
	    sprintf(wbuf, "dummy JNI return array used for \"%s\"", expr);
	    warning(wbuf);
	    sprintf(buf, "__env->New%s(1)", jni_array_name());
	}
    }
    else
    {
	sprintf(buf, "(%s) %s", JNITypeName(), expr);
    }
    return global.pool.add(buf);
}

const char*
PrimitiveCType::OperatorSuffix()
{
    if (is_string()) { return "String"; }
    return JNIFunctionTypeName();
}

const char*
PrimitiveCType::BareCTypeName()
{
    char buf[TOKEN_BUFFER_SIZE];
    buf[0] = '\0';

    if (modifiers & M_UNSIGNED)
    {
	strcpy(buf, "unsigned ");
    }

    const char* s = "unknown";

    switch(type) {
	case T_FLOAT:  s = "float"; break;
	case T_DOUBLE: s = "double"; break;
	case T_BOOL:   s = "bool"; break;
	case T_SHORT:  s = "short"; break;
	case T_INT:    s = "int"; break;
	case T_LONG:   s = "long"; break;
	case T_CHAR:   s = "char"; break;
    }

    strcat(buf, s);

    return global.pool.add(buf);
}

const char*
PrimitiveCType::CBCast(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    if ((modifiers & M_POINTER) || (modifiers & M_REFERENCE))
    {
	sprintf(buf, "__T%s", expr);
    }
    else
    {
	sprintf(buf, "%s", expr);
    }
    return global.pool.add(buf);
}

const char*
PrimitiveCType::CBUpCast(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    if ((modifiers & M_POINTER) || (modifiers & M_REFERENCE))
    {
	if (is_string())
	{
            if (indirection_count >= 2)
            {
                // FIXME: This can be supported, but is not (yet)
                warning("Return type String[] not supported from java callback functions");
                return "";
            }
            else
            {
                /* Duplicate the string so the Java String object
                 * (jstring) can be garbage collected.
                 */
                sprintf(buf,
                    "{\n"
                    "  const char* %sT = (const char*) __env->GetStringUTFChars(%s, NULL);\n"
                    "  char* %sTT = strdup(%sT);\n"
                    "  __env->ReleaseStringUTFChars(%s, %sT);\n"
                    "  return (%s) %sTT;\n"
                    "}\n",
                    expr, expr,
                    expr, expr,
                    expr, expr,
                    CTypeName(), expr
                );
            }
        }            
	else
	{
            // FIXME: This can be supported, but is not (yet)
            warning("Array return type not supported from java callback functions");
            return "";
	}
    }
    else
    {
	sprintf(buf, "return (%s) %s;\n", CTypeName(), expr);
    }
    return global.pool.add(buf);
}

const char*
PrimitiveCType::CBBegin(const char* expr)
{
    if (indirection_count > 0)
    {
	char buf[CODE_BUFFER_SIZE];
	if (is_string() && indirection_count == 1)
	{
	    sprintf(buf, "jstring __T%s = %s == 0 ? NULL : __env->NewStringUTF(%s);\n",
	    expr, expr, expr);
	}
	else
	{
            // This can't happen
            fprintf(stderr, "cxxwrap internal error, pointers to primitive types not supported in callbacks.\n");
            return "";
	}
	return global.pool.add(buf);
    }

    return "";
}

const char*
PrimitiveCType::CBEnd(const char* expr)
{
    return "";
}

const char*
PrimitiveCType::JavaTypeID()
{    
    char buf[CODE_BUFFER_SIZE];

    const char* s = "unknown";
    if (indirection_count > 0)
    {
        if (is_string() && indirection_count == 1)
        {
            s = "Ljava/lang/String;";
        }
        else
        {
            // References to primitive types are not supported in Java
            return NULL;
        }
    }
    else
    {     
        switch(type) {
            case T_FLOAT:  s = "F"; break;
	    case T_DOUBLE: s = "D"; break;
	    case T_BOOL:   s = "Z"; break;
	    case T_SHORT:  s = "S"; break;
	    case T_INT:    s = "I"; break;
	    case T_LONG:   s = "J"; break;
	    case T_CHAR:   s = "C"; break;
        }
    }
    sprintf(buf, "%s", s);

    return global.pool.add(buf);
}

const char*
PrimitiveCType::JavaJNITypeName()
{
    return JNITypeName();
}

const char*
PrimitiveCType::JNIFunctionTypeName()
{
    char buf[CODE_BUFFER_SIZE];

    const char* s = "unknown";
    if (indirection_count > 0)
    {
        if (is_string() && indirection_count == 1)
        {
            s = "Object";
        }
        else
        {
            // References to primitive types are not supported in Java
            return NULL;
        }
    }
    else
    {     
        switch(type) {
            case T_FLOAT:  s = "Float"; break;
	    case T_DOUBLE: s = "Double"; break;
	    case T_BOOL:   s = "Boolean"; break;
	    case T_SHORT:  s = "Short"; break;
	    case T_INT:    s = "Int"; break;
	    case T_LONG:   s = "Long"; break;
	    case T_CHAR:
	    {
		if (is_string())
		{
		    if (indirection_count <= 2) 
		    {
			if (indirection_count == 2) s = "Object";
			s = "Char";
		    }
		}
		s = "Byte";
	    }
	    break;
        }
    }
    sprintf(buf, "%s", s);

    return global.pool.add(buf);
}
