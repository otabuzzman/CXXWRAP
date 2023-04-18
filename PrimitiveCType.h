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
 * $Id: PrimitiveCType.h,v 1.17 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __PrimitiveCType__
#define __PrimitiveCType__

#include "CType.h"

struct PrimitiveCTypeAssignment
{
    int type;
    const char* name;
    int csize;
    int jsize;
};

class PrimitiveCType : public CType {
public:
    enum Type {
	T_CHAR,
	T_SHORT,
	T_INT,
	T_LONG,
	T_FLOAT,
	T_DOUBLE,
	T_BOOL
    };

    PrimitiveCType(const char* _comment, Type t);

    virtual CType* clone() { return new PrimitiveCType(*this); }
    virtual ~PrimitiveCType() { }
    virtual const char* classname() const { return "PrimitiveCType"; }

    const char* JavaTypeName();
    const char* DefaultPrivateJavaValue();
    const char* JNITypeName();
    const char* JavaJNITypeName();
    const char* JNIFunctionTypeName();

    const char* JNICast(const char* expr);
    const char* JNIUpCast(const char* expr);

    const char* OperatorSuffix();
    
    const char* JNIBegin(const char* expr);
    const char* JNIEnd(const char* expr);

    // Functions dealing with callbacks
    const char* CBCast(const char* expr);
    const char* CBUpCast(const char* expr);
    const char* CBBegin(const char* expr);
    const char* CBEnd(const char* expr);
    const char* JavaTypeID();
    

    virtual const char* BareCTypeName();

    Type type;
    PrimitiveCTypeAssignment* getAssignment();

    int writeTo(FILE* f);
    static PrimitiveCType* readFrom(FILE* f);

private:
    PrimitiveCTypeAssignment* _assignment;
    int is_string();
    int is_wrong_size();
    const char* jni_base_name();
    const char* jni_array_name();
    PrimitiveCType(const PrimitiveCType& other) : 
	CType(other), type(other.type), _assignment(other._assignment) { }
    PrimitiveCType(const CType& other) : 
	CType(other), type(T_CHAR), _assignment(NULL) { }
};

#endif // __PrimitiveCType__
