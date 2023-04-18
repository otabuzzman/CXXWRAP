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
 * $Id: AnalyzeEmitter.cxx,v 1.10 2006/12/17 04:08:11 deaven Exp $
 */

#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Class.h"
#include "CommentParser.h"
#include "ClassJar.h"
#include "AnalyzeEmitter.h"

int AnalyzeEmitter::numXref(const char* comment)
{
    int count = 0;

    // count explicit references

    CommentParser* cp = comment_parser(comment);
    CommentTag* tags = cp->tags();
    if (tags)
    {
	CommentTag* list = tags->enumerateTag("see");
	CommentTag* t;
	if (list)
	{
	    for (t = list; t; t = t->next)
	    {
		count++;
	    }
	    delete list;
	}
	list = tags->enumerateTag("link");
	if (list)
	{
	    for (t = list; t; t = t->next)
	    {
		count++;
	    }
	    delete list;
	}
    }
    delete cp;

    return count;
}

void AnalyzeEmitter::addComment(const char* s, const char* curClass)
{
    int count = 0;
    int xrefCount = 0;
    const char* p = s;

    char buf[COMMENT_BUFFER_SIZE];

    while (*p != '\0')
    {
	while (*p != '\0' && isspace(*p)) p++;

	// check out class cross-refs
	strncpy(buf, p, COMMENT_BUFFER_SIZE - 1);
	char* q = buf;
	while (isalnum(*q)) q++;
	*q = '\0';
	if (strcmp(buf, curClass) && jar->findClassByName(buf))
	{
	    xrefCount++;
	}

	if (isprint(*p)) count++;
	while (isprint(*p) && !isspace(*p)) p++;
    }

    commentWordCount += count;
    commentXrefCount += numXref(s) + xrefCount;
}

void AnalyzeEmitter::addClass(Class* c)
{
    // check if we are supposed to suppress docs on this one
    if (c->comment && strstr(c->comment, "<<NOT_API>>"))
    {
	return;
    }

    classCount++;
    if (c->comment) 
    {
	addComment(c->comment, c->name());
    }
    else
    {
	undocClasses.add(c->fullName());
    }

    Method* mlist = c->methods;
    Method* m;
    for (m = mlist; m; m = m->next)
    {
	methodCount++;
	if (m->comment)
	{
	    addComment(m->comment, c->name());
	}
	else
	{
	    /* Try walking up the parent chain looking for a comment.
	     */
	    Class* p = c->parentClass();
	    Method* pm = NULL;
	    while (p && (pm = p->methodWithSignature(m)))
	    {
		if (pm && pm->comment) break;
		p = p->parentClass();
	    }

	    if (pm && pm->comment)
	    {
		addComment(pm->comment, p->name());
	    }
	    else if (!(m->type->modifiers & (CType::M_PRIVATE | CType::M_PROTECTED)))
	    {
		char buf[TOKEN_BUFFER_SIZE];
		sprintf(buf, "%s::%s%s",
		    c->fullName(),
		    m->type->modifiers & CType::M_DTOR ? "~" : "",
		    m->name);
		undocMethods.add(buf);
	    }
	}
    }
}

AnalyzeEmitter::AnalyzeEmitter(ClassJar* jar_, FILE* fout_) :
    Emitter(jar_),
    fout(fout_)
{
    reset();
}

AnalyzeEmitter::~AnalyzeEmitter()
{
}

void AnalyzeEmitter::reset()
{
    classCount = 0;
    methodCount = 0;
    commentWordCount = 0;
    commentXrefCount = 0;
    undocClasses.clear();
    undocMethods.clear();
}

static double safedivide(double num, double denom)
{
    if (denom == 0)
    {
	if (num == 0) return 0.0;
	return DBL_MAX;
    }

    return num / denom;
}

static int scmp(string* a, string* b)
{
    return strcmp(*a, *b);
}

static void print_list(FILE* f, string* s)
{
    int i;
    int count = 0;

    for (i = 0; s[i] != NULL; i++) count++;

    qsort(s, count, sizeof(string), 
	(int (*)(const void *, const void *)) scmp);

    for (i = 0; s[i] != NULL; i++)
    {
	fprintf(f, "  %s\n", s[i]);
    }
}

void AnalyzeEmitter::printResults(FILE* f)
{
    int documentedClassCount = classCount - undocClasses.size();
    int documentedMethodCount = methodCount - undocMethods.size();
    int documentedCount = documentedClassCount + documentedMethodCount;
    double fracDoc = safedivide(documentedCount, methodCount + classCount);
    double methodsPerClass = safedivide(methodCount, classCount);
    double wordsPerDoc = safedivide(commentWordCount, documentedCount);
    double xrefPerDoc = safedivide(commentXrefCount, documentedCount);

    fprintf(f, "Analysis of %d class%s:\n", classCount, classCount == 1 ? "" : "es");
    fprintf(f, "  methods per class: %.1f\n", methodsPerClass);
    fprintf(f, "  fraction of API documented: %.1f%%\n", fracDoc * 100.0);
    fprintf(f, "  classes with no class doc: %d (%.1f%%)\n",
	undocClasses.size(), 
	100.0 * safedivide(undocClasses.size(), classCount));
    fprintf(f, "  methods with no doc: %d (%.1f%%)\n",
	undocMethods.size(),
	100.0 * safedivide(undocMethods.size(), methodCount));
    fprintf(f, "  words per doc: %.1f\n", wordsPerDoc);
    fprintf(f, "  cross references per doc: %.1f (%d cross-refs)\n",
	xrefPerDoc, commentXrefCount);

    if (undocClasses.size() > 0)
    {
	fprintf(f, "\nUndocumented classes:\n");
	string* s = undocClasses.elements();
	print_list(f, s);
	delete[] s;
    }

    if (undocMethods.size() > 0)
    {
	fprintf(f, "\nUndocumented methods:\n");
	string* s = undocMethods.elements();
	print_list(f, s);
	delete[] s;
    }
}

int AnalyzeEmitter::emit(int /* argc */, char** /* argv */)
{
    reset();

    Class* c;
    ClassJarIterator iter(jar->iterator());
    while ((c = iter.next()))
    {
	addClass(c);
    }

    printResults(fout);

    return 0;
}
