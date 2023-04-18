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
 * $Id: Token.cxx,v 1.10 2006/12/17 04:08:11 deaven Exp $
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "defs.h"
#include "Token.h"
#include "Class.h"
#include "CreateCType.h"
#include "EnumCType.h"
#include "ArrayCType.h"
#include "FunctionCType.h"
#include "PrimitiveCType.h"
#include "VoidCType.h"
#include "JNICType.h"

#define DEBUG_SERIALIZATION 0

int Token::cache_comments = 0;
#if DEBUG_SERIALIZATION
static FILE* flog = NULL;
class bootstrap {
public:
    bootstrap() { flog = fopen("cxxwrap.sdb", "w"); }
};
static bootstrap bs;
#endif

int Token::writeTokenTo(Token* t, FILE* f)
{
    int r;
#if DEBUG_SERIALIZATION
//    fprintf(flog, "token<%s>\n", t && t->classname() ? t->classname() : "null");
#endif
    if (t)
    {
	r = writeStringTo(t->classname(), f);
	r |= t->writeTo(f);
    }
    else
    {
	r = writeStringTo("NULL", f);
    }

    return r;
}

Token* Token::readTokenFrom(FILE* f)
{
    const char* type = readStringFrom(f);
#if DEBUG_SERIALIZATION
//    fprintf(flog, "token<%s>\n", type ? type : "null");
#endif

    if (!type || !strcmp(type, "NULL"))
    {
	return NULL;
    }

#define FACTORY(NAME) \
    if (!strcmp(type, #NAME)) { return NAME::readFrom(f); }

    FACTORY(Token)
    FACTORY(Arg)
    FACTORY(Class)
    FACTORY(Method)
    FACTORY(CType)
    FACTORY(ObjectCType)
    FACTORY(CreateCType)
    FACTORY(EnumCType)
    FACTORY(ArrayCType)
    FACTORY(FunctionCType)
    FACTORY(PrimitiveCType)
    FACTORY(VoidCType)
    FACTORY(JNICType)

#undef FACTORY
#define MAX_ERR_MESSAGE 30
    static char *msg = "...(truncated)";
    if (strlen(type) > (MAX_ERR_MESSAGE - strlen(msg) + 1))
    {
	strcpy((char*) type + MAX_ERR_MESSAGE - strlen(msg), msg);
    }
    printf("Internal error reading object \"%s\" from cache\n", type);
    return NULL;
}

int Token::writeTo(FILE* f)
{
    int r = writeStringTo(cache_comments ? comment : NULL, f);
    r |= writeStringTo(nmspace, f);
    return r;
}

Token* Token::readFrom(FILE* f)
{
    const char* comment = readStringFrom(f);
    Token* t = new Token(comment);
    t->nmspace = readStringFrom(f);
    return t;
}

int Token::writeStringTo(const char* s, FILE* f)
{
    int len = s ? strlen(s) : 0;
    writeIntTo(len, f);
    if (len > 0)
    {
	if (len != (int) fwrite(s, 1, len, f))
	{
	    fprintf(stderr, "error writing string to cache\n");
	    exit(-1);
	}
    }
#if DEBUG_SERIALIZATION
    fprintf(flog, "string<%s>\n", s ? s : "null");
#endif
    return 0;
}

const char* Token::readStringFrom(FILE* f)
{
    const char* result = NULL;
    int len = readIntFrom(f);
    if (len > 0)
    {
	char* s = new char[len + 1];
	if (len != (int) fread(s, 1, len, f))
	{
	    fprintf(stderr, "error reading string from cache\n");
	    exit(-1);
	}
	s[len] = '\0';
	result = global.pool.add(s);
	delete[] s;
    }
#if DEBUG_SERIALIZATION
    fprintf(flog, "string<%s>\n", result ? result : "null");
#endif
    return result;
}

int Token::writeIntTo(int i, FILE* f)
{
    unsigned char buf[2];
    buf[0] = i & 0xff;
    buf[1] = (i >> 8) & 0xff;
    if (1 != fwrite(buf, 2, 1, f))
    {
	fprintf(stderr, "error writing int to cache\n");
	exit(-1);
    }
#if DEBUG_SERIALIZATION
    fprintf(flog, "int<%d %d %d>\n", i, buf[0], buf[1]);
#endif
    return 0;
}

int Token::readIntFrom(FILE* f)
{
    unsigned char buf[2];
    if (1 != fread(buf, 2, 1, f))
    {
	fprintf(stderr, "error reading int from cache\n");
	exit(-1);
    }
    int r = (buf[0] | (buf[1] << 8));
#if DEBUG_SERIALIZATION
    fprintf(flog, "int<%d %d %d>\n", r, buf[0], buf[1]);
#endif
    return r;
}
