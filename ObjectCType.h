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
 * $Id: ObjectCType.h,v 1.22 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __ObjectCType__
#define __ObjectCType__

#include "CType.h"

class ObjectCType : public CType {
public:
    ObjectCType(const char* _comment, const char* objname_,
	const char* source_file_) :
	CType(_comment), objname(objname_),
	source_file(source_file_), args(NULL) { }
    ObjectCType(const ObjectCType& other) :
	CType(other),
	objname(other.objname),
	source_file(other.source_file),
	args(other.args ? other.args->clone() : NULL) { }

    virtual CType* clone() { return new ObjectCType(*this); }
    virtual ~ObjectCType() { delete args; }
    virtual const char* classname() const { return "ObjectCType"; }

    CType* expand(Arg* parameters, Arg* args);

    const char* PrivateJavaTypeName();
    const char* DefaultPrivateJavaValue();
    const char* JNITypeName();
    const char* JavaJNITypeName();
    const char* JNIFunctionTypeName();
    const char* JavaTypeName();
    const char* BareCTypeName();
    const char* JNICast(const char* expr);
    const char* JNIUpCast(const char* expr);
    const char* JNIInvokeUpCast(const char* expr);
    const char* JNIInvokeBegin(const char* expr);
    const char* JavaCast(const char* expr);
    const char* JavaUpCast(const char* expr);
    const char* OperatorSuffix();
    const char* JNIBegin(const char* expr);
    const char* JNIEnd(const char* expr);

    // Functions dealing with callbacks
    const char* CBCast(const char* expr);
    const char* CBUpCast(const char* expr);
    const char* CBBegin(const char* expr);
    const char* CBEnd(const char* expr);
    const char* JavaTypeID();
    
    int is_object();

    const char* scope();
    const char* name();

    const char* objname;
    const char* source_file;
    Arg* args; // template arguments

    int writeTo(FILE* f);
    static ObjectCType* readFrom(FILE* f);

private:
    ObjectCType(const CType& other) :
	CType(other),
	objname(NULL),
	source_file(NULL),
	args(NULL) { }
    const char* nmspaceName();
};

#endif // __ObjectCType__
