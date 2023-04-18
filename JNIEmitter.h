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
 * $Id: JNIEmitter.h,v 1.22 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __JNIEmitter__
#define __JNIEmitter__

#include "Emitter.h"
#include "FunctionCType.h"

class EnumCType;

class JNIEmitter : public Emitter {
public:
    JNIEmitter(ClassJar* jar_, const char* classpath_);
    virtual ~JNIEmitter();
    virtual int emit(int argc, char** argv);

protected:
    int emit_class(Class* c);
    int emit_callback_interface(FILE* fjava_cb, Class* c, const char* cb_name, FunctionCType* fct);
    int emit_method(FILE* fjava, FILE* fcxx, Class* c, Method* m);
    int emit_ctor(FILE* fjava, FILE* fcxx, Class* c, Method* m);
    int emit_noctor(FILE* fjava, FILE* fcxx, Class* c, Method* m);
    int emit_required_includes(FILE* fcxx, Class* c);
    int emit_adapter_def(FILE* fcxx, Class* c);
    int expand_if_needed(Class* c);
    CType* subst_jni(CType* t);
    int emit_required_imports(FILE* fjava, Class* c);
    int emit_enum(FILE* fjava, EnumCType* ect);
    int emit_comment(FILE* fjava, const char* comment, const char* add = NULL);
    int emit_jni_begin(FILE* fcxx, Arg* a);
    int emit_jni_end(FILE* fcxx, Arg* a);
    int emit_cb_jni(FILE* fcxx, Arg* fct_arg);
    int emit_cb_begin(FILE* fcxx, Arg* a);
    int emit_cb_end(FILE* fcxx, Arg* a);
    void analyze_class(Class* c);
    const char* path_of_class(Class* c);
    const char* jni_prefix_of_class(Class* c);
    const char* jni_type_of(CType* ct);
    const char* jni_cast_to(CType* ct, const char* expr);
    const char* java_type_of(CType* ct);
    const char* jni_type_spec(Method* m);
    const char* jni_type_spec_of_type(CType* t);
    int do_adapter(Method* m);
    const char* classpath;
    const char* import_hook;
    const char* begin_hook;
    const char* end_hook;
    int wrap_operators;
    int emit_defaulted;
    StringPool* emitted;

private:
    TypeMap callbacks;
};

#endif // __Emitter__

