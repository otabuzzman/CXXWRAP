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
 * $Id: JNICType.h,v 1.2 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __JNICType__
#define __JNICType__

#include "CType.h"

class JNICType : public CType {
public:
    enum Type {
	T_ENV,
	T_OBJECT
    };

    JNICType(const char* _comment, Type ty) :
	CType(_comment), type(ty) { }
    JNICType(CType* other, Type ty) :
	CType(*other), type(ty) { }

    virtual CType* clone() { return new JNICType(*this); }
    virtual ~JNICType() { }
    virtual const char* classname() const { return "JNICType"; }

    const char* JavaTypeName();
    const char* DefaultPrivateJavaValue();
    const char* JNITypeName();

    virtual const char* BareCTypeName();

    Type type;

    int writeTo(FILE* f);
    static JNICType* readFrom(FILE* f);

private:
    JNICType(const JNICType& other) : 
	CType(other), type(other.type) { }
};

#endif // __JNICType__
