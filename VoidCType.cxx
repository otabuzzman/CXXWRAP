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
 * $Id: VoidCType.cxx,v 1.16 2006/12/17 04:08:11 deaven Exp $
 */

#include "defs.h"
#include "VoidCType.h"

int
VoidCType::writeTo(FILE* f)
{
    CType::writeTo(f);
    return 0;
}

VoidCType*
VoidCType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    VoidCType* a = new VoidCType(*t);
    delete t;
    return a;
}

const char*
VoidCType::BareCTypeName()
{
    return "void";
}

const char*
VoidCType::JavaTypeName()
{
    if (indirection_count > 1) return NULL;
    if ((modifiers & M_POINTER) || (modifiers & M_REFERENCE))
    {
	return CType::JavaTypeName();
    }

    return global.pool.add("void");
}

const char*
VoidCType::JNITypeName()
{
    return JavaTypeName();
}

int
VoidCType::is_void()
{
    return 1;
}

const char*
VoidCType::JNIFunctionTypeName()
{
   if (indirection_count > 0)
   {
       // Java does not have the concept of void pointers
       return NULL;
   }
   return "Void";
}

const char*
VoidCType::JavaTypeID()
{
    return "V";
}
