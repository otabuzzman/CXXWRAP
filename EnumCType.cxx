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
 * $Id: EnumCType.cxx,v 1.12 2006/12/17 04:08:11 deaven Exp $
 */

#include "Class.h"
#include "EnumCType.h"
#include <string.h>

int
EnumCType::writeTo(FILE* f)
{
    CType::writeTo(f);
    writeStringTo(parentName, f);
    writeStringTo(name, f);
    writeTokenTo(symbols, f);
    return 0;
}

EnumCType*
EnumCType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    EnumCType* a = new EnumCType(*t);
    delete t;
    a->parentName = readStringFrom(f);
    a->name = readStringFrom(f);
    a->symbols = (Arg*) readTokenFrom(f);
    return a;
}

Class*
EnumCType::parent()
{
    if (!parentClass && parentName)
    {
	parentClass = global.classJar.findClassByName(parentName);
    }
    return parentClass;
}

void EnumCType::setParent(Class* c)
{
    parentClass = c;
    parentName = c->fullName();
}

const char*
EnumCType::DefaultPrivateJavaValue()
{
    return "0";
}

const char*
EnumCType::BareCTypeName()
{
    if (parent())
    {
	char buf[TOKEN_BUFFER_SIZE];
	sprintf(buf, "%s::%s", parent()->fullName(), name);
	return global.pool.add(buf);
    }
    return name;
}

const char*
EnumCType::JavaTypeName()
{
    if (indirection_count > 1) return NULL;
    return "int";
}

const char*
EnumCType::JNITypeName()
{
    if (indirection_count > 1) return NULL;
    return "jint";
}

const char*
EnumCType::JNICast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "(%s) %s", BareCTypeName(), expr);
    return global.pool.add(buf);
}

