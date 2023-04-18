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
 * $Id: Method.h,v 1.12 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __Method__
#define __Method__

#include "defs.h"
#include "Arg.h"

class Method : public Token {
public:
    enum Notes {
	// suppress inclusion of jni-{begin,end}-hook files if specified
	NO_JNI_HOOK = (1 << 0),

	// is actually an enumeration definition
        ENUM_DEFINE = (1 << 1)
    };

    Method(const char* _comment, const char* name_, Arg* args_) :
	Token(_comment),
	name(name_),
	args(args_),
	type(NULL),
	is_pure(0),
	is_const(0),
        is_operator(0),
        register_callback(false),
	next(NULL),
	serial(0),
	notes(0)
	{ }
    Method(const Method& other) :
	Token(other),
	name(other.name),
	args(other.args ? other.args->clone() : NULL),
	type(other.type->clone()),
	is_pure(other.is_pure),
	is_const(other.is_const),
        is_operator(other.is_operator),
        register_callback(other.register_callback),
	next(NULL),
	serial(other.serial),
	notes(other.notes)
	{ }
    ~Method() { delete next; }
    virtual const char* classname() const { return "Method"; }
    Method* clone();

    const char* getName();
    const char* getTextName();
    int hasSameSignature(Method* other);

    const char* name;
    Arg* args;
    CType* type;
    int is_pure;
    int is_const;
    int is_operator;
    bool register_callback;
    Method* next;
    int serial;
    /* set of notes, represented as bit settings */
    int notes;

    int writeTo(FILE* f);
    static Method* readFrom(FILE* f);

private:
    Method(const Token& other) :
	Token(other),
	name(NULL),
	args(NULL),
	type(NULL),
	is_pure(0),
	is_const(0),
        is_operator(0),
	next(NULL),
	serial(0),
	notes(0)
	{ }
};

#endif // __Method__
