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
 * $Id: Emitter.cxx,v 1.16 2006/12/17 04:08:11 deaven Exp $
 */

#include <ctype.h>
#include "Class.h"
#include "ClassJar.h"
#include "Emitter.h"

Emitter::Emitter(ClassJar* jar_) :
    ctab(NULL),
    jar(jar_)
{
    fill_ctab();
}

Emitter::~Emitter()
{
    delete[] ctab;
}

void Emitter::fill_ctab()
{
    if (ctab) delete[] ctab;

    class_count = jar->size();

    ctab = new Class*[class_count];
    class_count = 0;
    ClassJarIterator iter(jar->iterator());
    Class *c;
    while ((c = iter.next()))
    {
	ctab[class_count++] = c;
    }
}

static void pathfix(char * buf)
{
    char* p;
    for (p = buf; *p != '\0'; p++)
    {
	if ((*p) == '\\' || (*p) == '/')
	{
	    *p = PATHSEP;
	}
    }
}

static int cmp(Class** pa, Class** pb)
{
    Class *a = *pa;
    Class *b = *pb;
    return strcmp(a->fullName(), b->fullName());
}

Class** Emitter::sorted_class_list(ClassJar* jar, int* len)
{
    ClassJarIterator iter(jar->iterator());
    *len = jar->size();
    Class** ctab = new Class*[(*len)];

    Class* c;
    int i = 0;
    while ((c = iter.next()))
    {
	ctab[i++] = c;
    }

    /* sort so that the classes are alphabetized
     */
    qsort(ctab, (*len), sizeof(Class*),
	(int (*)(const void *, const void *)) cmp);

    return ctab;
}

FILE* Emitter::open_file(const char* path, const char* mode)
{
    char buf[32 + FILENAME_MAX];
    strcpy(buf, path);
    pathfix(buf);
 
    return fopen(buf, mode);
}

int Emitter::make_path(const char* path)
{
    // make sure the path exists
    char buf[32 + FILENAME_MAX];

#ifdef WIN32
    strcpy(buf, path);
    pathfix(buf);

    char* sep = strrchr(buf, PATHSEP);
    if (sep)
    {
	*sep = '\0';
	make_path(buf); // have to ignore errors...
    }
  
    sprintf(buf, "mkdir %s", path);
#else
    sprintf(buf, "mkdir -p %s", path);
#endif
    pathfix(buf);
    return system(buf);
}

void Emitter::emit_class_link(FILE* f, const char* link, const char* clazz)
{
    fprintf(f, "%s (see %s)", link, clazz);
}

int Emitter::emit_tree(FILE* f, Class* c)
{
    Class* p;
    int i, depth;
    const char* unknown_root = NULL;

    p = c;
    depth = 0;
    while (p)
    {
	depth++;
	Class* par = p->parentClass();
	if (par == NULL && p->parent && strlen(p->parent->objname) > 0)
	{
	    unknown_root = p->parent->objname;
	}
	p = par;
    }

    Class** tree = new Class*[depth];
    {
	i = depth;
	p = c;
	while(p)
	{
	    tree[--i] = p;
	    p = p->parentClass();
	}
    }

    static const char* sep = "+---";
    static const char* spa = "    ";
    char buf[TOKEN_BUFFER_SIZE];

    if (unknown_root)
    {
	fprintf(f, "%s <font size=\"-1\">(not documented)</font>\n|\n%s",
	    unknown_root, sep);
    }
    for (i = 0; i < depth; i++)
    {
	int j;
	for (j = 0; j < i; j++) fprintf(f, "%s", spa);
	if (i > 0 || !unknown_root)
	{
	    fprintf(f, "%s", i == 0 ? spa : sep);
	}

	const char* pn = tree[i]->dottedPackageName();
	const char* tn = tree[i]->name();
	if (pn && (*pn) != '\0')
	{
	    sprintf(buf, "%s.%s", tree[i]->dottedPackageName(), tree[i]->name());
	    tn = buf;
	}
	emit_class_link(f, tn, tree[i]->fullName());

	if (i < (depth - 1))
	{
	    fprintf(f, "\n");
	    for (j = 0; j < i; j++) fprintf(f, "%s", spa);
	    fprintf(f, "%s|\n", spa);
	}
    }

    delete[] tree;

    return 0;
}

void Emitter::emit_st(FILE* f, int indent, Class* c)
{
    int i;
    for (i = 0; i < indent - 2; i++)
    {
	fprintf(f, "%s", ((i - 2) % 4 == 0)  ? "|" : " ");
    }
    if (indent > 0) fprintf(f, "+-");

    emit_class_link(f, c->fullName(), c->name());
    fprintf(f, "\n");

    ClassJar* djar = c->derived();
    if (djar->size() > 0)
    {
	int len;
	Class** ctab = sorted_class_list(djar, &len);
	Class* dc;
	for (i = 0; i < len; i++)
	{
	    dc = ctab[i];
	    emit_st(f, indent + 4, dc);
	}
	delete[] ctab;
    }
    delete djar;
}

int Emitter::emit_sub_tree(FILE* f, Class* c)
{
    /* Check for an undocumented parent.
     */
    if (c->parent)
    {
	fprintf(f, "%s\n", c->parent->objname);
	emit_st(f, 4, c);
    }
    else
    {
	emit_st(f, 0, c);
    }
    return 0;
}

CommentParser* Emitter::comment_parser(const char* comment)
{
    CommentParser* cp;
    if (comment[0] == '*')
    {
	cp = new JavadocCommentParser(comment + 1);
    }
    else
    {
	cp = new CxxCommentParser(comment);
    }

    return cp;
}

int Emitter::is_html_tag(const char* tag)
{
    /* These are HTML tags which we pass directly through unchanged,
     * because they are not required to be matched with an ending.
     */
    static char* passed_tags[] = {
	"p", "li", "dt", "dd", "td", "tr", "br", "hr", "img",
	NULL
    };

    char buf[TOKEN_BUFFER_SIZE];
    const char* p;

    /* ensure the ending bracket.
     */
    const char* end = strchr(tag, '>');
    if (end == NULL) return 0;

    /* if there's a slash this is an ending tag.
     */
    for (p = tag + 1; isspace(*p); p++);
    if (*p == '/') return 1;

    /* look for an explicitly passed tag
     */
    char** pp;
    for (pp = passed_tags; *pp != NULL; pp++)
    {
	int pplen = strlen(*pp);
	if (strncasecmp(*pp, p, pplen) == 0)
	{
	    /* check to make sure a space or '>' follows
	     */
	    if (isspace(p[pplen]) || p[pplen] == '>')
	    {
		return 1;
	    }
	}
    }

    /* make up a "/$TAG" string and look for it.
     */
    int result = 0;
    buf[0] = '/';
    strncpy(buf + 1, p, TOKEN_BUFFER_SIZE - 1);
    {
	char* s;
	for (s = buf + 1; *s != '>' && !isspace(*s); s++);
	*s = '\0';
    }

    if (strstr(end + 1, buf))
    {
	result = 1;
    }

    return result;
}

void Emitter::emit_comment_text(FILE* f, const char* text)
{
    const char* p = text;
    while (p && *p != '\0')
    {
	p = emit_comment_word(f, text, p);
	p++;
    }
}

const char*
Emitter::emit_comment_word(FILE* f, const char* /* first */, const char* p)
{
    fputc((int) *p, f);
    return p;
}

