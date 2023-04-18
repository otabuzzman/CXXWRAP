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
 * $Id: Emitter.h,v 1.9 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __Emitter__
#define __Emitter__

#include "defs.h"
#include "CommentParser.h"

class ClassJar;
class Class;

class Emitter {
public:
    Emitter(ClassJar* jar_);
    virtual ~Emitter();
    virtual int emit(int argc, char** argv) = 0;

protected:
    virtual int emit_tree(FILE* f, Class* c);
    virtual int emit_sub_tree(FILE* f, Class* c);
	virtual FILE* open_file(const char* path, const char* mode);
    virtual int make_path(const char* path);
    virtual int is_html_tag(const char* tag);
    virtual const char* emit_comment_word(FILE* f, const char* start, const char* p);
    virtual void emit_comment_text(FILE* f, const char* text);
    virtual void emit_class_link(FILE* f, const char* link, const char* clazz);
    virtual CommentParser* comment_parser(const char* comment);
    virtual void fill_ctab();
    Class** sorted_class_list(ClassJar* jar, int* len);

    StringPool pool;
    Class** ctab;
    int class_count;
    ClassJar* jar;

private:
    void emit_st(FILE* f, int indent, Class* c);
};

#endif // __Emitter__
