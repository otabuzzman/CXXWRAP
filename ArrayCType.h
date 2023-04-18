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
 * $Id: ArrayCType.h,v 1.7 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __ArrayCType__
#define __ArrayCType__

#include "CType.h"

class ArrayCType : public CType {
public:
    ArrayCType(CType* base_, int len_ = -1) :
	CType(base_->comment), base(base_->clone()), len(len_)
	{
	    indirection_count = base->indirection_count + 1;
	}
    virtual CType* clone() { return new ArrayCType(*this); }
    virtual ~ArrayCType() { }
    virtual const char* classname() const { return "ArrayCType"; }

    const char* JavaTypeName();
    const char* JNITypeName();
    const char* CTypeName();
    const char* JNICast(const char* expr);
    const char* JNIUpCast(const char* expr);

    const char* JNIBegin(const char* expr);
    const char* JNIEnd(const char* expr);

    CType* base;
    int len;

    int writeTo(FILE* f);
    static ArrayCType* readFrom(FILE* f);

protected:
    ArrayCType(const ArrayCType& other) :
	CType(other), base(other.base->clone()), len(other.len) { }
    ArrayCType(const CType& other) :
	CType(other), base(NULL), len(0) { }
};

#endif // __CType__
