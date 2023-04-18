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
 * $Id: CommentParser.h,v 1.5 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __CommentParser__
#define __CommentParser__

#include "defs.h"

class CommentTag {
public:
    CommentTag() : tag(NULL), value(NULL), next(NULL) { }
    CommentTag(const CommentTag* o) : tag(o->tag), value(o->value), next(NULL) { }
    ~CommentTag() { delete next; }
    const char* tag;
    const char* value;
    CommentTag* next;

    CommentTag* findTag(const char* tag);
    CommentTag* enumerateTag(const char* tag);
};

class CommentParser {
public:
    CommentParser(const char* text) : _text(text) { }
    virtual ~CommentParser() { }

    const char* text() const { return _text; }
    virtual const char* summarySentance() = 0;
    virtual const char* body() = 0;
    virtual CommentTag* tags() = 0;

protected:
    const char* trimLeadingStarsAndSpace(const char* text);
    const char* _text;
};

class CxxCommentParser : public CommentParser {
public:
    CxxCommentParser(const char* text);
    ~CxxCommentParser();

    virtual const char* summarySentance();
    virtual const char* body();
    virtual CommentTag* tags();

protected:
    virtual void parse();
    int _parsed;
    char* _allocated;
    const char* _summarySentance;
    const char* _body;
};

class JavadocCommentParser : public CommentParser {
public:
    JavadocCommentParser(const char* text);
    ~JavadocCommentParser();

    virtual const char* summarySentance();
    virtual const char* body();
    virtual CommentTag* tags();

protected:
    void parse();
    void collapseStars(char* text);
    void trim(char* text);
    StringPool _pool;
    const char* _summarySentance;
    const char* _body;
    CommentTag* _tags;
};

#endif // __CommentParser__
