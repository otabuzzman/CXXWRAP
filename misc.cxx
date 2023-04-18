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
 * $Id: misc.cxx,v 1.11 2006/12/17 04:08:11 deaven Exp $
 */

#include <string.h>
#include <stdlib.h>
#include "Class.h"
#include "CType.h"

const char* argval(const char* s, char* arg)
{
    int len = strlen(s);
    if (strncmp(arg, s, len) == 0 && arg[len] == '=')
    {
	return arg + len + 1;
    }
    return NULL;
}

const char* argval_in(const char* s, int argc, char** argv)
{
    int arg;
    for (arg = 0; arg < argc; arg++)
    {
	const char* r = argval(s, argv[arg]);
	if (r) return r;
    }
    return NULL;
}

int arg_in(const char* s, int argc, char** argv)
{
    int arg;
    for (arg = 0; arg < argc; arg++)
    {
	if (!strcmp(s, argv[arg])) return 1;
    }
    return 0;
}

const char* read_file(const char* fn)
{
    static char* buf = NULL;
    static int len = 0;

    FILE* f = fopen(fn, "r");
    if (!f) return NULL;

    int fp = 0;
    int c;
    while ((c = fgetc(f)) != EOF)
    {
	if (fp >= len)
	{
	    len += 512;
	    buf = (char*) (buf ? realloc(buf, len * sizeof(char)) : malloc(len * sizeof(char)));
	    if (!buf) abort();
	}
	buf[fp++] = c;
    }
    buf[fp++] = '\n';
    buf[fp++] = '\0';
    return buf;
}

#ifdef WIN32
int strncasecmp(const char* s1, const char* s2, int len)
{
	static char* buf1 = NULL;
	static char* buf2 = NULL;
	static unsigned int len1 = 0;
	static unsigned int len2 = 0;

	if ((strlen(s1) + 8) > len1)
	{
		if (buf1 != NULL) free(buf1);
		len1 = strlen(s1) + 8;
		buf1 = (char*) malloc(len1 * sizeof(char));
	}

	if ((strlen(s2) + 8) > len2)
	{
		if (buf2 != NULL) free(buf2);
		len2 = strlen(s2) + 8;
		buf2 = (char*) malloc(len2 * sizeof(char));
	}

	int i;
	for (i = 0; s1[i] != '\0'; i++)
	{
		buf1[i] = tolower(s1[i]);
	}
	for (i = 0; s2[i] != '\0'; i++)
	{
		buf2[i] = tolower(s2[i]);
	}

	return strncmp(buf1, buf2, len);
}
#endif

static int warnings = 0;
static const char* where = NULL;

void set_warning_context(const char* s)
{
    where = s;
}

void warning(const char* msg)
{
    warnings++;
    printf("WARNING: ");
    if (NULL != where) printf("%s: ", where);
    printf("%s\n", msg);
}

int error_summary()
{
    printf("Summary: ");
    if (warnings > 0)
	printf("%d warnings, ", warnings);
    printf(".\n");
    return 0;
}
    
