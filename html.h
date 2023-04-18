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
 * $Id: html.h,v 1.10 2006/12/17 04:08:11 deaven Exp $
 */

#include "Class.h"
#include "Emitter.h"

class HTMLEmitter : public Emitter {
public:
    HTMLEmitter(ClassJar* jar_, const char* path_);
    ~HTMLEmitter();

    int emit(int argc, char** argv);

protected:
    const char* path;

    virtual void start_html(FILE* f, const char* title);
    virtual void end_html(FILE* f);
    virtual void emit_frame_index(FILE* f, const char* title);
    virtual void open_section(FILE*f, const char* title);
    virtual void open_section_line(FILE* f, const char* color = NULL);
    virtual void close_section_line(FILE* f);
    virtual void close_section(FILE* f);
    virtual const char* emit_link_text(FILE* f, const char* text);
    virtual void emit_class_link(FILE* f, const char* link, const char* clazz);
    virtual const char* emit_comment_word(FILE* f, const char* first, const char* p);
    virtual void emit_gathered_tags(FILE* f, CommentTag* tags,
	 const char* tag, const char* title);
    virtual void emit_sees(FILE* f, CommentTag* tags);
    virtual void emit_params(FILE* f, CommentTag* tags);
    virtual void emit_comment(FILE* f, const char* comment);
    virtual void emit_html_spaces(FILE*f, int len);
    virtual void emit_method(FILE* fout, Class* c, Method* m);
    virtual void emit_method_index_item(FILE* fout, Method* m);
    virtual int emit_method_summary(FILE* fout,
	Class* c, Method** mtab, int count);
    virtual int emit_html_for_one(Class* c);
    virtual void emit_package_summary_file(FILE* f, const char* p);
    virtual void emit_package_index();
    virtual int emit_class_diagram(ClassJar* roots);
    virtual void emit_package_summary(FILE* f,
	const char* hf, Class** ctab, int class_count);
    const char* html_sub(const char* in);

private:
    int autolink;
};

class PrintableHTMLEmitter : public HTMLEmitter {
public:
    PrintableHTMLEmitter(ClassJar* jar_, const char* path_);
    ~PrintableHTMLEmitter();

protected:
    virtual void start_html(FILE* f, const char* title);
    virtual void end_html(FILE* f);
    virtual void open_section(FILE*f, const char* title);
    virtual void open_section_line(FILE* f, const char* color = NULL);
    virtual void close_section_line(FILE* f);
    virtual void close_section(FILE* f);
    virtual int emit_method_summary(FILE* fout,
	Class* c, Method** mtab, int count);
};

