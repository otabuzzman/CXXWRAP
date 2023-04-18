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
 * $Id: html.cxx,v 1.47 2006/12/17 04:08:11 deaven Exp $
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Class.h"
#include "EnumCType.h"
#include "CommentParser.h"
#include "JavadocLink.h"
#include "ClassJar.h"
#include "html.h"

/* html browser color definitions
 */
#define LIGHT_YELLOW "#FCFCAC"
#define LIGHT_PURPLE "#CCCCFF"
#define LIGHT_SILVER "#F0F0F0"

void HTMLEmitter::start_html(FILE* f, const char* title)
{
    fprintf(f, "<html><head><title>%s</title></head>\n"
	       "<body bgcolor=\"#ffffff\">\n", title);
    fprintf(f, "<center><h2>%s</h2></center>\n", title);
}

void HTMLEmitter::end_html(FILE* f)
{
    fprintf(f, "</body></html>\n");
}

void HTMLEmitter::emit_frame_index(FILE* f, const char* title)
{
    static char* src1 = 
	"<HTML>\n"
	"<HEAD>\n"
	"<TITLE>\n";
    static char* src2 =
	"</TITLE>\n"
	"</HEAD>\n"
	"<FRAMESET cols=\"20%,80%\">\n"
	"<FRAMESET rows=\"30%,70%\">\n"
	"<FRAME src=\"packages-frame.html\" name=\"packagesFrame\">\n"
	"<FRAME src=\"classes-frame.html\" name=\"classesFrame\">\n"
	"</FRAMESET>\n"
	"<FRAME src=\"index-noframes.html\" name=\"mainFrame\">\n"
	"</FRAMESET>\n";

    fprintf(f, "%s%s\n%s", src1, title, src2);
}

void HTMLEmitter::open_section(FILE*f, const char* title)
{
    fprintf(f,
	"<table border=\"1\" cellpadding=\"3\" "
	    "cellspacing=\"0\" width=\"100%%\">\n"
	"<tr bgcolor=\"%s\">"
	    "<td><font size=\"+2\"><b>%s</b></font></td></tr>\n",
	LIGHT_PURPLE, title);
}

void HTMLEmitter::open_section_line(FILE* f, const char* color)
{
    if (color == NULL) color = "white";
    fprintf(f, "<tr bgcolor=\"%s\"><td>", color);
}

void HTMLEmitter::close_section_line(FILE* f)
{
    fprintf(f, "</td></tr>\n");
}

void HTMLEmitter::close_section(FILE* f)
{
    fprintf(f, "</table>\n");
}

const char* HTMLEmitter::emit_link_text(FILE* f, const char* text)
{
    const char* eol = strchr(text, '}');
    if (!eol)
    {
	fputc((int) *text, f);
	return text;
    }

    int len = eol - text;

    char* buf = new char[len + 2];
    strncpy(buf, text, len);
    buf[len] = '\0';

    JavadocLink l(buf, jar);
    if (l.hrefText())
    {
	fprintf(f, "<a href=\"%s\">%s</a>", l.hrefText(), l.linkText());
    }
    else
    {
	fprintf(f, "%s", l.linkText());
    }

    delete[] buf;

    return eol;
}

const char*
HTMLEmitter::emit_comment_word(FILE* f, const char* first, const char* p)
{
    if (*p == '\n')
    {
	fputc((int) *p, f);

	const char* l = p + 1;
	while (*l != '\0' && isspace(*l)) l++;
	if (*l == '*') // skip leading stars
	{
	    p = l;
	}
    }
    else if (autolink && isalnum(*p) && (p == first || isspace(*(p - 1))))
    {
	// check for known class names
	char buf[TOKEN_BUFFER_SIZE];
	strncpy(buf, p, 127);
	char* s = buf;
	while (isalnum(*s)) s++;
	*s = '\0';
	if (jar->findClassByName(buf))
	{
	    fprintf(f, "<a href=\"%s.html\">%s</a>", buf, buf);
	    p += strlen(buf) - 1;
	}
	else
	{
	    fputc((int) *p, f);
	}
    }
    else if (strncmp(p, "{@link ", 7) == 0)
    {
	p = emit_link_text(f, p + 7);
    }
    else if (*p == '<')
    {
	if (is_html_tag(p))
	{
	    while (*p != '\0' && *p != '>')
	    {
		fputc((int) *(p++), f);
	    }
	    if (*p != '\0') fputc((int) *p, f);
	}
	else
	{
	    /* escape code to print the bracket text.
	     */
	    fprintf(f, "&#060");
	}
    }
    else if (*p == '>')
    {
	/* escape code to print the bracket text.
	 */
	fprintf(f, "&#062");
    }
    else
    {
	fputc((int) *p, f);
    }

    return p;
}

void HTMLEmitter::emit_gathered_tags(FILE* f, CommentTag* tags,
    const char* tag, const char* title)
{
    CommentTag* list = tags->enumerateTag(tag);
    if (list)
    {
	fprintf(f, "<dt><b>%s</b>\n<dd><dl>\n", title);
	CommentTag* t;
	for (t = list; t; t = t->next)
	{
	    fprintf(f, "\n<dt>");
	    emit_comment_text(f, t->value);
	}
	fprintf(f, "\n</dl>\n");
    }
    delete list;
}

void HTMLEmitter::emit_sees(FILE* f, CommentTag* tags)
{
    CommentTag* list = tags->enumerateTag("see");
    if (list)
    {
	fprintf(f, "<dt><b>See also:</b>\n<dd><ul>\n");
	CommentTag* t;
	for (t = list; t; t = t->next)
	{
	    JavadocLink link(t->value, jar);
	    if (link.hrefText())
	    {
		fprintf(f, "\n<li><a href=\"%s\">%s</a>",
		    link.hrefText(), link.linkText());
	    }
	    else
	    {
		fprintf(f, "\n<li>");
		emit_comment_text(f, link.linkText());
	    }
	}
	fprintf(f, "\n</ul>\n");
    }
    delete list;
}

void HTMLEmitter::emit_params(FILE* f, CommentTag* tags)
{
    CommentTag* list = tags->enumerateTag("param");
    if (list)
    {
	fprintf(f, "<dt><b>Parameters:</b>\n<dd><dl>\n");
	CommentTag* t;
	for (t = list; t; t = t->next)
	{
	    fprintf(f, "\n<dt><b>");
	    const char* p = t->value;
	    while (*p != '\0' && isspace(*p)) p++;
	    while (*p != '\0' && !isspace(*p)) fputc((int) *(p++), f);
	    fprintf(f, "</b> - ");
	    while (*p != '\0' && isspace(*p)) p++;
	    emit_comment_text(f, p);
	}
	fprintf(f, "\n</dl>\n");
    }
    delete list;
}

void HTMLEmitter::emit_class_link(FILE* f, const char* link, const char* clazz)
{
    char buf[TOKEN_BUFFER_SIZE];
    strcpy(buf, link);
    char* rightdot = strrchr(buf, '.');
    if (rightdot)
    {
	*rightdot = '\0';
	fprintf(f, "<a href=\"%s.html\"><var>%s</var>.%s</a>",
	    html_sub(clazz), buf, rightdot + 1);
    }
    else
    {
	fprintf(f, "<a href=\"%s.html\">%s</a>",
	    html_sub(clazz), html_sub(link));
    }
}

void HTMLEmitter::emit_comment(FILE* f, const char* comment)
{
    // printf("comment:\n%s\n", comment);
    CommentParser* cp = comment_parser(comment);

    // start a table for the comment
    fprintf(f, "<table border=\"0\">\n");

    // mark it as deprecated if it is
    CommentTag* tags = cp->tags();
    CommentTag* t;
    if (tags && (t = tags->findTag("deprecated")))
    {
	fprintf(f, "<tr><td><b>DEPRECATED:</b>\n<var>");
	emit_comment_text(f, t->value);
	fprintf(f, "</var>\n</td></tr>\n");
    }

    // write the body of the comment
    const char* p = cp->body();
    if (p)
    {
	fprintf(f, "<tr><td>\n");
	emit_comment_text(f, p);
	fprintf(f, "\n</td></tr>\n");
    }

    // write the comment tags
    if (tags)
    {
	fprintf(f, "<tr><td><dl>\n");

	// all of the parameters
	emit_params(f, tags);
	emit_gathered_tags(f, tags, "return", "Returns:");

	// anything but {deprecated,param,return,see}
	for (t = tags; t; t = t->next)
	{
	    // tags not to output in this section
	    static char* skip[] = {
		"param", "return", "deprecated", "see", "header", "package",
		NULL
	    };

	    int doit = 1;
	    char** s;
	    for (s = skip; *s; s++)
	    {
		if (strcmp(*s, t->tag) == 0)
		{
		    doit = 0;
		    break;
		}
	    }

	    if (doit)
	    {
		// first character upper case, add colon
		fprintf(f, "\n<dt><b>");
		fputc((int) toupper(*(t->tag)), f);
		fprintf(f, "%s:</b>\n", t->tag + 1);
		emit_comment_text(f, t->value);
	    }
	}

	// see also(s)
	emit_sees(f, tags);

	fprintf(f, "</dl></td></tr>\n");
    }

    fprintf(f, "</table>\n");

    delete cp;
}

void HTMLEmitter::emit_html_spaces(FILE*f, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
	fprintf(f, "&nbsp;");
    }
}

void HTMLEmitter::emit_method(FILE* fout, Class* c, Method* m)
{
    char buf[TOKEN_BUFFER_SIZE];
    sprintf(buf, "%s %s", m->type->CTypeName(), m->name);
    int len = strlen(buf);

    fprintf(fout, "<a name=\"%s\"> </a>\n<dl><dt><code><b>", m->name);
    const char* hbuf = html_sub(buf);
    emit_comment_text(fout, hbuf);

    if (m->type->modifiers & CType::M_ATTRIBUTE)
    {
	fprintf(fout, "<br></b></code>\n");
    }
    else
    {
	fprintf(fout, "(");
	Arg* a;
	for (a = m->args; a != NULL; a = a->next)
	{
	    if (a != m->args) emit_html_spaces(fout, len + 1);

	    const char* ctn = a->type->CTypeName();
	    emit_comment_text(fout, html_sub(ctn)); // get links to classes
	    fprintf(fout, " %s", a->name);

	    if (a->default_value)
	    {
		fprintf(fout, " = ");
		emit_comment_text(fout, a->default_value);
	    }

	    if (a->next)
	    {
		fprintf(fout, ",<br>\n");
	    }
	}
	fprintf(fout, ")<br></b></code>\n");
    }
    if (m->comment)
    {
	fprintf(fout, "<dd>");
	emit_comment(fout, m->comment);
    }
    else
    {
	/* Use the parent documentation if the derived
	   class's is missing. */

	Class* p;
	Method* pm;
	for (p = c->parentClass();
	     p && (pm = p->methodWithSignature(m));
	     p = p->parentClass())
	{
	    if (pm->comment)
	    {
		fprintf(fout, "<dd>");
		emit_comment(fout, pm->comment);
		break;
	    }
	}
    }
    fprintf(fout, "</dl>\n");
}

static int mi_cmp(Method** ma, Method** mb)
{
    int modsa = (*ma)->type->modifiers;
    int modsb = (*mb)->type->modifiers;

    // ctors first
    if ( (modsa & CType::M_CTOR) &&
        !(modsb & CType::M_CTOR)) return -1;
    if (!(modsa & CType::M_CTOR) &&
         (modsb & CType::M_CTOR)) return +1;
    
    // dtors next
    if ( (modsa & CType::M_DTOR) &&
        !(modsb & CType::M_DTOR)) return -1;
    if (!(modsa & CType::M_DTOR) &&
         (modsb & CType::M_DTOR)) return +1;

    // alphanum 
    return strcmp((*ma)->name, (*mb)->name);
}

static int cmp(Class** pa, Class** pb)
{
    Class *a = *pa;
    Class *b = *pb;
    return strcmp(a->fullName(), b->fullName());
}

static int ccmp(Class** pa, Class** pb)
{
    Class *a = *pa;
    Class *b = *pb;
    if (a->type()->source_file && b->type()->source_file)
    {
	const char *da = a->packageName();
	const char* db = b->packageName();
	int r = strcmp(da, db);
	if (r == 0)
	    return strcmp(a->name(), b->name());
	return r;
    }
    if (a->type()->source_file) return -1;
    if (b->type()->source_file) return +1;

    return strcmp(a->name(), b->name());
}

void HTMLEmitter::emit_method_index_item(FILE* fout, Method* m)
{
    const char* cname = m->type->CTypeName();
    if (*cname == '~') cname = "";

    fprintf(fout, "<tr><td align=\"right\" valign=\"top\"><font size=\"-1\"><code>%s</code></font></td>\n",
	cname);
    fprintf(fout, "<td><code><a href=\"#%s\">%s%s",
	m->name, m->type->modifiers & CType::M_DTOR ? "~" : "", m->name);

    if (!(m->type->modifiers & CType::M_ATTRIBUTE))
    {
	fprintf(fout, "(");
	Arg* a;
	for (a = m->args; a != NULL; a = a->next)
	{
	    const char* ctn = a->type->CTypeName();
	    if (strcmp(ctn, "void") == 0 && a->next == NULL)
	    {
		// "void" --> "" in strict typecheck C style
	    }
	    else
	    {
		fprintf(fout, "%s%s",
		    a->name, a->next ? ", " : "");
	    }
	}
	fprintf(fout, ")");
    }
    fprintf(fout, "</a></code>\n");
    if (m->comment)
    {
	fprintf(fout, "<br><font size=\"-1\">\n");
	fprintf(fout, "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
	CommentParser* cp = comment_parser(m->comment);
	emit_comment_text(fout, cp->summarySentance());
	fprintf(fout, "</font>\n");
	delete cp;
    }
    fprintf(fout, "</td>\n");
}

int HTMLEmitter::emit_method_summary(FILE* fout,
    Class* c, Method** mtab, int count)
{
    Method* m;

    open_section(fout, "Public Method and Attribute summary");
    open_section_line(fout);
    fprintf(fout, "<table border=\"1\" width=\"100%%\">\n");
    int i;
    int inherited_count = 0;
    for (i = 0; i < count; i++)
    {
	m = mtab[i];
	if (!(m->notes & Method::ENUM_DEFINE) &&
	    !c->isInheritedMethod(m) &&
	    !(m->type->modifiers & (CType::M_PRIVATE | CType::M_PROTECTED)))
	{
	    emit_method_index_item(fout, m);
	}

	if (c->isInheritedMethod(m))
	{
	    inherited_count++;
	}
    }
    fprintf(fout, "</table>\n");
    close_section_line(fout);

    if (inherited_count > 0)
    {
	open_section_line(fout, LIGHT_YELLOW);
	fprintf(fout, "Inherited Methods and Attributes");
	close_section_line(fout);
	open_section_line(fout, LIGHT_YELLOW);
	fprintf(fout, "<table border=\"0\">\n");
	for (i = 0; i < count; i++)
	{
	    m = mtab[i];
	    if (!(m->notes & Method::ENUM_DEFINE) &&
		c->isInheritedMethod(m) &&
		!(m->type->modifiers & (CType::M_PRIVATE | CType::M_PROTECTED)))
	    {
		emit_method_index_item(fout, m);
	    }
	}
	fprintf(fout, "</table>\n");
	close_section_line(fout);
    }
    close_section(fout);

    return 0;
}

int HTMLEmitter::emit_html_for_one(Class* c)
{
    /* check if we are supposed to suppress docs on this one
     */
    if (c->comment && strstr(c->comment, "<<NOT_API>>"))
    {
	return 1;
    }

    /* do not write docs for a template expansion
     */
    if (c->type()->args)
    {
	return 1;
    }

    // open appropriately-named files

    FILE *fout;
    
    {
	char buf[FILENAME_MAX];
	sprintf(buf, "%s/%s.html", path, c->name());
        fout = open_file(buf, "w");
	if (!fout) {
	    fprintf(stderr, "cannot open output file \"%s\"\n", buf);
	    return -1;
	}
    }

    printf("writing HTML for class %s\n", c->name());

    // write a file header

    {
	char buf[TOKEN_BUFFER_SIZE];
	sprintf(buf, "%s%s API", html_sub(c->fullName()), 
	    c->parameters() ? " template" : "");
	start_html(fout, buf);
    }

    if (c->comment) 
    {
	open_section(fout, "Description");
	open_section_line(fout);
	emit_comment(fout, c->comment);
	close_section_line(fout);
	close_section(fout);
    }

    open_section(fout, "Heritage");
    open_section_line(fout);
    fprintf(fout, "<pre>\n");
    emit_tree(fout, c);
    fprintf(fout, "</pre>\n");
    close_section_line(fout);

    ClassJar* djar = c->derived();
    if (djar->size() > 0)
    {
	open_section_line(fout);
	fprintf(fout, "Known directly derived classes: ");
	fprintf(fout, "<code>");
	int i, len;
	Class** ctab = sorted_class_list(djar, &len);
	Class* dc;
	for (i = 0; i < len; i++)
	{
	    dc = ctab[i];
	    if (i > 0) fprintf(fout, ", ");
	    emit_class_link(fout, dc->fullName(), dc->name());
	}
	delete[] ctab;
	fprintf(fout, "</code>\n");
	close_section_line(fout);
    }
    delete djar;

    open_section_line(fout);
    fprintf(fout,
	"<a href=\"class-diagram.html#%s\">Complete known class diagram</a>\n",
	c->rootClass()->name());
    close_section_line(fout);
    close_section(fout);

    if (c->type()->source_file)
    {
	const char* p = c->type()->source_file;
	while (isspace(*p) || *p == '.' || *p == '/') p++;
	open_section(fout, "Header file");
	open_section_line(fout);
	// look for a @header tag in the class comment
	if (c->comment)
	{
	    CommentParser* cp = comment_parser(c->comment);
	    CommentTag* t = cp->tags()->findTag("header");
	    if (t) 
	    {
		fprintf(fout, "<br><code>\n");
		emit_comment_text(fout, t->value);
		fprintf(fout, "</code>\n");
	    }
	    delete cp;
	}
	fprintf(fout, "<br><code>#include &#060%s&#062</code><p><br>\n", p);
	close_section_line(fout);
	close_section(fout);
    }

    // go through the methods, emitting code for each

    Method* mlist = c->completeMethodList();
    Method* m;
    Method** mtab;
    int count;
    count = 0;
    for (m = mlist; m; m = m->next) count++;
    mtab = new Method*[count];
    count = 0;
    for (m = mlist; m; m = m->next) mtab[count++] = m;

    // write an alphabetized index at the top
    qsort(mtab, count, sizeof(Method*), 
	(int (*)(const void *, const void *)) mi_cmp);

    emit_method_summary(fout, c, mtab, count);

    // main method listing is reverse order in mlist XXX
    {
	int tmp = count;
	for (m = mlist; m; m = m->next) mtab[--tmp] = m;
    }

    /* Do all of the type definitions.
     */
    int numTypedefs = 0;

    int i;
    for (i = 0; i < count; i++)
    {
	m = mtab[i];
	if (m->type->modifiers & CType::M_TYPEDEF)
	    numTypedefs++;
    }

    if (numTypedefs > 0)
    {
	open_section(fout, "Type Definitions");
	open_section_line(fout);

	for (i = 0; i < count; i++)
	{
	    m = mtab[i];

	    if (m->type->modifiers & CType::M_TYPEDEF)
	    {
		fprintf(fout, "<dl><dt><code><b>typedef %s %s</b></code>\n",
		    m->type->CTypeName(), m->name);
		if (m->comment)
		{
		    fprintf(fout, "<dd>");
		    emit_comment(fout, m->comment);
		    fprintf(fout, "<br>\n");
		}
		fprintf(fout, "</dl>\n");
	    }
	}

	close_section_line(fout);
	close_section(fout);
    }

    /* Do all of the enumerations.
     */
    int numEnumerations = 0;

    for (i = 0; i < count; i++)
    {
	m = mtab[i];
	if (m->notes & Method::ENUM_DEFINE)
	{
	    numEnumerations++;
	}
    }

    if (numEnumerations > 0)
    {
	open_section(fout, "Enumerations");
	open_section_line(fout);

	fprintf(fout, "<dl>");
	for (i = 0; i < count; i++)
	{
	    m = mtab[i];

	    if (m->notes & Method::ENUM_DEFINE)
	    {
		EnumCType* ect = (EnumCType*) m->type; // XXX
		fprintf(fout, "<dt><code><b>enum %s</b></code>\n<dd>",
		    ect->getName());
		if (m->comment)
		{
		    emit_comment(fout, m->comment);
		    fprintf(fout, "<br>\n");
		}
		fprintf(fout, "<i>Values:</i><br><table>\n");
		Arg* a;
		for (a = ect->getSymbols(); a != NULL; a = a->next)
		{
		    fprintf(fout, "<tr><td><code>%s</code></td>\n", a->name);
		    if (a->comment)
		    {
			fprintf(fout, "<td>");
			emit_comment(fout, a->comment);
			fprintf(fout, "</td>\n");
		    }
		}
		fprintf(fout, "</table>\n");
	    }
	}
	fprintf(fout, "</dl>\n");

	close_section_line(fout);
	close_section(fout);
    }

    open_section(fout, "Constructors");
    for (i = 0; i < count; i++)
    {
	m = mtab[i];
	if (m->type->modifiers & (CType::M_CTOR | CType::M_DTOR) &&
	    !(m->type->modifiers & CType::M_PRIVATE))
	{
	    open_section_line(fout);
	    emit_method(fout, c, m);
	    close_section_line(fout);
	}
    }
    close_section(fout);

    open_section(fout, "Public Methods and Attributes");
    int seen_inherited = 0;
    for (i = 0; i < count; i++)
    {
	m = mtab[i];

	if (!(m->type->modifiers &
	    (CType::M_TYPEDEF |
	     CType::M_CTOR | CType::M_DTOR |
	     CType::M_PROTECTED | CType::M_PRIVATE)) &&
	    !(m->notes & Method::ENUM_DEFINE))
	{
	    if (c->isInheritedMethod(m) && !seen_inherited)
	    {
		seen_inherited = 1;
	    }
	    if (seen_inherited) {
		open_section_line(fout, LIGHT_YELLOW);
	    } else {
		open_section_line(fout);
	    }
	    emit_method(fout, c, m);
	    close_section_line(fout);
	}
    }
    close_section(fout);

    if (!global.public_doc)
    {
	open_section(fout, "Protected Methods and Attributes");
	for (i = 0; i < count; i++)
	{
	    m = mtab[i];
	    
	    if (!(m->type->modifiers &
		(CType::M_TYPEDEF |
		 CType::M_CTOR | CType::M_DTOR)) &&
		(m->type->modifiers & CType::M_PROTECTED) &&
		!(m->notes & Method::ENUM_DEFINE))
	    {
		open_section_line(fout);
		emit_method(fout, c, m);
		close_section_line(fout);
	    }
	}
	close_section(fout);
    }

    delete[] mtab;
    delete mlist;

    // close the file

    end_html(fout);
    fclose(fout);

    return 0;
}

void HTMLEmitter::emit_package_summary_file(FILE* f, const char* p)
{
    char buf[FILENAME_MAX];
    sprintf(buf, "%s%sPackageSummary.html", p,
	strlen(p) > 0 ? "/" : "");
    FILE* fs = open_file(buf, "r");
    if (fs)
    {
	int c;
	while ((c = fgetc(fs)) != EOF) fputc(c, f);
	fclose(fs);
    }
}

void HTMLEmitter::emit_package_summary(FILE* f,
    const char* hf, Class** ctab, int class_count)
{
    // emit package summary if summary file is present
    emit_package_summary_file(f, hf);

    // emit all embedded class package docs
    int j;
    for (j = 0; j < class_count; j++)
    {
	const char* tdn = ctab[j]->packageName();
	if (strcmp(hf, tdn) == 0 && ctab[j]->comment)
	{
	    CommentParser* cp = comment_parser(ctab[j]->comment);
	    CommentTag* t = cp->tags()->findTag("package");
	    if (t)
	    {
		emit_comment_text(f, t->value);
	    }
	    delete cp;
	}
    }
}

void HTMLEmitter::emit_package_index()
{
    int i;
    FILE* f;
    char hf[FILENAME_MAX];
    int package_serial;
    int package_count;

    /* sort so that the classes are alphabetized
     */
    qsort(ctab, class_count, sizeof(Class*),
	(int (*)(const void *, const void *)) cmp);

    /* make the class browser frame.
     */
    sprintf(hf, "%s/classes-frame.html", path);
    f = open_file(hf, "w");
    if (f == NULL)
    {
	fprintf(stderr, "cannot open file \"%s\"\n", hf);
	return;
    }
    start_html(f, "All classes");
    for (i = 0; i < class_count; i++)
    {
	/* do not include expanded templates or NOT_API classes.
	 */
	if (ctab[i]->type()->args == NULL &&
	    (ctab[i]->comment == NULL || !strstr(ctab[i]->comment, "<<NOT_API>>")))
	{
	    fprintf(f, "<code><a href=\"%s.html\" "
		    "target=\"mainFrame\">%s</a></code><br>\n",
		    ctab[i]->name(), html_sub(ctab[i]->fullName()));
	}
    }
    end_html(f);
    fclose(f);

    /* sort so that the classes are in order by package name.
     */
    qsort(ctab, class_count, sizeof(Class*),
	(int (*)(const void *, const void *)) ccmp);

    /* count the number of packages
     */
    *hf = '\0';
    package_count = 0;
    for (i = 0; i < class_count; i++)
    {
	const char* dn = ctab[i]->packageName();

	if (strcmp(hf, dn))
	{
	    package_count++;
	    strcpy(hf, dn);
	}
    }

    /* make the frames index
     */
    sprintf(hf, "%s/index.html", path);
    f = open_file(hf, "w");
    if (f == NULL)
    {
	fprintf(stderr, "cannot open file \"%s\"\n", hf);
	return;
    }
    emit_frame_index(f, "API index");
    fclose(f);

    /* make the package browser frame.
     */
    sprintf(hf, "%s/packages-frame.html", path);
    f = open_file(hf, "w");
    if (f == NULL)
    {
	fprintf(stderr, "cannot open file \"%s\"\n", hf);
	return;
    }
    start_html(f, "Packages");
    package_serial = 0;
    for (i = 0; i < class_count; i++)
    {
	const char* dn = ctab[i]->packageName();

	if (strcmp(hf, dn)) // next package starts here
	{
	    package_serial++;
	    fprintf(f, "<var><a href=\"index-noframes.html#package%d\" "
		"target=\"mainFrame\">%s</a></var><br>\n",
		package_serial, dn);
	    strcpy(hf, dn);
	}
    }
    end_html(f);
    fclose(f);

    /* make the noframes index
     */
    sprintf(hf, "%s/index-noframes.html", path);
    f = open_file(hf, "w");
    if (f == NULL)
    {
	fprintf(stderr, "cannot open HTML index file \"%s\"\n", hf);
	return;
    }

    start_html(f, "API index");
    fprintf(f, "Also see <a href=\"class-diagram.html\">Class diagram</a>.\n");

    /* run through once just to print the package names.
     */
    *hf = '\0';
    package_serial = 0;
    open_section(f, "Package summary");
    open_section_line(f);
    fprintf(f, "<table border=\"1\" width=\"100%%\">\n");
    for (i = 0; i < class_count; i++)
    {
	const char* dn = ctab[i]->packageName();

	if (strcmp(hf, dn)) // next package starts here
	{
	    package_serial++;
	    fprintf(f, "<tr bgcolor=\"%s\">"
		       "<td><a href=\"#package%d\"><i>%s</i></a></td>"
		       "\n<td>\n", LIGHT_SILVER, package_serial, dn);
	    strcpy(hf, dn);

	    emit_package_summary(f, hf, ctab, class_count);

	    fprintf(f, "</td></tr>\n");
	}
    }
    fprintf(f, "</table>\n");
    close_section_line(f);
    close_section(f);

    /* next pass, write a complete list of classes in each
     * package.
     */
    *hf = '\0';
    package_serial = 0;
    for (i = 0; i < class_count; i++)
    {
	Class* c = ctab[i];
	const char* dn = c->packageName();

	if (strcmp(hf, dn)) // next package starts here
	{
	    if (package_serial > 0)
	    {
		fprintf(f, "</dl>\n"); // close the last list
	    }
	    package_serial++;

	    fprintf(f, "<a name=\"package%d\"> </a>\n", package_serial);

	    char buf[TOKEN_BUFFER_SIZE];
	    sprintf(buf, "Classes in package <var>%s</var>", dn);
	    open_section(f, buf);
	    close_section(f);
	    fprintf(f, "<dl>\n"); // start a list
	    strcpy(hf, dn);
	}

	/* do not include expanded templates.
	 */
	if (c->type()->args == NULL)
	{
	    fprintf(f, "<dt><a href=\"%s.html\"><code>%s</code></a>\n",
		c->name(), html_sub(c->fullName()));
	}
    }
    if (package_serial > 0)
    {
	fprintf(f, "</dl>\n");
    }

    end_html(f);

    fclose(f);
}

int HTMLEmitter::emit_class_diagram(ClassJar* roots)
{
    char buf[FILENAME_MAX];
    sprintf(buf, "%s/class-diagram.html", path);
    FILE* f = open_file(buf, "w");
    if (!f)
    {
	fprintf(stderr, "cannot open output file \"%s\"\n", buf);
	return -1;
    }

    start_html(f, "Class diagram");
    fprintf(f, "Also see <a href=\"index-noframes.html\">API index</a>.\n");
    open_section(f, "Class diagram");
    open_section_line(f);

    int i, len;
    Class** ctab = sorted_class_list(roots, &len);
    for (i = 0; i < len; i++)
    {
	Class* c = ctab[i];
	fprintf(f, "<br><a name=\"%s\"> </a>\n<pre>\n", c->name());
	emit_sub_tree(f, c);
	fprintf(f, "</pre>\n");
    }
    delete[] ctab;

    close_section_line(f);
    close_section(f);
    end_html(f);
    fclose(f);


    return 0;
}

HTMLEmitter::HTMLEmitter(ClassJar* jar_, const char* path_) :
    Emitter(jar_),
    path(path_),
    autolink(0)
{
}

HTMLEmitter::~HTMLEmitter()
{
}

int HTMLEmitter::emit(int argc, char** argv)
{
    autolink = arg_in("--html-autolink", argc - 1, argv + 1);

    make_path(path);

    ClassJar roots;

    Class* c;
    ClassJarIterator iter(jar->iterator());
    while ((c = iter.next()))
    {
	emit_html_for_one(c);
	roots.add(c->rootClass());
    }

    emit_package_index();
    emit_class_diagram(&roots);

    return 0;
}

const char* 
HTMLEmitter::html_sub(const char* in)
{
    char buf[COMMENT_BUFFER_SIZE];
    int p = 0;

    for (; *in != '\0'; in++)
    {
	switch(*in)
	{
	    case '<':
		strcpy(buf + p, "&#060");
		p += 5;
		break;

	    case '>':
		strcpy(buf + p, "&#062");
		p += 5;
		break;

	    default:
		buf[p++] = *in;
		break;
	}
    }

    buf[p] = '\0';
    return pool.add(buf);
}

// ==============================================================

PrintableHTMLEmitter::PrintableHTMLEmitter(ClassJar* jar_, const char* path_) :
    HTMLEmitter(jar_, path_)
{
}

PrintableHTMLEmitter::~PrintableHTMLEmitter()
{
}

void PrintableHTMLEmitter::start_html(FILE* f, const char* title)
{
    fprintf(f, "<html><head><title>%s</title></head>\n"
	       "<body>\n", title);
    fprintf(f, "<h1>%s</h1>\n", title);
}

void PrintableHTMLEmitter::end_html(FILE* f)
{
    fprintf(f, "</body></html>\n");
}

void PrintableHTMLEmitter::open_section(FILE*f, const char* title)
{
    fprintf(f, "<h2>%s</h2>\n", title);
}

void PrintableHTMLEmitter::open_section_line(FILE* f, const char* /* color */)
{
    fprintf(f, "\n");
}

void PrintableHTMLEmitter::close_section_line(FILE* f)
{
    fprintf(f, "\n");
}

void PrintableHTMLEmitter::close_section(FILE* f)
{
    fprintf(f, "\n");
}

int PrintableHTMLEmitter::emit_method_summary(FILE* /* fout */,
    Class* /* c */, Method** /* mtab */, int /* count */)
{
    return 0;
}
