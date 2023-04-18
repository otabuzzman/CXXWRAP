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
 * $Id: Class.cxx,v 1.42 2006/12/17 04:08:11 deaven Exp $
 */

#include <string.h>
#include <ctype.h>
#include "ClassJar.h"
#include "Class.h"
#include "EnumCType.h"
#include "FunctionCType.h"
#include "ArrayCType.h"

const char* Class::_packagePrefix = NULL;

static EnumCType* find_enum(ObjectCType* oct, Class* c)
{
    if (c)
    {
	Method* m;
	for (m = c->methods; m; m = m->next)
	{
	    if (m->type->modifiers & CType::M_ENUM)
	    {
		EnumCType* ect = (EnumCType*) m->type; // XXX
		if (!strcmp(oct->name(), ect->getName()))
		{
		    ect = (EnumCType*) ect->clone();
		    ect->modifiers |= oct->modifiers;
		    return ect;
		}
	    }
	}

	return find_enum(oct, c->parentClass());
    }

    return NULL;
}

void Class::subst_typedef_fct_args(CType* t, Class* c)
{
    /* If this is a FunctionCType, its return type and arguments
     * may also be in need of typedef expansion.
     */
    if (!strcmp(t->classname(),"FunctionCType"))
    {
        FunctionCType *fct = (FunctionCType*)t;
        fct->rtype = Class::subst_typedef(fct->rtype, c, 1);
        for (Arg* fa = fct->args; fa; fa = fa->next)
        {
            fa->type = Class::subst_typedef(fa->type, c, 1);
        }
    }
}

CType* Class::subst_typedef(CType* t, Class* c, int clone)
{
    CType* ct = NULL;
    const char* ctn = t->BareCTypeName();

    // defined in this class or parents?
    while (c)
    {
	ct = c->typedefs.get(ctn);
	if (ct) break;
	c = c->parentClass();
    }

    if (!ct)
    {
	// defined globally (file scope)?
	ct = global.typedefs.get(ctn);
        // If this is a FunctionCType, its arguments gets expanded here
        if (ct) subst_typedef_fct_args(ct, c);
    }

    if (ct)
    {
        /* typedefs can be defined recursively.  If the original type
	 * was a typedef, the typedef's primitive type is cloned. It
	 * may have modifiers and indirections we must apply only to
	 * the new type.  Also, insure that the final chased down
	 * result is never marked as a TYPEDEF -- it isn't, anymore.
	 */
        ct = subst_typedef(ct, c, 1);
	ct->indirection_count += t->indirection_count;
	ct->modifiers |= t->modifiers;
	ct->modifiers &= ~(CType::M_TYPEDEF);
        return ct;
    }

    if (clone)
    {
        t = t->clone();
        if (t->modifiers & CType::M_TYPEDEF) t->modifiers ^= CType::M_TYPEDEF;
        return t;
    }
    return t;
}

CType* Class::subst_enum(CType* t, Class* c, ClassJar* jar)
{
    if (t->is_object())
    {
	ObjectCType* oct = (ObjectCType*) t;
	EnumCType* ect;

	/* Handle specifically scoped enumerations first.
	 */
	const char* scope = oct->scope();
	if (scope)
	{
	    c = jar->findClassByName(scope);
	    ect = find_enum(oct, c);
	    if (ect) return ect;
	}

	/* Look for locally scoped enumerations next.
	 */
	ect = find_enum(oct, c);
	if (ect) return ect;
    }

    return t;
}

Class::Class(const char* _comment,
    ObjectCType* type_, ObjectCType* parent_, Method* methods_) :
    Token(_comment),
    parent(parent_),
    methods(methods_),
    _type(type_),
    _parameters(NULL),
    _cachedFullName(NULL),
    _packageName(NULL),
    _cached(0),
    _serial(0)
{
    /* Check to make sure that all methods marked as constructors
     * have the same name as the class.  The parser assumes that 
     * unadorned functions are constructors, but they might be 
     * macro calls, for example.
     *
     * Also mark enumerations as belonging to me.
     */
    Method* m;
    Method* mlast = NULL;
    for (m = methods; m; m = m->next)
    {
	if (m->type->modifiers & CType::M_CTOR)
	{
	    if (strcmp(m->name, fullName()))
	    {
		if (global.verbose)
		{
		    printf("removing guessed constructor %s::%s()\n",
			fullName(), m->name);
		}
		if (m != methods && mlast)
		{
		    mlast->next = m->next;
		    m = mlast;
		}
		else
		{
		    methods = m->next;
		}
	    }
	}
	if (m->notes & Method::ENUM_DEFINE)
	{
	    EnumCType* ect = (EnumCType*) m->type; // XXX
	    ect->setParent(this); // make sure
	}
	mlast = m;
    }

    /* Assign serial numbers to all of the methods in this 
     * class.
     */
    _serial = 0;
    for (m = methods; m; m = m->next)
    {
	m->serial = next_serial();
    }
}

Class::Class(const Class& other) :
    Token(other),
    parent(other.parent ? new ObjectCType(*other.parent) : NULL),
    methods(other.methods ? other.methods->clone() : NULL),
    _type(new ObjectCType(*other._type)),
    _parameters(other._parameters),
    _cachedFullName(NULL),
    _packageName(NULL),
    _cached(other._cached),
    _serial(other._serial)
{
    /* Mark enumerations as belonging to me.
     */
    Method* m;
    for (m = methods; m; m = m->next)
    {
	if (m->notes & Method::ENUM_DEFINE)
	{
	    EnumCType* ect = (EnumCType*) m->type; // XXX
	    ect->setParent(this); // make sure
	}
    }
}

Class::Class(const Token& other) :
    Token(other),
    parent(NULL),
    methods(NULL),
    _type(NULL),
    _parameters(NULL),
    _cachedFullName(NULL),
    _packageName(NULL),
    _cached(0),
    _serial(0)
{
}

int Class::writeTo(FILE* f)
{
    Token::writeTo(f);
    writeTokenTo(parent, f);
    writeTokenTo(methods, f);
    writeTokenTo(_type, f);
    writeTokenTo(_parameters, f);
    writeStringTo(_packageName, f);
    writeIntTo(_serial, f);
    return 0;
}

Class* Class::readFrom(FILE* f)
{
    Token* t = Token::readFrom(f);
    Class* c = new Class(*t);
    delete t;
    c->parent = (ObjectCType*) readTokenFrom(f);
    c->methods = (Method*) readTokenFrom(f);
    c->_type = (ObjectCType*) readTokenFrom(f);
    c->_parameters = (Arg*) readTokenFrom(f);
    c->_packageName = readStringFrom(f);
    c->_cachedFullName = NULL;
    c->_cached = 1;
    c->_serial = readIntFrom(f);
    return c;
}

void Class::setType(ObjectCType* t)
{
    _type = t;
    _cachedFullName = NULL;
    _packageName = NULL;

    /* If this is a template (has args), make sure any 
     * references to this class without args get copies of
     * the args.  e.g.
     *
     *   template<X> class B { B(const B& other); }
     *
     * is equivalent syntactically to 
     *
     *   template<X> class B { B(const B<X>& other); }
     */
    if (type()->args)
    {
	Method* m;
	for (m = methods; m; m = m->next)
	{
	    if (m->type->is_object())
	    {
		ObjectCType* oct = (ObjectCType*) m->type;
		if (!strcmp(oct->objname, name()))
		{
		    oct->args = type()->args;
		}
	    }

	    Arg* a;
	    for (a = m->args; a; a = a->next)
	    {
		if (a->type->is_object())
		{
		    ObjectCType* oct = (ObjectCType*) a->type;
		    if (!strcmp(oct->objname, name()))
		    {
			oct->args = type()->args;
		    }
		}
	    }
	}
    }

}

int Class::isInheritedMethod(Method* mm)
{
    Method* m;
    for (m = methods; m; m = m->next)
    {
	if (m->hasSameSignature(mm))
	{
	    return 0;
	}
    }
    return 1;
}

Method* Class::completeMethodList()
{
    Method* list = NULL;

    // copy our list...
    Method* m;
    for (m = methods; m; m = m->next)
    {
        Method* nm = new Method(*m);
	nm->next = list;
	list = nm;
    }

    // ...and add inherited methods
    Class* p = parentClass();
    if (p)
    {
	Method *plist = p->completeMethodList();
	Method* pm;
	for (pm = plist; pm; pm = pm->next)
	{
	    // do not include constructors or destructors
	    if (pm->type->modifiers & (CType::M_CTOR | CType::M_DTOR))
		continue;

	    // is this method already here (was an override)
	    int haveIt = 0;
	    for (m = list; m; m = m->next)
	    {
		if (m->hasSameSignature(pm))
		{
		    haveIt = 1;
		    break;
		}
	    }

	    if (!haveIt)
	    {
		m = new Method(*pm);
		m->next = list;
		list = m;
	    }
	}
	delete plist;
    }

    return list;
}

Method* Class::baseMethodOf(Method* m)
{
    Class* p = parentClass();

    if (p != NULL)
    {
	Method* pm = p->methodWithSignature(m);
	if (pm) return pm;
    }

    return m;
}

Method* Class::methodWithSignature(Method* m)
{
    Method *plist = completeMethodList();
    Method* pm;
    for (pm = plist; pm != NULL; pm = pm->next)
    {
	if (m->hasSameSignature(pm))
	{
	    /** Clip out just the one we need, and delete the rest.
	     */
	    Method* ret = new Method(*pm);
	    delete plist;
	    return ret;
	}
    }
    delete plist;
    return NULL;
}

static const char* dirname(const char* fn)
{
    char buf[FILENAME_MAX];
    strcpy(buf, fn);
    char* p = buf;
    char* slash = strrchr(p, '/');
    if (slash)
    {
	*slash = '\0';
	while (isspace(p[0]) || p[0] == '.' || p[0] == '/') p++;
    }
    else
    {
	strcpy(buf, ".");
    }
    return global.pool.add(p);
}

const char* Class::packageName()
{
    if (!_packageName && type()->source_file)
    {
	_packageName = dirname(type()->source_file);

	/* special case if the directory is the current directory,
	 * then no package is used.
	 */
	if (strcmp(_packageName, ".") == 0)
	{
	    _packageName = "";
	}

	/* append C++ namespace, if any.
	 */
	if (type()->nmspace)
	{
	    if (strlen(_packageName) > 0)
	    {
		char buf[TOKEN_BUFFER_SIZE];
		sprintf(buf, "%s.%s", _packageName, type()->nmspace);
	    }
	    else
	    {
		_packageName = type()->nmspace;
	    }
	}

	if (_packagePrefix)
	{
	    char buf[FILENAME_MAX];
	    sprintf(buf, "%s%s%s", _packagePrefix,
		strlen(_packageName) > 0 ? "/" : "",
		_packageName);
	    _packageName = global.pool.add(buf);
	}
    }

    if (!_packageName && type()->nmspace)
    {
	_packageName = type()->nmspace;
    }

    return _packageName;
}

const char* Class::dottedPackageName()
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s", packageName());

    char* p;
    for (p = buf; *p != '\0'; p++)
    {
	if (*p == '/' || *p == '\\') *p = '.';
    }

    return global.pool.add(buf);
}

const char* Class::slashedPackageName()
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s", packageName());

    char* p;
    for (p = buf; *p != '\0'; p++)
    {
	if (*p == '.' || *p == '\\') *p = '/';
    }

    return global.pool.add(buf);
}

Class* Class::parentClass()
{
    if (!parent) return NULL;
    return global.classJar.findClassByType(parent, this);
}

void Class::expand(Arg* args)
{
    // set our type
    type()->args = args;
    setType(type()); // since we "changed" type

    // go through each method/arg, expanding
    Method* m;
    for (m = methods; m; m = m->next)
    {
	m->type = m->type->expand(parameters(), args);

	Arg* a;
	for (a = m->args; a; a = a->next)
	{
	    a->type = a->type->expand(parameters(), args);
	}
    }

    // expand our parent type if necessary
    if (parent)
    {
	parent = (ObjectCType*) parent->expand(parameters(), args);
    }

    setParameters(NULL); // we have instantiated, so we have none

    if (global.verbose)
    {
	printf("Instantiated \"%s\" in package \"%s\"\n",
	    fullName(), packageName() ? packageName() : "");
    }
}

const char* Class::computeFullName()
{
    if (!parameters())
    {
	return type()->CTypeName();
    }

    char buf[TOKEN_BUFFER_SIZE], buf2[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s<", type()->CTypeName());
    Arg* p;
    for (p = parameters(); p != NULL; p = p->next)
    {
	sprintf(buf2, "%s%s", p->name, p->next ? "," : "");
	strcat(buf, buf2);
    }
    strcat(buf, ">");
    return global.pool.add(buf);
}

void Class::print()
{
    printf("fullName=%s name=%s type={",
	fullName(), name());
    type()->print();
    printf("}");
}

void Class::printAll()
{
    Class* c;
    ClassJarIterator iter(global.classJar.iterator());
    while ((c = iter.next()))
    {
	printf("{");
	c->print();
	printf("}\n");
	fflush(stdout);
    }
}

int Class::isAbstract()
{
    int result = 0;

    /* are any of my methods pure virtual?
     */
    Method* mc = completeMethodList();
    Method* m;
    for (m = mc; m && !result; m = m->next)
    {
	if (m->is_pure) result = 1;
    }

    delete mc;

    return result;
}

void Class::make_consistent(ClassJar* jar)
{
    Class* c;
    ClassJarIterator iter(jar->iterator());
    while ((c = iter.next()))
    {
	Method* m;
	for (m = c->methods; m != NULL; m = m->next)
	{
	    if (!(m->type->modifiers & CType::M_VIRTUAL))
	    {
		Class* p = c;
		while ((p = p->parentClass()) != NULL)
		{
		    Method* mp;
		    for (mp = p->methods; mp != NULL; mp = mp->next)
		    {
			if ((!strcmp(mp->name, m->name)) &&
			    (mp->hasSameSignature(m)) &&
			    (mp->type->modifiers & CType::M_VIRTUAL))
			{
			    m->type->modifiers |= CType::M_VIRTUAL;
			}
		    }
		}
	    }
	}
    }
}

ClassJar* Class::derived()
{
    ClassJar* jar = new ClassJar();
    Class* c;
    ClassJarIterator iter(global.classJar.iterator());
    while ((c = iter.next()))
    {
	if (c != this && c->parentClass() == this)
	{
	    jar->add(c);
	}
    }

    return jar;
}

static void depends_recurse(StringPool* visited, ClassJar* jar, Class* c)
{
    if (c && !visited->contains(c->fullName()))
    {
	jar->add(c);
	visited->add(c->fullName());

	/* Next, enumerate all objects referenced.
	 */
	Method* m;
	for (m = c->methods; m; m = m->next)
	{
	    if (m->type->is_object())
	    {
		depends_recurse(visited, jar, 
		    global.classJar.findClassByType(m->type));
	    }
	    Arg* a;
	    for (a = m->args; a; a = a->next)
	    {
		if (a->type->is_object())
		{
		    depends_recurse(visited, jar, 
			global.classJar.findClassByType(a->type));
		}
	    }
	}
    }
}

ClassJar* Class::depends()
{
    ClassJar* result;

    /* First, we depend on all our parent does.
     */
    Class* p = parentClass();
    if (p)
    {
	result = p->depends();
    }
    else
    {
	result = new ClassJar();
    }

    StringPool visited;
    depends_recurse(&visited, result, this);

    return result;
}

StringPool* Class::packageDepends()
{
    StringPool* result = new StringPool();

    ClassJar* jar = depends();
    ClassJarIterator iter(jar->iterator());

    Class* c;
    while ((c = iter.next()))
    {
	result->add(c->packageName());
    }

    return result;
}

void Class::setPackagePrefix(const char* pfx)
{
    if (pfx == NULL || strlen(pfx) < 1) 
    {
	_packagePrefix = NULL;
	return;
    }

    char buf[FILENAME_MAX];
    strcpy(buf, pfx);
    char* p;
    for (p = buf; *p != '\0'; p++)
    {
	switch (*p)
	{
	    case '.': 
	    case '\\':
		*p = '/';
		break;

	    default:
		break;
	}
    }

    p = buf + strlen(buf) - 1;
    if (*p == '/') *p = '\0';

    _packagePrefix = global.pool.add(buf);
}
