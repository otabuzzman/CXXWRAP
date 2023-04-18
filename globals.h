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
 * $Id: globals.h,v 1.7 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __globals_h__
#define __globals_h__

#define COMMENT_BUFFER_SIZE 2048
#define CODE_BUFFER_SIZE 2048
#define TOKEN_BUFFER_SIZE 512

#include "StringPool.h"
#include "ClassJar.h"
#include "TypeMap.h"

class globals {
public:
    globals() :
        callbacks(0),
	attributes(0),
        verbose(0),
        public_doc(0),
        pool(),
        classJar(),
	typedefs()
        { }
    int callbacks;
    int attributes;
    int verbose;
    int public_doc;
    StringPool pool;
    ClassJar classJar;
    TypeMap typedefs;
};

extern globals global;

#endif // __globals__
