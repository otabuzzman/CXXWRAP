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
 * $Id: CreateCType.h,v 1.7 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __CreateCType__
#define __CreateCType__

#include "CType.h"

/** This class manages the "types" associated with constructors and 
    destructors.
 */
class CreateCType : public CType {
public:
    CreateCType(const char* _comment, int t = M_CTOR, int mods = 0) :
	CType(_comment, mods | t) { }

    virtual CType* clone() { return new CreateCType(*this); }
    virtual ~CreateCType() { }
    virtual const char* classname() const { return "CreateCType"; }

    virtual const char* CTypeName();

    int writeTo(FILE* f);
    static CreateCType* readFrom(FILE* f);

protected:
    CreateCType(const CreateCType& other) : 
	CType(other) { }
    CreateCType(const CType& other) : 
	CType(other) { }
};

#endif // __CreateCType__
