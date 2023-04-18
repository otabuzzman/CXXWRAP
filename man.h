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
 * $Id: man.h,v 1.4 2006/12/17 04:08:11 deaven Exp $
 */

#include "Emitter.h"

class ManualEmitter : public Emitter {
public:
    ManualEmitter(ClassJar* jar_, const char* path_);
    ~ManualEmitter();

    int emit(int argc, char** argv);

protected:
    const char* path;

    virtual void start_page(FILE* f, const char* title);
    virtual void start_section(FILE* f, const char* title);
    virtual const char* emit_link_text(FILE* f, const char* text);
    virtual const char* emit_comment_word(FILE* f, const char* first, const char* p);
    virtual void emit_gathered_tags(FILE* f, CommentTag* tags,
	 const char* tag, const char* title);
    virtual void emit_sees(FILE* f, CommentTag* tags);
    virtual void emit_params(FILE* f, CommentTag* tags);
    virtual void emit_comment(FILE* f, const char* comment);
    virtual void emit_method(FILE* fout, Method* m);
    virtual int emit_page_for_one(Class* c, const char* path);
};

