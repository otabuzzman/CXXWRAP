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
 * $Id: FunctionCType.cxx,v 1.7 2006/12/17 04:08:11 deaven Exp $
 */

#include "Class.h"
#include "FunctionCType.h"
#include <string.h>
#include <ctype.h>

int
FunctionCType::writeTo(FILE* f)
{
    CType::writeTo(f);
    writeTokenTo(rtype, f);
    writeTokenTo(args, f);
    return 0;
}

FunctionCType*
FunctionCType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    FunctionCType* a = new FunctionCType(*t);
    delete t;
    a->rtype = (CType*) readTokenFrom(f);
    a->args = (Arg*) readTokenFrom(f);
    return a;
}

const char*
FunctionCType::JNICast(const char* expr)
{
    char buf[CODE_BUFFER_SIZE];
    sprintf(buf, "(__cbd == NULL) ? NULL : &__%s__callback", alias);

    return global.pool.add(buf);
}

const char*
FunctionCType::JNITypeName()
{
    if (can_wrap)
    {
        return "jobject";
    }
    return CType::JNITypeName();
}

const char*
FunctionCType::JavaTypeName()
{
    if (can_wrap)
    {
        char buf[TOKEN_BUFFER_SIZE];
        sprintf(buf, "%sCallback", alias);
        return global.pool.add(buf);
    }
    return CType::JavaTypeName();
}

const char*
FunctionCType::BareCTypeName()
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s (*)(", rtype->CTypeName());

    Arg* a;
    for (a = args; a; a = a->next)
    {
	strcat(buf, a->type->CTypeName());
	if (a->next) strcat(buf, ",");
    }
    strcat(buf, ")");
    return global.pool.add(buf);
}






