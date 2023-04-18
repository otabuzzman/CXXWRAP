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
 * $Id: EnumCType.h,v 1.13 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __EnumCType__
#define __EnumCType__

#include "defs.h"
#include "CType.h"

class EnumCType : public CType {
public:

    EnumCType(const char* _comment, const char* _name, Arg* _symbols) :
	CType(_comment, M_ENUM),
	parentName(NULL), parentClass(NULL),
	name(_name),
	symbols(_symbols) { }
    virtual CType* clone() { return new EnumCType(*this); }
    virtual ~EnumCType() { }
    virtual const char* classname() const { return "EnumCType"; }

    const char* DefaultPrivateJavaValue();
    const char* BareCTypeName();
    const char* JNITypeName();
    const char* JavaTypeName();
    const char* JNICast(const char* expr);

    Arg* getSymbols() const { return symbols; }
    const char* getName() const { return name; }
    Class* parent();
    void setParent(Class* c);

    int writeTo(FILE* f);
    static EnumCType* readFrom(FILE* f);

private:
    const char* parentName;
    Class* parentClass;
    const char* name;
    Arg* symbols;
    EnumCType(const EnumCType& other) :
	CType(other), parentName(other.parentName),
	parentClass(other.parentClass), name(other.name),
	symbols(other.symbols) { }
    EnumCType(const CType& other) :
	CType(other), parentName(NULL), parentClass(NULL),
	name(NULL), symbols(NULL) { }
};

#endif // __EnumCType__
