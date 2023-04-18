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
 * $Id: Method.cxx,v 1.6 2006/12/17 04:08:11 deaven Exp $
 */

#include <string.h>
#include <ctype.h>
#include "Method.h"

int Method::writeTo(FILE* f)
{
    Token::writeTo(f);
    writeStringTo(name, f);
    writeTokenTo(args, f);
    writeTokenTo(type, f);
    writeIntTo(is_pure, f);
    writeIntTo(is_const, f);
    writeTokenTo(next, f);
    writeIntTo(serial, f);
    writeIntTo(notes, f);
    return 0;
}

Method* Method::readFrom(FILE* f)
{
    Token* t = Token::readFrom(f);
    Method* m = new Method(*t);
    delete t;
    m->name = readStringFrom(f);
    m->args = (Arg*) readTokenFrom(f);
    m->type = (CType*) readTokenFrom(f);
    m->is_pure = readIntFrom(f);
    m->is_const = readIntFrom(f);
    m->next = (Method*) readTokenFrom(f);
    m->serial = readIntFrom(f);
    m->notes = readIntFrom(f);
    return m;
}

Method* Method::clone()
{
    Method* result = NULL;

    Method* mlast = NULL;
    Method* m;
    for (m = this; m; m = m->next)
    {
	Method* nm = new Method(*m);
	if (mlast)
	{
	    mlast->next = nm;
	}
	else
	{
	    result = nm;
	}
	mlast = nm;
    }

    return result;
}

const char*
Method::getName()
{
    if (is_operator)
    {
        char buf[TOKEN_BUFFER_SIZE];

        if (!strcmp(name,"opCast")) sprintf(buf, "operator %s", type->CTypeName());
        else sprintf(buf, "operator%s", name+2);
        
        return global.pool.add(buf);
    }
    return name;
}

const char*
Method::getTextName()
{
    String s(name);
    s.replace('+', "Add");
    s.replace('-', "Sub");
    s.replace('*', "Mul");
    s.replace('/', "Div");
    s.replace('%', "Mod");
    s.replace('&', "And");
    s.replace('|', "Or");
    s.replace('^', "Xor");
    s.replace('~', "OneComp");
    s.replace('<', "Lt");
    s.replace('>', "Gt");
    s.replace('!', "Not");
    s.replace('=', "Eq");
    s.replace('[', "Array");
    s.replace(']', "");

    return s.val;
}

int Method::hasSameSignature(Method* other)
{
    if (name != other->name) return 0;

    Arg* a = args;
    Arg* oa = other->args;

    while (a)
    {
	if (!oa) return 0;
	if (!(a->equals(oa))) return 0;
	a = a->next;
	oa = oa->next;
    }

    if (oa) return 0;

    return 1;
}
