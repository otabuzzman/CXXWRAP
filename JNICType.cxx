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
 * $Id: JNICType.cxx,v 1.2 2006/12/17 04:08:11 deaven Exp $
 */

#include <ctype.h>

#include "defs.h"
#include "JNICType.h"
#include <string.h>

int
JNICType::writeTo(FILE* f)
{
    CType::writeTo(f);
    writeIntTo(type, f);
    return 0;
}

JNICType*
JNICType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    Type ty = (Type) readIntFrom(f);
    JNICType* a = new JNICType(t, ty);
    delete t;
    return a;
}

const char*
JNICType::JavaTypeName()
{
    if (type == T_OBJECT) return "Object";
    if (type == T_ENV) return "!JNIEnv";
    return NULL;
}

const char*
JNICType::DefaultPrivateJavaValue()
{
    if (type == T_OBJECT) return "null";
    return "0";
}

const char*
JNICType::JNITypeName()
{
    switch(type) {
	case T_ENV:    return "JNIEnv";
	case T_OBJECT: return "jobject";
    }

    return NULL;
}

const char*
JNICType::BareCTypeName()
{
    return JNITypeName();
}
