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
 * $Id: CommentParser.cxx,v 1.8 2006/12/17 04:08:11 deaven Exp $
 */

#include <string.h>
#include <ctype.h>
#include "CommentParser.h"

// ================================================================

CommentTag* CommentTag::findTag(const char* tag)
{
    CommentTag* t = this;
    while(t)
    {
	if (strcmp(t->tag, tag) == 0) return t;
	t = t->next;
    }
    return NULL;
}

CommentTag* CommentTag::enumerateTag(const char* tag)
{
    CommentTag* t = this;
    CommentTag* result = NULL;
    CommentTag* last = NULL;
    while(t)
    {
	if (strcmp(t->tag, tag) == 0)
	{
	    if (last)
	    {
		last->next = new CommentTag(t);
		last = last->next;
	    }
	    else
	    {
		result = new CommentTag(t);
		last = result;
	    }
	}

	t = t->next;
    }
    return result;
}

// ================================================================

const char*
CommentParser::trimLeadingStarsAndSpace(const char* text)
{
    while (*text != '\0' && isspace(*text)) text++;
    int repeat = *text;
    if (repeat == '*' || repeat == '-' || repeat == '=' || 
	repeat == '#')
    {
	while (*text == repeat) text++;
    }

    return text;
}

// ================================================================

CxxCommentParser::CxxCommentParser(const char* text) :
    CommentParser(text),
    _parsed(0),
    _allocated(NULL),
    _summarySentance(NULL),
    _body(NULL)
{
}

CxxCommentParser::~CxxCommentParser()
{
    if (_allocated)
    {
	delete[] _allocated;
    }
}

void
CxxCommentParser::parse()
{
    // split off text to the first period.
    const char* period = strchr(_text, '.');
    if (period)
    {
	int len = period - _text;
	char* buf = new char[len + 1];
	strncpy(buf, _text, len);
	buf[len] = '\0';
	_summarySentance = buf;
	_allocated = buf;
    }
    else
    {
	_summarySentance = _text;
	_allocated = NULL;
    }
    _summarySentance = trimLeadingStarsAndSpace(_summarySentance);

    // body is all of it
    _body = trimLeadingStarsAndSpace(_text);
}

const char*
CxxCommentParser::summarySentance()
{
    if (!_parsed) parse();
    return _summarySentance;
}

const char*
CxxCommentParser::body()
{
    if (!_parsed) parse();
    return _body;
}

CommentTag*
CxxCommentParser::tags()
{
    return NULL;
}

// ================================================================

static int is_tag(const char* x)
{
    static char* tags[] = {
	"@author", "@version", "@param", "@return",
	"@exception", "@see", "@since", "@serial",
	"@deprecated", "@throws",
	"@header", "@package", "@date",
	NULL
	};

    char** s;
    for (s = tags; *s; s++)
    {
	if (!strncmp(x, *s, strlen(*s))) return 1;
    }

    return 0;
}

/* Search for a '@' that is part of a tag
 */
static char* strchr_tags(char* text)
{
    char* p = strchr(text, '@');
    while (p && !is_tag(p))
    {
	p = strchr(p + 1, '@');
    }
    return p;
}

JavadocCommentParser::JavadocCommentParser(const char* text) :
    CommentParser(text),
    _pool(),
    _summarySentance(NULL),
    _body(NULL),
    _tags(NULL)

{
    parse();
}

JavadocCommentParser::~JavadocCommentParser()
{
    delete _tags;
}

const char* 
JavadocCommentParser::summarySentance()
{
    return _summarySentance;
}

const char* 
JavadocCommentParser::body()
{
    return _body;
}

CommentTag*
JavadocCommentParser::tags()
{
    return _tags;
}

void
JavadocCommentParser::parse()
{
    /* Make a temporary writable copy.
     */
    char* base = strdup(text());
    collapseStars(base);

    /* summary sentance -- gather text to the first period,
     * or to the beginning of the first tag.
     */
    char* pdot = strchr(base, '.');
    char* p = strchr_tags(base);
    if ((pdot && p && p < pdot) || (p && !pdot)) 
    {
	*p = '\0';
	_summarySentance = _pool.add(base);
	*p = '@';
    }
    else if (pdot)
    {
    	p = pdot;
	char c = *(p + 1);
	*(p + 1) = '\0';
	_summarySentance = _pool.add(base);
	*(p + 1) = c;
    }
    else
    {
	_summarySentance = _pool.add(base);
    }
    trim((char*) _summarySentance);
    _summarySentance = trimLeadingStarsAndSpace(_summarySentance);

    /* body -- all text up until the first tag.
     */
    if ((p = strchr_tags(base)))
    {
	*p = '\0';
	_body = _pool.add(base);
	*p = '@';
    }
    else
    {
	_body = _pool.add(base);
    }
    trim((char*) _body);
    _body = trimLeadingStarsAndSpace(_body);

    /* search for all tags, adding definitions in order.
     */
    p = base;
    CommentTag* lastTag = NULL;
    while ((p = strchr_tags(p)))
    {
	CommentTag* tag = new CommentTag;

	// read the tag name
	char* eot = p + 1;
	while(isalpha(*eot)) eot++;
	char c = *eot;
	*eot = '\0';
	tag->tag = _pool.add(p + 1);
	*eot = c;

	// skip white space
	while(isspace(*eot)) eot++;

	// get the value
	char* eol = strchr_tags(eot);
	if (eol)
	{
	    c = *eol;
	    *eol = '\0';
	    tag->value = _pool.add(eot);
	    *eol = c;
	    p = eol;
	}
	else
	{
	    tag->value = _pool.add(eot);
	    p = ""; // done
	}
	trim((char*) tag->value);

	// skip this if it is an "{@link}" tag, which we handle later
	if (strcmp(tag->tag, "link") == 0)
	{
	    delete tag;
	}
	else
	{
	    // add it in
	    if (lastTag)
	    {
		lastTag->next = tag;
	    }
	    else
	    {
		_tags = tag;
	    }

	    lastTag = tag;
	}
    }

    free(base);
}

void
JavadocCommentParser::collapseStars(char* text)
{
    char* p = text;
    while ((p = strchr(p, '\n')))
    {
	char* next = p + 1;
	while (*next != '\0' && isspace(*next)) next++;
	if (*next == '*')
	{
	    int len = strlen(next);
	    memmove(p + 1, next + 1, len * sizeof(char));
	}

	p++;
    }
}

void
JavadocCommentParser::trim(char* text)
{
    char* p = text;
    while (isspace(*p)) p++;

    if (p != text)
    {
	int len = strlen(p);
	memmove(text, p, (len + 1) * sizeof(char));
    }

    p = text + strlen(text) - 1;
    while(p > text && isspace(*p))
    {
	*p = '\0';
	p--;
    }
}


