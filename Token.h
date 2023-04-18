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
 * $Id: Token.h,v 1.8 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __Token__
#define __Token__

#include <stdio.h>

class Token {
public:
    Token(const char* s = NULL) : comment(s), nmspace(NULL) { }
    Token(const Token& other) : comment(other.comment), nmspace(other.nmspace) { }
    virtual ~Token() { }
    virtual const char* classname() const { return "Token"; }
    const char* comment;
    const char* nmspace;

    virtual int writeTo(FILE* f);
    static Token* readFrom(FILE* f);

    static int writeStringTo(const char* s, FILE* f);
    static const char* readStringFrom(FILE* f);
    static int writeIntTo(int i, FILE* f);
    static int readIntFrom(FILE* f);
    static int writeTokenTo(Token* t, FILE* f);
    static Token* readTokenFrom(FILE* f);

    static int cache_comments;
};

#endif // __Token__
