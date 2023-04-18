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
 * $Id: FunctionCType.h,v 1.5 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __FunctionCType__
#define __FunctionCType__

#include "CType.h"

class FunctionCType : public CType {
public:
    FunctionCType(CType* rtype_, Arg* args_) :
	CType(*rtype_), rtype(rtype_), args(args_), can_wrap(false) { }
    FunctionCType(const FunctionCType& other) :
	CType(other), rtype(other.rtype->clone()),
	args(other.args ? other.args->clone() : NULL), can_wrap(other.can_wrap) { }
    virtual CType* clone() { return new FunctionCType(*this); }
    virtual ~FunctionCType() { }
    virtual const char* classname() const { return "FunctionCType"; }

    const char* JNICast(const char* expr);
    const char* JNITypeName();
    const char* JavaTypeName();
    const char* BareCTypeName();

    int writeTo(FILE* f);
    static FunctionCType* readFrom(FILE* f);

    CType* rtype; // return type
    Arg* args; // function arguments
    int can_wrap; // can we wrap this (requires a void* in arg list)?

private:
    FunctionCType(const CType& other) :
	CType(other), rtype(NULL), args(NULL) { }
};

#endif // __FunctionCType__
