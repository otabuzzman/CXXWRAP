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
 * $Id: StringPool.cxx,v 1.13 2006/12/17 04:08:11 deaven Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StringPool.h"

class StringNode {
public:
    StringNode(const char* s) : str(strdup(s)), lesser(NULL), greater(NULL) { }
    ~StringNode();

    const char* str;
    StringNode* lesser;
    StringNode* greater;
};

StringNode::~StringNode()
{
    if (lesser) delete lesser;
    if (greater) delete greater;
    free((char*) str);
}

StringPool::StringPool() :
    root(NULL),
    count(0)
{
}

StringPool::~StringPool()
{
    clear();
}

void StringPool::clear()
{
    if (root)
    {
	delete root;
	root = NULL;
    }
    count = 0;
}


int StringPool::contains(const char* s)
{
    StringNode* n = root;

    int r = 0;
    while (n)
    {
	r = strcmp(n->str, s);
	if (r == 0) return 1;
	n = (r < 0 ? n->lesser : n->greater);
    }

    return 0;
}

const char* StringPool::add(const char* s)
{
    if (s == NULL) return NULL;

    StringNode* n = root;
    StringNode* leaf = NULL;

    int r = 0;
    while (n)
    {
	r = strcmp(n->str, s);
	if (r == 0) return n->str;
	leaf = n;
	n = (r < 0 ? n->lesser : n->greater);
    }

    if (leaf)
    {
	StringNode* x = new StringNode(s);
	if (r < 0)
	{
	    leaf->lesser = x;
	}
	else
	{
	    leaf->greater = x;
	}

	count++;
	return x->str;
    }

    root = new StringNode(s);
    count = 1;
    return root->str;
}

static int count_recursive(StringNode* n, string** base)
{
    int count = 1;
    if (base) {
	(**base) = n->str;
	(*base)++;
    }
    if (n->lesser) count += count_recursive(n->lesser, base);
    if (n->greater) count += count_recursive(n->greater, base);
    return count;
}

string* StringPool::elements()
{
    string* array;
    if (root)
    {
	array = new string[count + 1];
	string* base = array;
	count_recursive(root, &base);
	array[count] = NULL;
	return array;
    }

    array = new string[1];
    *array = NULL;
    return array;
}

void StringPool::print()
{
    string* e = elements();
    int i;
    printf("StringPool 0x%lx={", (long) this);
    for (i = 0; e[i]; i++)
    {
	printf("%s%s", e[i], e[i + 1] ? "|" : "}\n");
    }
    delete e;
}
