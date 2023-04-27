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
 * $Id: ObjectCType.cxx,v 1.33 2006/12/17 04:08:11 deaven Exp $
 */

#include "Class.h"
#include "ObjectCType.h"
#include <string.h>
#include <ctype.h>

int
ObjectCType::writeTo(FILE* f)
{
    int s = CType::writeTo(f);
    s |= writeStringTo(objname, f);
    s |= writeStringTo(source_file, f);
    s |= writeTokenTo(args, f);
    return s;
}

ObjectCType*
ObjectCType::readFrom(FILE* f)
{
    CType* ct = CType::readFrom(f);
    ObjectCType* oct = new ObjectCType(*ct);
    delete ct;
    oct->objname = readStringFrom(f);
    oct->source_file = readStringFrom(f);
    oct->args = (Arg*) readTokenFrom(f);
    return oct;
}

const char*
ObjectCType::JavaTypeName()
{
    if (indirection_count > 1)
    {
	/* We can wrap this if there are two indirections, of type
	 * reference to a pointer.
	 */
	if ((indirection_count > 2) || !(modifiers & M_REFERENCE)) return NULL;
    }
    if (!global.classJar.findClassByType(this)) return NULL;

    if (args)
    {
	char buf[TOKEN_BUFFER_SIZE];

	sprintf(buf, "%sOf", name());
	Arg* p;
	for (p = args; p != NULL; p = p->next)
	{
	    global.classJar.findClassByType(p->type); // expand as needed
	    const char* jtn = p->type->JavaTypeName();
	    if (!jtn)
	    {
		return NULL; // error path
	    }
	    sprintf(buf + strlen(buf), "%s%s",
		jtn, p->next ? "And" : "");
	}
	return global.pool.add(buf);
    }
    
    return name();
}

const char*
ObjectCType::PrivateJavaTypeName()
{
    if (!JavaTypeName()) return NULL;
    return "long";
}

const char*
ObjectCType::DefaultPrivateJavaValue()
{
    return "0";
}

const char*
ObjectCType::JNICast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    int storage_mods = modifiers & (M_POINTER | M_REFERENCE);

    switch (storage_mods) 
    {
	case M_POINTER:
	    sprintf(buf, "(%s*) %s", BareCTypeName(), expr);
	    return global.pool.add(buf);

	default:
        case M_REFERENCE:
	    if (indirection_count == 2)
	    {
		/* Make sure this is wrapped as a reference to a
		 * pointer, not just as a reference.
		 */
		sprintf(buf, "__%s_ptr", expr);		
	    }
	    else
	    {
		sprintf(buf, "*((%s*) %s)", BareCTypeName(), expr);
	    }
	    return global.pool.add(buf);
    }

    return NULL;
}

const char*
ObjectCType::JNIUpCast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    int storage_mods = modifiers & (M_POINTER | M_REFERENCE);

    switch (storage_mods) 
    {
	case M_POINTER:
	    sprintf(buf, "(jlong) %s", expr);
	    return global.pool.add(buf);

	default:
        case M_REFERENCE:
	    sprintf(buf, "(jlong) &%s", expr);
	    return global.pool.add(buf);
    }

    return NULL;
}

const char*
ObjectCType::JNIInvokeUpCast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s_j", expr);
    return global.pool.add(buf);
}

const char*
ObjectCType::JNIInvokeBegin(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    int storage_mods = modifiers & (M_POINTER | M_REFERENCE);
    const char* deref = storage_mods == M_POINTER ? "" : "&";
    Class* c = global.classJar.findClassByType(this);
    if (!c) return "";

    const char* spn = c->slashedPackageName();
    const char* opt_slash = *spn == '\0' ? "" : "/";

    /* Create a new java object.
     */
    sprintf(buf,
	"jobject %s_j; {\n"
	"jclass lc = env->FindClass(\"java/lang/Long\");\n"
	"jmethodID lmid = env->GetMethodID(lc, \"<init>\", \"(J)V\");\n"
	"jobject ldata = env->NewObject(lc, lmid, (jlong) %s%s, NULL);\n"
	"jclass clazz = env->FindClass(\"%s%s%s\");\n"
	"jmethodID mid = env->GetMethodID(clazz, \"<init>\", \"(Ljava/lang/Long;)V\");\n"
	"%s_j = env->NewObject(clazz, mid, ldata, NULL);\n"
	"}\n",
	expr,
	deref, expr,
	spn, opt_slash, name(),
	expr);

    return global.pool.add(buf);
}

const char*
ObjectCType::JavaCast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "(%s == null ? 0 : %s.getCxxwrapImpl().longValue())",
	expr, expr);
    return global.pool.add(buf);
}

const char*
ObjectCType::JavaUpCast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "(%s == 0 ? null : new %s(Long.valueOf(%s)))",
	expr, JavaTypeName(), expr);
    return global.pool.add(buf);
}

const char*
ObjectCType::OperatorSuffix()
{
    return BareCTypeName();
}

const char*
ObjectCType::JavaJNITypeName()
{
    return "jobject";
}

const char*
ObjectCType::JNIFunctionTypeName()
{
    if (indirection_count > 1)
    {
        // There is no such thing as a reference to a reference in Java
        return NULL;
    }
    return "Object";
}

const char*
ObjectCType::JNITypeName()
{
    if (!global.classJar.findClassByType(this)) return NULL;
    return "jlong";
}

const char*
ObjectCType::nmspaceName()
{
    if (nmspace)
    {
	char buf[TOKEN_BUFFER_SIZE];
	sprintf(buf, "%s::%s", nmspace, objname);
	return global.pool.add(buf);
    }

    return objname;
}

const char*
ObjectCType::BareCTypeName()
{
    if (args)
    {
	char buf[TOKEN_BUFFER_SIZE];
	sprintf(buf, "%s<", nmspaceName());
	Arg* p;
	for (p = args; p != NULL; p = p->next)
	{
	    sprintf(buf + strlen(buf), "%s%s",
		p == args ? "" : ",", p->type->CTypeName());
	}
	strcat(buf, "> ");
	return global.pool.add(buf);
    }
    
    return nmspaceName();
}

CType* ObjectCType::expand(Arg* parameters, Arg* targs)
{
    /* if it's an object type with name equal to an arg name, use
     * the substitute type.
     *
     * class vector<class X>;
     * typedef vector<foo> nw;
     *
     * par->name = parameter name ("X")
     * curarg->type is the template argument ("foo")
     */

    /* Are we ourselves an expandable type?
     */
    if (!args) 
    {
	Arg* curarg = targs;
	Arg* par;
	for (par = parameters; par; par = par->next)
	{
	    if (!curarg) 
	    {
		printf("ObjectCType::expand not enough args\n");
		return this;
	    }

	    if (!strcmp(objname, par->name))
	    {
		/* we really need to compound the modifiers here,
		 * so that the template argument is substituted into
		 * the type information.  For example:
		 *
		 * Given
		 *   template<class T>class vector{T* bar();};
		 * and expanding
		 *   vector<foo*>
		 * should yield
		 *   class {foo** bar();}
		 *
		 * Note the "foo**" instead of "foo*" above.  But this is not
		 * currently allowed by the limited C type representation here,
		 * so we are stuck with a bug for now.
		 *
		 * Nah, I can handle the above case by counting indirection levels.
		 * More complex type compounds are still not supported.
		 */
		CType* result = curarg->type->clone();
		result->modifiers |= modifiers; // just copy in, don't compound
		result->indirection_count += indirection_count;
		return result;
	    }

	    curarg = curarg->next;
	}
    }

    /* If not us, any of our arguments?
     */
    ObjectCType* result = new ObjectCType(*this);
    int changed = 0;
    Arg* a;
    for (a = result->args; a; a = a->next)
    {
	CType* t = a->type->expand(parameters, targs);
	if (t != a->type)
	{
	    changed++;
	    a->type = t;
	}
    }
    if (changed) return result;
    delete result;

    /* By default, just do nothing.
     */
    return this;
}

int 
ObjectCType::is_object()
{
    return 1;
}

const char*
ObjectCType::scope()
{
    const char* ncc = strstr(objname, "::");
    const char* cc = NULL;

    while (ncc)
    {
	cc = ncc;
	ncc = strstr(cc + 2, "::");
    }

    if (cc)
    {
	char buf[TOKEN_BUFFER_SIZE];
	strncpy(buf, objname, cc - objname);
	buf[cc - objname] = '\0';
	return global.pool.add(buf);
    }

    return NULL;
}

const char*
ObjectCType::name()
{
    const char* ncc = strstr(objname, "::");
    const char* cc = NULL;

    while (ncc)
    {
	cc = ncc;
	ncc = strstr(cc + 2, "::");
    }

    if (cc)
    {
	char buf[TOKEN_BUFFER_SIZE];
	strcpy(buf, cc + 2);
	return global.pool.add(buf);
    }

    return objname;
}

const char*
ObjectCType::JNIBegin(const char* expr)
{
    /* If this is a reference to a pointer, give some code to allow us
     * to detect a change in the pointer variable upon the return of
     * the function call.
     */
    if ((indirection_count == 2) && (modifiers & M_REFERENCE))
    {
	char buf[CODE_BUFFER_SIZE];
	sprintf(buf, "%s* __%s_ptr = (%s*) %s;\n"
		     "%s* __%s_ptr_copy = __%s_ptr;\n", 
		BareCTypeName(), expr, BareCTypeName(), expr,
		BareCTypeName(), expr, expr);
        return global.pool.add(buf);
    }
    return "";
}

const char*
ObjectCType::JNIEnd(const char* expr)
{
    /* If this is a reference to a pointer, give some code to check if
     * the pointer variable has been modified by the function call.
     * If it has, change the implementation value of the Java object
     * to reflect this.
     *
     * FIXME: The code can probably be optimized/shortened some more.
     */
    if ((indirection_count == 2) && (modifiers & M_REFERENCE))
    {
	char buf[CODE_BUFFER_SIZE*2];
	sprintf(buf,
		"if (__%s_ptr != __%s_ptr_copy)\n"
		"{\n"
		"  jclass _cls = (__env)->GetObjectClass(__jobj_%s);\n"
		"  jfieldID _fid = (__env)->GetFieldID(_cls, \"implementation\", \"Ljava/lang/Long;\");\n"
		//"  if (_fid == NULL);" // FIXME: Should throw exception
		"  jclass _Long_class = (__env)->FindClass(\"java/lang/Long\");\n"
		"  jmethodID _Long_init = (__env)->GetMethodID(_Long_class, \"<init>\", \"(J)V\");\n"
		"  jobject _new_implementation = (__env)->NewObject(_Long_class, _Long_init,\n"
		"    (jlong) __%s_ptr);\n"
		"  (__env)->SetObjectField(__jobj_%s, _fid, _new_implementation);\n"
		"}\n",
		expr, expr,
		expr,
		expr,
		expr);
        return global.pool.add(buf);
    }
    return "";
}

const char*
ObjectCType::CBCast(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    sprintf(buf, "__T%s", expr);
    return global.pool.add(buf);
}

const char*
ObjectCType::CBUpCast(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    int storage_mods = modifiers & (M_POINTER | M_REFERENCE);
    const char* deref = storage_mods == M_POINTER ? "" : "*";
    
    sprintf(buf,
        "{\n"
        "  jfieldID __fid = __env->GetFieldID(__env->GetObjectClass(%s), \"implementation\", \"Ljava/lang/Long;\");\n"
        "  jobject __impl_obj = __env->GetObjectField(%s, __fid);\n"
        "  jlong __impl_value = __env->CallLongMethod(__impl_obj, __env->GetMethodID(__env->FindClass(\"java/lang/Long\"), \"longValue\", \"()J\"));\n"
        "  return %s((%s%s*)__impl_value);\n"
        "}\n",
        expr,
        expr,
        deref, CTypeNamePrefix(), BareCTypeName()
    );
    return global.pool.add(buf);
}

const char*
ObjectCType::CBBegin(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    int storage_mods = modifiers & (M_POINTER | M_REFERENCE);
    const char* deref = storage_mods == M_POINTER ? "" : "&";
    Class* c = global.classJar.findClassByType(this);
    if (!c) return "";

    const char* spn = c->slashedPackageName();
    const char* opt_slash = *spn == '\0' ? "" : "/";

    // If this is a pointer, we should check if it is null before creating the java object
    char check_ptr_null[CODE_BUFFER_SIZE];
    if (storage_mods == M_POINTER)
    {
        sprintf(check_ptr_null, "%s == 0 ? NULL : ", expr);
    }
    else
    {
        strcpy(check_ptr_null, "");
    }

    /* Create a new java object (this code can be split over multiple
     * lines to make it a bit more readable).
     */
    sprintf(buf, "jobject __T%s = %s__env->NewObject(__env->FindClass(\"%s%s%s\"), __env->GetMethodID(__env->FindClass(\"%s%s%s\"), \"<init>\", \"(Ljava/lang/Long;)V\"), __env->NewObject(__env->FindClass(\"java/lang/Long\"), __env->GetMethodID(__env->FindClass(\"java/lang/Long\"), \"<init>\", \"(J)V\"), (jlong) %s%s ));\n",
        expr, check_ptr_null,
        spn, opt_slash, name(),
        spn, opt_slash, name(),
        deref, expr
    );
    return global.pool.add(buf);
}

const char*
ObjectCType::CBEnd(const char* expr)
{    
    return "";
}

const char*
ObjectCType::JavaTypeID()
{
    if (indirection_count > 1)
    {
        // There is no such thing as a reference to a reference in Java
        return NULL;
    }
    
    char buf[CODE_BUFFER_SIZE];
    Class* c = global.classJar.findClassByType(this);
    if (!c) return "";

    const char* spn = c->slashedPackageName();
    const char* opt_slash = *spn == '\0' ? "" : "/";

    sprintf(buf, "L%s%s%s;", spn, opt_slash, name());
    return global.pool.add(buf);
}











