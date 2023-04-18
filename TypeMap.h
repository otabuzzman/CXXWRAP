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
 * $Id: TypeMap.h,v 1.5 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __TypeMap__
#define __TypeMap__

#include "CType.h"
#include <string.h>

class TypeMap {
public:
    TypeMap() : tab(NULL), count(0), allocated(0) { }
    ~TypeMap() {
	if (tab) delete[] tab;
    }

    CType* get(const char* alias) {
	int i = index(alias);
	if (i >= 0) return tab[i];
	return NULL;
    }
    CType* get(int i) {
        if (i >= count) return NULL;
        return tab[i];
    }

    void put(const char* alias, CType* ct) {
	int i = index(alias);
	if (i >= 0) {
	    tab[i] = ct;
	}
	else {
	    add(ct);
	}
    }

    int size() {
        return count;
    }

    void clear() {
	count = 0;
    }

    void copy(const TypeMap& other) {
	clear();
	allocate(other.count);
	count = other.count;
	int i;
	for (i = 0; i < count; i++) tab[i] = other.tab[i];
    }

private:
    TypeMap(const TypeMap& other) { } // unused

    int index(const char* alias) {
	int i;
	for (i = 0; i < count; i++) {
	    if (0 == strcmp(alias, tab[i]->alias)) return i;
	}
	return -1;
    }

    void add(CType* ct) {
	allocate(count + 1);
	tab[count++] = ct;
    }

    void allocate(int new_allocated)
    {
	if (new_allocated > allocated)
	{
	    CType** ntab = new CType*[new_allocated];
	    if (tab)
	    {
		int i;
		for (i = 0; i < count; i++) ntab[i] = tab[i];
		delete[] tab;
	    }
	    tab = ntab;
	}
	allocated = new_allocated;
    }

    CType** tab;
    int count;
    int allocated;
};

#endif // __TypeMap__

