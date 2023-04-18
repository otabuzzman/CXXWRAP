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
 * $Id: JavadocLink.cxx,v 1.7 2006/12/17 04:08:11 deaven Exp $
 */

#include <string.h>
#include <ctype.h>
#include "Class.h"
#include "ClassJar.h"
#include "JavadocLink.h"

JavadocLink::JavadocLink(const char* text, ClassJar* jar_) :
    pool(),
    jar(jar_),
    href(NULL),
    link(NULL)
{
    parse(text);
}

void
JavadocLink::parse(const char* text)
{
    StringPool copy; // we'll use it as writable
    char* p = (char*) copy.add(text);
    char save;

    // eat space
    while (isspace(*p)) p++;

    // eat until NL or # to get "name"
    const char* name = p;
    while(isalnum(*p) || *p == '_') p++;
    save = *p;
    *p = '\0';
    name = pool.add(name);
    *p = save;

    // get the method detail if needed
    const char* method = NULL;
    if (*p == '#')
    {
	// scan method name
	method = p + 1;

	int depth = 0;
	while ((isalnum(*p) || depth > 0) && *p != '\0')
	{
	    if (*p == '(') depth++;
	    else if (*p == ')') depth--;
	    p++;
	}
	save = *p;
	*p = '\0';
	method = pool.add(method);
	*p = save;
    }

    // do we know this class?
    Class* c = jar->findClassByName(name);

    if (name && strlen(name) > 0 && !c)
    {
	link = text;

	// all done if no link is needed
	const char* s;
	for (s = text; *s != '\0'; s++)
	{
	    if (!isprint(*s) || isspace(*s)) return;
	}

	// looks like it might be a url, link to it
	href = text;
	return;
    }
    else if (method)
    {
	char buf[TOKEN_BUFFER_SIZE];
	if (!name || strlen(name) == 0)
	{
	    sprintf(buf, "#%s", method);
	}
	else
	{
	    sprintf(buf, "%s.html#%s", name, method);
	}
	href = pool.add(buf);

	sprintf(buf, "%s()", method);
	link = pool.add(buf);
    }
    else if (c)
    {
	char buf[TOKEN_BUFFER_SIZE];
	sprintf(buf, "%s.html", name);
	href = pool.add(buf);
	link = text;
    }
    else
    {
	// there's just plain html here
	link = text;
    }
}

