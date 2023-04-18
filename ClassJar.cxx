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
 * $Id: ClassJar.cxx,v 1.10 2006/12/17 04:08:11 deaven Exp $
 */

#include <stdio.h>
#include "Class.h"
#include "ClassJar.h"

ClassJar::TemplateMode ClassJar::templateMode =
    ClassJar::AUTO_PACKAGE;

void ClassJar::add(Class* c)
{
    ClassNode* n = root;
    ClassNode* leaf = NULL;

    int r = 0;
    while (n)
    {
	r = strcmp(n->clazz->fullName(), c->fullName());
	if (r == 0) return;
	leaf = n;
	n = (r < 0 ? n->lesser : n->greater);
    }

    count++;

    if (leaf)
    {
	n = new ClassNode(c);
	if (r < 0) leaf->lesser = n;
	else       leaf->greater = n;
	return;
    }

    root = new ClassNode(c);
}

void ClassJar::addJar(ClassJar* other)
{
    ClassJarIterator iter(other->iterator());
    Class* c;
    while ((c = iter.next()))
    {
	add(c);
    }
}

Class* ClassJar::findClassByName(const char* s) const
{
    ClassNode* n;

    n = root;
    while (n)
    {
	int r = strcmp(n->clazz->fullName(), s);
	if (r == 0) return n->clazz;
	n = (r < 0 ? n->lesser : n->greater);
    }

    n = root;
    while (n)
    {
	int r = strcmp(n->clazz->name(), s);
	if (r == 0) return n->clazz;
	n = (r < 0 ? n->lesser : n->greater);
    }

    return NULL;
}

Class* ClassJar::findClassByType(CType* t, Class* author)
{
    const char* s = t->BareCTypeName();
    Class * c = findClassByName(s);
    if (c && !strcmp(s, c->fullName())) return c;

    if (evaluating_depends) return NULL;

    /* if this is a template expansion, we may need 
     * to expand it and append it to the list of classes.
     *
     * A catch is, we must have instances of all objects used as
     * args; we cannot expand a template with arguments made up of
     * unknown types, or the java jni generator will croak on
     * them.
     */
    if (t->is_object())
    {
	ObjectCType* oct = (ObjectCType*) t;
	if (oct->args)
	{
	    c = findClassByName(oct->objname); // template def.
	    if (c && c->parameters())
	    {
		/* check out the args for "expandability".  Keep track of the
		 * template argument source files, for later use as authoring
		 * rules dictate.
		 */
		Arg* a;
		StringPool contributors;
		for (a = oct->args; a; a = a->next)
		{
		    if (a->type->is_object())
		    {
			Class* rc = findClassByType(a->type, author);
			if (!rc) return NULL; // no expansion
			contributors.add(rc->type()->source_file);
		    }
		}

		/* Make a copy of the template to fill in ("expand").
		 */
		c = new Class(*c);

		/* automatic dependency check mode?
		 */
		if (templateMode == AUTO_PACKAGE)
		{
		    /* The package is currently set to the template
		     * source package.  Look through the parameters
		     * and adjust the package as necessary.
		     */
		    StringPool* pd = packageDepends(c->packageName());
		    for (a = oct->args; a; a = a->next)
		    {
			Class* rc = findClassByType(a->type, author);
			if (rc && !(pd->contains(rc->packageName())))
			{
			    c->type()->source_file = rc->type()->source_file;
			    c->setType(c->type());
			    delete pd;
			    pd = packageDepends(c->packageName());
			}
		    }
		    delete pd;
		}
		else if (templateMode == PARAMETER_PACKAGE &&
		    contributors.size() > 0)
		{
		    /* if there's at least one contributing package, use it
		     * as the author.  Otherwise, use the instantiating package.
		     */
		    string* e = contributors.elements();
		    c->type()->source_file = global.pool.add(e[0]);
		    c->setType(c->type()); // since it changed
		    delete e;
		}
		else if (author) 
		{
		    c->type()->source_file = author->type()->source_file;
		    c->setType(c->type()); // since it changed
		}

		c->expand(oct->args);

		add(c);
		return c;
	    }
	}
    }

    return NULL;
}

ClassJarIterator ClassJar::iterator()
{
    long bit = newbit();
    if (bit == 0)
    {
	fprintf(stderr, "cxxwrap internal error (bit=0x%lx) \"%s\":%d\n",
	    bit, __FILE__, __LINE__);
	abort();
    }
    return ClassJarIterator(this, bit);
}

StringPool* ClassJar::packageDepends(const char* package)
{
    StringPool* result = new StringPool();

    /* we have to set a mode to prevent infinite recursion,
     * as classes attempt to look up dependent classes
     * via this object instance.
     */
    evaluating_depends = 1;

    ClassJarIterator iter(iterator());
    Class* c;
    while ((c = iter.next()))
    {
	if (0 == strcmp(c->packageName(), package))
	{
	    StringPool* pd = c->packageDepends();
	    string* e = pd->elements();
	    int i;
	    for (i = 0; e[i]; i++)
	    {
		result->add(e[i]);
	    }
	    delete[] e;
	    delete pd;
	}
    }

    evaluating_depends = 0;
    return result;
}

StringPool* ClassJar::packages()
{
    StringPool* result = new StringPool();

    ClassJarIterator iter(iterator());
    Class* c;
    while ((c = iter.next()))
    {
	result->add(c->packageName());
    }

    return result;
}
