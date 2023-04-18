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
 * $Id: AnalyzeEmitter.h,v 1.4 2006/12/17 04:08:11 deaven Exp $
 */

#include "Emitter.h"

class AnalyzeEmitter : public Emitter {
public:
    AnalyzeEmitter(ClassJar* jar_, FILE* fout_);
    ~AnalyzeEmitter();

    int emit(int argc, char** argv);

protected:
    FILE* fout;

    void addClass(Class* c);
    void addComment(const char* s, const char* curClass);
    void reset();
    void printResults(FILE* f);

    // count explicit cross-refs
    int numXref(const char* s);

    int classCount;
    int methodCount;
    int commentWordCount;
    int commentXrefCount;
    StringPool undocClasses;
    StringPool undocMethods;
};

