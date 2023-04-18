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
 * $Id: ArrayCType.cxx,v 1.14 2006/12/17 04:08:11 deaven Exp $
 */

#include "defs.h"
#include "ArrayCType.h"
#include <string.h>

int
ArrayCType::writeTo(FILE* f)
{
    CType::writeTo(f);
    writeTokenTo(base, f);
    writeIntTo(len, f);
    return 0;
}

ArrayCType*
ArrayCType::readFrom(FILE* f)
{
    CType* t = CType::readFrom(f);
    ArrayCType* a = new ArrayCType(*t);
    delete t;
    a->base = (CType*) readTokenFrom(f);
    a->len = readIntFrom(f);
    return a;
}

const char*
ArrayCType::JavaTypeName()
{
    char buf[TOKEN_BUFFER_SIZE] = "";

    CType* ct = base;
    while (!strcmp(ct->classname(),"ArrayCType")) ct = ((ArrayCType*)ct)->base;

    const char* bn = ct->JavaTypeName();
    if (!bn) return NULL;
    sprintf(buf, "%s", bn);

    int i = modifiers & M_REFERENCE ? 1 : 0;
    if (modifiers & M_POINTER && !strcmp(ct->classname(),"ObjectCType")) i++;
    for (; i < indirection_count; i++) strcat(buf, "[]");
    return global.pool.add(buf);
}

const char*
ArrayCType::JNITypeName()
{
    if (!strcmp(base->classname(),"ArrayCType") || (modifiers & M_POINTER))
    {
        return "jobjectArray";
    }

    const char* bn = base->JavaJNITypeName();
    if (!bn) return NULL;

    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%sArray", bn);
    return global.pool.add(buf);
}

const char*
ArrayCType::CTypeName()
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s*", base->CTypeName());
    return global.pool.add(buf);
}

const char*
ArrayCType::JNICast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "__T%s", expr);
    return global.pool.add(buf);
}

const char*
ArrayCType::JNIUpCast(const char* expr)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "(%s) %s", JNITypeName(), expr);
    return global.pool.add(buf);
}

const char*
ArrayCType::JNIBegin(const char* expr)
{
    /* The code emitted by this function is not optimal in all
     * cases. In particular, if the array is of a primitive type, and
     * the C and Java types have the same size, memcpy() could be
     * used. For the special case of one-dimensional arrays, the
     * native array could be used directly.  Also, by storing the
     * pointers returned by Get<type>ArrayElements, the number of
     * calls to that function could be halved.
     */
    static int first = 1;
    static int depth = 1;
    static int obj = 0;
    static int obj_ptr = 0;
    static CType* fbase;
    /* Should be good for more dimensions than anybody would reasonably need */
    char buf[CODE_BUFFER_SIZE*10] = "";

    if (first)
    {
        fbase = base;
        while (!strcmp(fbase->classname(),"ArrayCType")) fbase = ((ArrayCType*)fbase)->base;

        if (modifiers & M_POINTER && modifiers & M_REFERENCE)
        {
            warning("Reference to pointer to array not supported");
            return "";
        }

        // Is this an array of objects or object pointers?
        if (!strcmp(fbase->classname(),"ObjectCType")) obj = 1;

        // Is this a pointer to an array of objects?
        if (modifiers & M_POINTER && !strcmp(fbase->classname(),"ObjectCType")) obj_ptr = 1;

        if (modifiers & M_POINTER && !obj_ptr)
        {
            sprintf(buf+strlen(buf),
                    "int __TL%s = __env->GetArrayLength(%s);\n",
                    expr, expr
                );
            sprintf(buf+strlen(buf),
                    "%s%s __T%s[__TL%s][%d]",
                    fbase->BareCTypeName(), fbase->CTypeNameSuffix(), expr, expr, len
                );
        }
        else
        {
            sprintf(buf+strlen(buf),
                    "%s%s (%s__T%s)[%d]",
                    fbase->BareCTypeName(), fbase->CTypeNameSuffix(), (obj_ptr?" *":""), expr, len
                );
        }

        CType* ct = base;
        while (!strcmp(ct->classname(),"ArrayCType"))
        {
            ArrayCType* act = (ArrayCType*)ct;
            sprintf(buf+strlen(buf),
                    "[%d]",
                    act->len
                );
            ct = act->base;
        }
        sprintf(buf+strlen(buf), ";\n");
    }

    if (strcmp(base->classname(),"ArrayCType") && !(modifiers & M_POINTER))
    {
        if (!first)
        {
            sprintf(buf+strlen(buf),
                    "%sArray __TD%d = (%sArray)__env->GetObjectArrayElement(__TD%d%s, __i%d);\n",
                    fbase->JavaJNITypeName(), depth, fbase->JavaJNITypeName(), depth-1, expr, depth-1
                );
        }

        char varname[TOKEN_BUFFER_SIZE];
        if (first) sprintf(varname, "%s", expr);
        else sprintf(varname, "__TD%d", depth);

        if (!obj)
        {    
            sprintf(buf+strlen(buf),
                    "%s *__element_%s = __env->Get%sArrayElements(%s, 0);\n",
                    fbase->JavaJNITypeName(), expr, fbase->JNIFunctionTypeName(), varname
                );
        }

        sprintf(buf+strlen(buf),
                "{int __i%d; for(__i%d=0; __i%d<%d; __i%d++) {\n",
                depth, depth, depth, len, depth
            );

        
        if (obj)
        {
            sprintf(buf+strlen(buf),
                    "jobject __element_%s = __env->GetObjectArrayElement(%s, __i%d);\n"
                    "if (!__element_%s) continue;\n"
                    "jclass __obj_class = __env->GetObjectClass(__element_%s);\n"
                    "jfieldID __fid = __env->GetFieldID(__obj_class, \"implementation\", \"Ljava/lang/Long;\");\n"
                    "jobject __impl_obj = __env->GetObjectField(__element_%s, __fid);\n"
                    "jlong __impl_value = __env->CallLongMethod(__impl_obj, __env->GetMethodID(__env->FindClass(\"java/lang/Long\"), \"longValue\", \"()J\"));\n",
                    expr, varname, depth,
                    expr,
                    expr,
                    expr
                );
        }

        sprintf(buf+strlen(buf),
                "    (%s__T%s)",
                (obj_ptr?"*":""), expr
            );
        for (int i=1; i <= depth; i++) sprintf(buf+strlen(buf), "[__i%d]", i);

        if (obj)
        {
            sprintf(buf+strlen(buf),
                    " = %s((%s*)__impl_value);\n",
                    (fbase->modifiers & M_POINTER?"":"*"), fbase->BareCTypeName()
                );
        }
        else
        {
            sprintf(buf+strlen(buf), " = __element_%s[__i%d];\n", expr, depth);
        }

        sprintf(buf+strlen(buf),
                "}}\n"
            );

        if (!obj)
        {
            sprintf(buf+strlen(buf),
                    "__env->Release%sArrayElements(%s, __element_%s, JNI_ABORT);\n",
                    fbase->JNIFunctionTypeName(), varname, expr
            );
        }
            
    }
    else
    {
        if (first) {
            first = 0;

            char alen[TOKEN_BUFFER_SIZE];
            if (modifiers & M_POINTER && !obj_ptr)
            {
                sprintf(alen, "__TL%s", expr);
            }
            else
            {
                sprintf(alen, "%d", len);
            }

            sprintf(buf+strlen(buf),
                    "jobjectArray __TD%d%s = %s;\n"
                    "{int __i%d; for(__i%d=0; __i%d<%s; __i%d++) {\n",
                    depth, expr, expr,
                    depth, depth, depth, alen, depth
                );
            
            if (modifiers & M_POINTER && !obj)
            {
                modifiers ^= M_POINTER;
                depth++;
                strcat(buf+strlen(buf), JNIBegin(expr));
                depth--;
                modifiers |= M_POINTER;
            }
            else
            {
                depth++;
                strcat(buf+strlen(buf), base->JNIBegin(expr));
                depth--;
            }
            strcat(buf+strlen(buf), "}}\n");

            first = 1;
        }
        else
        {
            sprintf(buf+strlen(buf),
                    "jobjectArray __TD%d%s = (jobjectArray)__env->GetObjectArrayElement(__TD%d%s, __i%d);\n"
                    "{int __i%d; for(__i%d=0; __i%d<%d; __i%d++) {\n",
                    depth, expr, depth-1, expr, depth-1,
                    depth, depth, depth, len, depth
                );

            depth++;
            strcat(buf+strlen(buf), base->JNIBegin(expr));
            depth--;
            strcat(buf+strlen(buf), "}}\n");
        }
    }

    if (first)
    {
        // Reset static variables
        obj = 0;
        obj_ptr = 0;
    }
    
    return global.pool.add(buf);
}

const char*
ArrayCType::JNIEnd(const char* expr)
{
    static int first = 1;
    static int depth = 1;
    static CType* fbase;
    /* Should be good for more dimensions than anybody would reasonably need */
    char buf[CODE_BUFFER_SIZE*10] = "";

    if (first)
    {
        fbase = base;
        while (!strcmp(fbase->classname(),"ArrayCType")) fbase = ((ArrayCType*)fbase)->base;

        if (modifiers & M_POINTER && modifiers & M_REFERENCE)
        {
            warning("Reference to pointer to array not supported");
            return "";
        }

        // Is this an array of objects or object pointers?
        if (!strcmp(fbase->classname(),"ObjectCType"))
        {
            /* No need to copy objects back, as we only have a pointer to them on the
             * Java side anyway
             */
            return "";
        }
    }

    if (strcmp(base->classname(),"ArrayCType") && !(modifiers & M_POINTER))
    {
        if (!first)
        {
            sprintf(buf+strlen(buf),
                    "%sArray __TD%d = (%sArray)__env->GetObjectArrayElement(__TD%d%s, __i%d);\n",
                    fbase->JavaJNITypeName(), depth, fbase->JavaJNITypeName(), depth-1, expr, depth-1
                );
        }

        char varname[TOKEN_BUFFER_SIZE];
        if (first) sprintf(varname, "%s", expr);
        else sprintf(varname, "__TD%d", depth);

        sprintf(buf+strlen(buf),
                "%s *__element__%s = __env->Get%sArrayElements(%s, 0);\n",
                fbase->JavaJNITypeName(), expr, fbase->JNIFunctionTypeName(), varname
            );

        sprintf(buf+strlen(buf),
                "{int __i%d; for(__i%d=0; __i%d<%d; __i%d++) {\n",
                depth, depth, depth, len, depth
            );

        
        sprintf(buf+strlen(buf), "__element__%s[__i%d] = (__T%s)",
                expr, depth, expr
            );
        for (int i=1; i <= depth; i++) sprintf(buf+strlen(buf), "[__i%d]", i);        

        sprintf(buf+strlen(buf),
                ";\n}}\n"
            );

        sprintf(buf+strlen(buf),
                "__env->Release%sArrayElements(%s, __element__%s, JNI_COMMIT);\n",
                fbase->JNIFunctionTypeName(), varname, expr
            );            
    }
    else
    {
        if (first) {
            first = 0;

            char alen[TOKEN_BUFFER_SIZE];
            if (modifiers & M_POINTER)
            {
                sprintf(alen, "__TL%s", expr);
            }
            else
            {
                sprintf(alen, "%d", len);
            }

            sprintf(buf+strlen(buf),
                    "{int __i%d; for(__i%d=0; __i%d<%s; __i%d++) {\n",
                    depth, depth, depth, alen, depth
                );
            
            if (modifiers & M_POINTER)
            {
                modifiers ^= M_POINTER;
                depth++;
                strcat(buf+strlen(buf), JNIEnd(expr));
                depth--;
                modifiers |= M_POINTER;
            }
            else
            {
                depth++;
                strcat(buf+strlen(buf), base->JNIEnd(expr));
                depth--;
            }
            strcat(buf+strlen(buf), "}}\n");

            first = 1;
        }
        else
        {
            sprintf(buf+strlen(buf),
                    "jobjectArray __TD%d%s = (jobjectArray)__env->GetObjectArrayElement(__TD%d%s, __i%d);\n"
                    "{int __i%d; for(__i%d=0; __i%d<%d; __i%d++) {\n",
                    depth, expr, depth-1, expr, depth-1,
                    depth, depth, depth, len, depth
                );

            depth++;
            strcat(buf+strlen(buf), base->JNIEnd(expr));
            depth--;
            strcat(buf+strlen(buf), "}}\n");
        }
    }

    return global.pool.add(buf);
}
