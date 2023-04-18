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
 * $Id: VoidCType.h,v 1.10 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __VoidCType__
#define __VoidCType__

#include "CType.h"

class VoidCType : public CType {
public:

    VoidCType(const char* _comment) :
	CType(_comment) { }

    virtual CType* clone() { return new VoidCType(*this); }
    virtual ~VoidCType() { }
    virtual const char* classname() const { return "VoidCType"; }

    int is_void();

    // Functions dealing with callbacks
    const char* JavaTypeID();
    
    
    const char* BareCTypeName();
    const char* JNITypeName();
    const char* JNIFunctionTypeName();
    const char* JavaTypeName();

    int writeTo(FILE* f);
    static VoidCType* readFrom(FILE* f);

private:
    VoidCType(const VoidCType& other) : 
	CType(other) { }
    VoidCType(const CType& other) : 
	CType(other) { }
};

#endif // __VoidCType__
