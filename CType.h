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
 * $Id: CType.h,v 1.24 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __CType__
#define __CType__

#include "Token.h"

class Arg;

class CType : public Token {
public:
    enum Modifier {
	M_CONST     = 1 << 0,
	M_STATIC    = 1 << 1,
	M_REFERENCE = 1 << 2,
	M_POINTER   = 1 << 3,
	M_ENUM	    = 1 << 4,
	M_VIRTUAL   = 1 << 5,
	M_TEMPLATE  = 1 << 6,
	M_UNSIGNED  = 1 << 7,
	M_VOLATILE  = 1 << 8,
	M_REGISTER  = 1 << 9,
	M_CTOR      = 1 << 10,
	M_DTOR      = 1 << 11,
	M_PROTECTED = 1 << 12,
	M_PRIVATE   = 1 << 13,
	M_VARARG    = 1 << 14,
	M_ATTRIBUTE = 1 << 15,
	M_TYPEDEF   = 1 << 16,
	M_OPERATOR  = 1 << 17,
	M_PTR_CONST = 1 << 18
    };

    CType(const char* _comment, long mods = 0L) :
	Token(_comment), modifiers(mods), indirection_count(0),
	alias(NULL) { }
    virtual CType* clone() { return new CType(*this); }
    virtual ~CType() { }
    virtual const char* classname() const { return "CType"; }
    virtual int equals(CType* other);

    // expand a templated type
    virtual CType* expand(Arg* parameters, Arg* args);

    /** The C type name of this data type.
     */
    virtual const char* CTypeName();

    /** The C type name used by JNI to represent this type.
     */
    virtual const char* JNITypeName();

    /** Return the JNI type used in the return value from JNI
     *  functions, e.g. Call<type>Method.
     */
    virtual const char* JavaJNITypeName();

    /** Return the java type name as used in the Call<type>Method JNI
     *  functions.
     */
    virtual const char* JNIFunctionTypeName();

    /** The Java type name used in the public interface.
     */
    virtual const char* JavaTypeName();

    /** The Java type name used in the private native interface.
     */
    virtual const char* PrivateJavaTypeName();

    /** The default Java value used in the private native interface.
     */
    virtual const char* DefaultPrivateJavaValue();

    /** Emit any C code needed to support JNI casts.
     */
    virtual const char* JNIBegin(const char* expr);
    virtual const char* JNIEnd(const char* expr);
    virtual const char* JNIInvokeBegin(const char* expr);
    virtual const char* JNIInvokeEnd(const char* expr);

    /** Cast a C expression in JNI type to the proper C type.
     */
    virtual const char* JNICast(const char* expr);

    /** Cast a C expression in C to the JNI type.
     */
    virtual const char* JNIUpCast(const char* expr);

    /** Cast a C expression in C to the JNI type, without
	a previous down-cast (for C++ invoking Java).
     */
    virtual const char* JNIInvokeUpCast(const char* expr);

    /** Cast a java expression into the private java interface type.
     */
    virtual const char* JavaCast(const char* expr);

    /** Cast a java expression from private interface type to public
     *	interface type.
     */
    virtual const char* JavaUpCast(const char* expr);

    /** The string to append to the method name if an operator
     *  is used for casting to this type
     */
    virtual const char* OperatorSuffix();
    

    /* Functions dealing with callbacks */

    /** Return name of variable to use in argument list when calling
     *  the java callback method from native code.
     */
    virtual const char* CBCast(const char* expr);
    
    /** Return code for converting the return value from the Java
     *  method to a native type.
     */
    virtual const char* CBUpCast(const char* expr);

    /** Return code for converting from native to Java types before the
     *  call to the Java callback method.
     */
    virtual const char* CBBegin(const char* expr);

    /** Return code for cleaning up after the call to the Java
     *  callback method.
     */
    virtual const char* CBEnd(const char* expr);

    /** Return the java type id (used in forming a java method
     *  descriptor).
     */
    virtual const char* JavaTypeID();
    

    virtual int is_void();
    virtual int is_object();

    virtual const char* CTypeNamePrefix();
    virtual const char* BareCTypeName();
    virtual const char* CTypeNameSuffix();
    

    long modifiers;
    int indirection_count;
    const char* alias; // hack for type defs

    virtual void print();

    virtual int writeTo(FILE* f);
    static CType* readFrom(FILE* f);

protected:
    CType(const CType& other) :
	Token(other.comment), modifiers(other.modifiers),
	indirection_count(other.indirection_count),
	alias(other.alias) { }
    CType(const Token& other) :
	Token(other), modifiers(0L),
	indirection_count(0),
	alias(NULL) { }
};

#endif // __CType__
