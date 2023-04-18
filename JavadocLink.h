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
 * $Id: JavadocLink.h,v 1.2 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __JavadocLink__
#define __JavadocLink__

#include "StringPool.h"

class ClassJar;

class JavadocLink {
public:
    JavadocLink(const char* text, ClassJar* jar);
    ~JavadocLink() { };

    const char* hrefText() { return href; }
    const char* linkText() { return link; }

private:
    void parse(const char* text);
    StringPool pool;
    ClassJar* jar;
    const char* href;
    const char* link;
};

#endif // __JavadocLink__
