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
 * $Id: Class.h,v 1.24 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __Class_h__
#define __Class_h__

#include "defs.h"
#include "Method.h"
#include "ObjectCType.h"
#include "TypeMap.h"

class ClassJar;

// concrete class, template if parameters != NULL
class Class : public Token {
public:
    Class(const char* comment,
	ObjectCType* type, ObjectCType* parent, Method* methods);
    Class(const Class& other);
    ~Class() { delete methods; }
    virtual const char* classname() const { return "Class"; }

    Method* completeMethodList();
    Method* baseMethodOf(Method* m);
    Method* methodWithSignature(Method* m);
    int isInheritedMethod(Method* m);
    int isAbstract();
    Class* parentClass();
    Class* rootClass() {
	Class* rc = this;
	Class* c;
	while ((c = rc->parentClass())) rc = c;
	return rc;
    }

    /** Give the full name as known to C++.  This includes
     *	template suffixes (for templates or instantiated classes).
     *
     *  vector<int>
     *  vector<class T>
     */
    const char* fullName() {
	if (!_cachedFullName) _cachedFullName = computeFullName();
	return _cachedFullName;
    }

    ClassJar* derived();

    ClassJar* depends();
    StringPool* packageDepends();

    /** Give the bare name (no template suffixes)
     *
     * vector
     */
    const char* name() { return type()->objname; }
    const char* packageName();
    const char* dottedPackageName();
    const char* slashedPackageName();

    void print();
    void printAll();

    /** Check several things:
     *
     *  1. If a method is declared in a derived class, but not explicitly
     *  virtual, but in reality it is (in the base class), make sure
     *  we know it is virtual.
     * 
     *  2. Make sure enumerated types are correctly identified (they
     *  will get parsed as objects).
     */
    static void make_consistent(ClassJar* jar);
    static CType* subst_enum(CType* t, Class* c, ClassJar* jar);
    static void subst_typedef_fct_args(CType* t, Class* c);
    static CType* subst_typedef(CType* t, Class* c, int clone = 0);

    void expand(Arg* args);

    void setType(ObjectCType* t);

    ObjectCType* type() { return _type; }

    void setParameters(Arg* p) {
	_parameters = p;
	_cachedFullName = NULL;
    }

    Arg* parameters() { return _parameters; }

    ObjectCType* parent;
    Method* methods;
    TypeMap typedefs;
    TypeMap callbacks;

    static void setPackagePrefix(const char* pfx);
    int cached() const { return _cached; }
    int next_serial() { return _serial++; }

    int writeTo(FILE* f);
    static Class* readFrom(FILE *f);

protected:
    Class(const Token& t);

    ObjectCType* _type;
    Arg* _parameters;
    const char* computeFullName();
    const char* _cachedFullName;
    const char* _packageName;
    static const char* _packagePrefix;
    int _cached;
    int _serial;
};

#endif // __Class_h_
