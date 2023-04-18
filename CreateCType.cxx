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
 * $Id: CreateCType.cxx,v 1.10 2006/12/17 04:08:11 deaven Exp $
 */

#include "globals.h"
#include "CreateCType.h"

int
CreateCType::writeTo(FILE* f)
{
    CType::writeTo(f);
    return 0;
}

CreateCType*
CreateCType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    CreateCType* a = new CreateCType(*t);
    delete t;
    return a;
}

const char*
CreateCType::CTypeName()
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s%s", CTypeNamePrefix(),
	modifiers & M_DTOR ? "~" : "");
    return global.pool.add(buf);
}
