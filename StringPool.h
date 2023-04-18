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
 * $Id: StringPool.h,v 1.11 2006/12/17 04:08:11 deaven Exp $
 */

#ifndef __StringPool__
#define __StringPool__

class StringNode;

typedef const char* string;

/** A string pool allows storage of strings, so that multiple pointers
    into the string storage may be made, and the memory can be freed 
    correctly after all user are done, without a lot of resource overhead.
 */
class StringPool {
public:
    StringPool();
    ~StringPool();

    /** Clear this pool, freeing all memory currently used.
     */
    void clear();
     
    /** Add a string to the pool.  The application passes
	in "s", and gets a pointer to the same string, 
	which might be a copy of an earlier equal string.
	@param s the string to add.
	@return a pointer to the string in the pool.
     */
    string add(string s);

    /** Return non-zero if the given string is in the pool.
	@param s the string to search for
	@return non-zero if s is in the pool
     */
    int contains(string s);

    /** Obtain a NULL-terminated list of all strings in the
	pool.  This list must be delete[]d by the caller.
	@return a list of strings
     */
    string* elements();

    /** Return the current number of unique strings contained.
     */
    int size() { return count; }

    /** Print out the contents for debugging.
     */
    void print();

private:
    StringNode* root;
    int count;
};

#endif // __StringPool__
