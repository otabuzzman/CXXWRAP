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
 * $Id: ClassJar.h,v 1.9 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __ClassJar__
#define __ClassJar__

#include <stdio.h> // NULL
class Class;
class CType;
class StringPool;

class ClassNode {
public:
    ClassNode(Class* c) : clazz(c), lesser(NULL), greater(NULL), visited(0) { }
    ~ClassNode() { delete lesser; delete greater; }

    Class* clazz;
    ClassNode* lesser;
    ClassNode* greater;
    long visited;

    void clearVisited(long bit) {
	visited &= ~bit;
	if (lesser) lesser->clearVisited(bit);
	if (greater) greater->clearVisited(bit);
    }

    Class* next(long bit) {
	Class* c;
	if (lesser && (c = lesser->next(bit))) return c;
	if (greater && (c = greater->next(bit))) return c;

	if (!(visited & bit)) {
	    visited |= bit;
	    return clazz;
	}

	return NULL;
    }
};

class ClassJarIterator;
class ClassJar {
    friend class ClassJarIterator;
public:
    ClassJar() :
	root(NULL),
	count(0),
	ibits(0),
	evaluating_depends(0)
	{
	    unsigned int i;
	    for (i = 0; i < sizeof(long); i++) bitcount[i] = 0;
	}
    ~ClassJar() { delete root; }

    void add(Class* c);
    void addJar(ClassJar* other);
    int size() const { return count; }
    Class* findClassByName(const char* s) const;

    /** Find a class by the given (object) type.  If an author is
     *  given, use that class's package as the package to expand
     *  templates into.
     */
    Class* findClassByType(CType* t, Class* author = NULL);

    ClassJarIterator iterator();

    StringPool* packageDepends(const char* package);
    StringPool* packages();

    enum TemplateMode {
	INVOKING_PACKAGE,
	PARAMETER_PACKAGE,
	AUTO_PACKAGE
    };

    static TemplateMode templateMode;

private:
    ClassNode* root;
    int count;
    long ibits;
    int evaluating_depends;

    long newbit() {
	long bit = 1;
	unsigned int i;
	for (i = 0; i < sizeof(long); i++)
	{
	    if (!(ibits & bit)) 
	    {
		ibits |= bit;
		bitcount[i]++;
		return bit;
	    }
	    bit = bit << 1;
	}
	return 0;
    }

    void returnbit(long bit) {
	unsigned int i;
	for (i = 0; i < sizeof(long) && !(bit & (1 << i)); i++);
	if (i < sizeof(long))
	{
	    bitcount[i]--;
	    if (bitcount[i] == 0)
	    {
		ibits &= ~bit;
	    }
	}
    }

    void usebit(long bit) {
	unsigned int i;
	for (i = 0; i < sizeof(long) && !(bit & (1 << i)); i++);
	if (i < sizeof(long))
	{
	    bitcount[i]++;
	}
    }

    unsigned int bitcount[sizeof(long)];
};

class ClassJarIterator {
public:
    ClassJarIterator(ClassJar* jar_, long bit_) :
	jar(jar_), bit(bit_) { reset(); }
    ClassJarIterator(const ClassJarIterator& other) :
	jar(other.jar), bit(other.bit) { jar->usebit(bit); }
    ~ClassJarIterator() { if (jar) jar->returnbit(bit); }

    Class* next() {
	if (jar && jar->root)
	{
	    return jar->root->next(bit);
	}
	return NULL;
    }

    void reset() {
	if (jar && jar->root)
	{
	    jar->root->clearVisited(bit);
	}
    }

    void cancel() { jar = NULL; }

private:
    ClassJar* jar;
    long bit;
};

#endif // __ClassJar__
