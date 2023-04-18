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
 * $Id: DependEmitter.cxx,v 1.5 2006/12/17 04:08:11 deaven Exp $
 */

#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Class.h"
#include "ClassJar.h"
#include "DependEmitter.h"

DependEmitter::DependEmitter(ClassJar* jar_, FILE* fout_) :
    Emitter(jar_),
    fout(fout_)
{
}

DependEmitter::~DependEmitter()
{
}

static int class_cmp(Class** pa, Class** pb)
{
    Class *a = *pa;
    Class *b = *pb;
    char bufa[TOKEN_BUFFER_SIZE], bufb[TOKEN_BUFFER_SIZE];
    sprintf(bufa, "%s/%s", a->packageName(), a->fullName());
    sprintf(bufb, "%s/%s", b->packageName(), b->fullName());
    return strcmp(bufa, bufb);
}

static int string_cmp(string* a, string* b)
{
    return strcmp((*a), (*b));
}

static string* alpha_list(StringPool* pool, int* len = NULL)
{
    string* e = pool->elements();
    int mylen = 0;
    int i;
    for (i = 0; e[i]; i++)
    {
	mylen++;
    }
    qsort(e, mylen, sizeof(string),
	(int (*)(const void *, const void *)) string_cmp);

    if (len) (*len) = mylen;
    return e;
}

int DependEmitter::emit(int /* argc */, char** /* argv */)
{
    int i, j;

    // package dependencies

    fprintf(fout, "Package dependencies:");
    StringPool* packages = jar->packages();
    string* e = alpha_list(packages);
    for (i = 0; e[i]; i++)
    {
	fprintf(fout, "\n  %s depends on {", e[i]);
	StringPool* pd = jar->packageDepends(e[i]);
	string* ed = alpha_list(pd);
	int count = 0;
	for (j = 0; ed[j]; j++)
	{
	    if (0 != strcmp(ed[j], e[i]))
	    {
		fprintf(fout, "%s%s", count > 0 ? ", " : "", ed[j]);
		count++;
	    }
	}
	fprintf(fout, "}");
	delete[] ed;
	delete pd;
    }
    fprintf(fout, "\n\n");

    delete[] e;
    delete packages;

    // now do class list

    fill_ctab();
    qsort(ctab, class_count, sizeof(Class*),
	(int (*)(const void *, const void *)) class_cmp);

    fprintf(fout, "Class dependencies:\n");
    Class* c;
    for (i = 0; i < class_count; i++)
    {
	char buf[TOKEN_BUFFER_SIZE];
	c = ctab[i];
	fprintf(fout, "  %s/%s depends on:",
	    c->packageName(), c->fullName());
	ClassJar* d = c->depends();
	ClassJarIterator di(d->iterator());
	Class* dc;
	int count = 0;
	StringPool cd;
	while ((dc = di.next()))
	{
	    if (dc->fullName() != c->fullName())
	    {
		sprintf(buf, "%s/%s", dc->packageName(), dc->fullName());
		cd.add(buf);
		count++;
	    }
	}
	if (count == 0) 
	{
	    fprintf(fout, " (no other classes)\n");
	}
	else
	{
	    string* ccd = alpha_list(&cd);
	    for (j = 0; ccd[j]; j++)
	    {
		fprintf(fout, "\n    %s", ccd[j]);
	    }
	    fprintf(fout, "\n");
	    delete ccd;
	}
	di.cancel();
	delete d;
    }

    return 0;
}
