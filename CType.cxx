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
 * $Id: CType.cxx,v 1.26 2006/12/17 04:08:11 deaven Exp $
 */

#include "defs.h"
#include <string.h>

int CType::writeTo(FILE* f)
{
    int s = Token::writeTo(f);
    int hi = (modifiers >> 16) & 0xffff;
    int lo = modifiers & 0xffff;
    s |= writeIntTo(lo, f);
    s |= writeIntTo(hi, f);
    s |= writeIntTo(indirection_count, f);
    s |= writeStringTo(alias, f);
    return s;
}

CType* CType::readFrom(FILE* f)
{
    Token* t = Token::readFrom(f);
    CType* ct = new CType(*t);
    delete t;
    int lo = readIntFrom(f);
    int hi = readIntFrom(f);
    ct->modifiers = lo | (hi << 16);
    ct->indirection_count = readIntFrom(f);
    ct->alias = readStringFrom(f);
    return ct;
}

CType* CType::expand(Arg* /* parameters */, Arg* /* args */)
{
    // by default just a shallow copy
    return this;
}

int CType::equals(CType* other)
{
    if (strcmp(CTypeName(), other->CTypeName())) return 0;
    return 1;
}

const char*
CType::JNICast(const char* expr)
{
    return expr;
}

const char*
CType::CBCast(const char* expr)
{
    return expr;
}

const char*
CType::JavaTypeID()
{
    return "";
}

const char*
CType::JNIUpCast(const char* expr)
{
    return JNICast(expr);
}

const char*
CType::CBUpCast(const char* expr)
{
    return "";
}

const char*
CType::JNIInvokeUpCast(const char* expr)
{
    return JNIUpCast(expr);
}

const char*
CType::JavaCast(const char* expr)
{
    return expr;
}

const char*
CType::JavaUpCast(const char* expr)
{
    return JavaCast(expr);
}

const char*
CType::OperatorSuffix()
{
    return "";
}

const char*
CType::JavaTypeName()
{
    return NULL;
}

const char*
CType::PrivateJavaTypeName()
{
    return JavaTypeName();
}

const char*
CType::DefaultPrivateJavaValue()
{
    return "null";
}

const char*
CType::JNIBegin(const char* /* expr */)
{
    return "";
}

const char*
CType::JNIEnd(const char* /* expr */)
{
    return "";
}

const char*
CType::CBBegin(const char* /* expr */)
{
    return "";
}

const char*
CType::CBEnd(const char* /* expr */)
{
    return "";
}

const char*
CType::JNIInvokeBegin(const char* expr)
{
    return "";
}

const char*
CType::JNIInvokeEnd(const char* expr)
{
    return "";
}

const char*
CType::JNITypeName()
{
    return NULL;
}

const char*
CType::JavaJNITypeName()
{
    return "";
}

const char* 
CType::JNIFunctionTypeName()
{
    return "";
}

const char*
CType::CTypeName()
{
    char buf[TOKEN_BUFFER_SIZE];
    strcpy(buf, CTypeNamePrefix());
    strcat(buf, BareCTypeName());
    strcat(buf, CTypeNameSuffix());
    return global.pool.add(buf);
}

const char* 
CType::CTypeNamePrefix()
{
    char buf[TOKEN_BUFFER_SIZE];
    buf[0] = '\0';

    if (modifiers & M_CONST) strcat(buf, "const ");
    if (modifiers & M_STATIC) strcat(buf, "static ");
    if (modifiers & M_VIRTUAL) strcat(buf, "virtual ");

    return global.pool.add(buf);
}

const char* 
CType::BareCTypeName()
{
    return "";
}

const char* 
CType::CTypeNameSuffix()
{
    char buf[TOKEN_BUFFER_SIZE];
    buf[0] = '\0';

    int indir_count_temp = indirection_count;
    if (modifiers & M_POINTER)
    {
	/* If this is a reference to a pointer, temporarily decrease
	 * the indirection count, so as to not output to many
	 * asterisks.
	 */
	if (modifiers & M_REFERENCE) indir_count_temp--;
	int i;
	for (i = 0; i < indir_count_temp; i++)
	{
	    strcat(buf, "*");
	}
    }
    if (modifiers & M_REFERENCE) strcat(buf, "&");
    if (modifiers & M_TEMPLATE) strcat(buf, "{template}");

    return global.pool.add(buf);
}

void
CType::print()
{
    printf("CTypeName=%s JavaTypeName=%s",
	CTypeName(), JavaTypeName());
    fflush(stdout);
}

int
CType::is_void()
{
    return 0;
}

int
CType::is_object()
{
    return 0;
}
