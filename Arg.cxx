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
 * $Id: Arg.cxx,v 1.4 2006/12/17 04:08:11 deaven Exp $
 */

#include "Arg.h"

int Arg::writeTo(FILE* f)
{
    Token::writeTo(f);
    writeStringTo(name, f);
    writeStringTo(default_value, f);
    writeTokenTo(type, f);
    writeTokenTo(next, f);
    return 0;
}

Arg* Arg::readFrom(FILE* f)
{
    Token* t = Token::readFrom(f);
    Arg* a = new Arg(*t);
    delete t;
    a->name = readStringFrom(f);
    a->default_value = readStringFrom(f);
    a->type = (CType*) readTokenFrom(f);
    a->next = (Arg*) readTokenFrom(f);
    return a;
}

Arg* Arg::clone()
{
    Arg* a;
    Arg* result = NULL;
    Arg* last = NULL;
    for (a = this; a; a = a->next)
    {
	if (!result)
	{
	    result = new Arg(*a);
	    last = result;
	}
	else
	{
	    last->next = new Arg(*a);
	    last = last->next;
	}
    }

    return result;
}

void Arg::print()
{
    printf("{");
    Arg *a;
    for (a = this; a; a = a->next)
    {
	printf("%s ", a->name ? a->name : "(null)");
	a->type->print();
	if (a->next) printf(", ");
    }
    printf("}\n");
}
