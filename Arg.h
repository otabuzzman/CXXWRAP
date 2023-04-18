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
 * $Id: Arg.h,v 1.3 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __Arg__
#define __Arg__

#include "CType.h"
#include "Token.h"

class Arg : public Token {
public:
    Arg(const char* _comment, const char* name_, CType* type_) :
	Token(_comment),
	name(name_), default_value(NULL), type(type_), next(NULL) { }
    Arg(const Arg& other) :
	Token(other),
	name(other.name),
	default_value(other.default_value),
	type(other.type->clone()),
	next(NULL) { }
    ~Arg()
	{ delete type; delete next; }
    virtual const char* classname() const { return "Arg"; }
    Arg* clone();
    
    int equals(Arg* other) { return type->equals(other->type); }
    void print();

    const char* name;
    const char* default_value;
    CType* type;
    Arg* next;

    int writeTo(FILE* f);
    static Arg* readFrom(FILE* f);

protected:
    Arg(const Token& other) :
	Token(other),
	name(NULL),
	default_value(NULL),
	type(NULL),
	next(NULL) { }
};

#endif // __Arg__
