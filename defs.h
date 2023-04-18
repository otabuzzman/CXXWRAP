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
 * $Id: defs.h,v 1.29 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __defs_h__
#define __defs_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringPool.h"
#include "CType.h"
#include "globals.h"

#ifdef WIN32
#define PATHSEP '\\'
#define PATHSEPSTR "\\"
#else
#define PATHSEP '/'
#define PATHSEPSTR "/"
#endif

class String : public Token {
public:
    String(const char* _comment, const char* s) :
	Token(_comment),
	val(global.pool.add(s)) { }
    String(const char* s) :
	Token(),
	val(global.pool.add(s)) { }

    /* Replace all occurrences of char 'c' with string 's' in the
     * string contained in this object.
     */
    void replace(const char c, const char* s)
    {
        char buf[TOKEN_BUFFER_SIZE];
        strcpy(buf, val);
        int l = strlen(s);

        char* p;
        while ((p = strchr(buf, c)))
        {
            memmove(p+l, p+1, strlen(p+1)+1);
            memcpy(p, s, l);
        }
        val = global.pool.add(buf);
    }

    const char* val;
};

class Number : public Token {
public:
    Number(const char* _comment, double d, const char* sv = NULL) :
	Token(_comment),
	strval(global.pool.add(sv)), val(d) { }
    const char* strval;
    double val;
};

int scan_cxx_file(const char* root, const char* fn, int trace);
const char* argval(const char* s, char* arg);
const char* argval_in(const char* s, int argc, char** argv);
int arg_in(const char* s, int argc, char** argv);
const char* read_file(const char* fn);
void set_warning_context(const char* where);
void warning(const char* msg);
int error_summary();
#ifdef WIN32
int strncasecmp(const char* s1, const char* s2, int len);
#endif

#endif // __defs_h__
