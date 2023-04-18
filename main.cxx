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
 * $Id: main.cxx,v 1.40 2006/12/17 04:08:11 deaven Exp $
 */

#include "Class.h"
#include "html.h"
#include "man.h"
#include "AnalyzeEmitter.h"
#include "DependEmitter.h"
#include "JNIEmitter.h"
#ifdef WIN32
#define LEAN_AND_MEAN
#include <windows.h>
#endif

#define CACHE_VERSION "cxxwrap cache v0.1"

static void print_usage(char* myname)
{
    printf(
"C++ wrapper generator (c)1998-2002 David Deaven\n"
"usage: %s [options] file [file ...]\n"
"options:\n"
"  --help     print this help\n"
"  --root=<path>\n"
"             set the path prefix for subsequent header files\n"
"  --files=<file>\n"
"             read files to process from <file>.\n"
"  --template-package=[parameter|invoking|auto]\n"
"             specify template instantiation package.\n"
"  --package-prefix=<path>\n"
"	      specify a prefix for generated Java packages\n"
"  --html     create HTML API docs for all C++ classes\n"
"  --htmlpath=<path>\n"
"             specify the path to write HTML pages to\n"
"  --html-printable\n"
"             write simplified html suitable for printing\n"
"  --man      create manual pages for all C++ classes\n"
"  --manpath=<path>\n"
"             specify the path to write manual pages into\n"
"  --public-doc\n"
"             only document public methods, not protected or private ones\n"
"  --jni      create JNI Java wrappers for all C++ classes\n"
"  --classpath=<path>\n"
"             specify the Java classpath; new Java/C++ sources go\n"
"             here (defaults to current directory)\n"
"  --jni-makefile=<file>\n"
"             write a makefile fragment to the named file, defining\n"
"             the make variable \"JNIOBJ\".\n"
"  --jni-packages=<p1>[,<p2>]\n"
"             restrict the packages actually generated\n"
"  --jni-import-hook=<file>\n"
"             specify Java code to be inserted after import statements.\n"
"  --jni-begin-hook=<file>\n"
"             specify Java code to be inserted at the beginning of\n"
"             each Java method, before native invocations.\n"
"  --jni-end-hook=<file>\n"
"             specify Java code to be inserted at the end of\n"
"             each Java method, after native invocations.\n"
"  --jni-callbacks\n"
"             Enable automatic JNI wrapping of callback functions\n"
"  --jni-attributes\n"
"             Enable automatic JNI wrapping of attributes as accessor methods\n"
"  --jni-operators\n"
"             Enable automatic JNI wrapping of operators\n"
"  --read-cache=<file>\n"
"             read class cache in <file>.\n"
"  --write-cache=<file>\n"
"             write class cache to <file>.\n"
"  --cache-comments\n"
"             write comments on classes and methods to cache.\n"
"  --analyze=<file>\n"
"             write a toolkit API documentation analysis to the named file.\n"
"  --depend=<file>\n"
"             write a dependency analysis to the named file\n"
"  --verbose  print information about activities to standard output.\n"
"  --trace    print all lexical tokens (for debugging parser)\n"
    , myname);
}

globals global;

int main(int argc, char** argv)
{
    int trace = 0;
    int jni = 0;
    int html = 0;
    int html_printable = 0;
    int man = 0;
    const char* root = ".";
    const char* classpath = "";
    const char* htmlpath = ".";
    const char* manpath = ".";
    const char* analyzepath = NULL;
    const char* dependpath = NULL;
    const char* writecache = NULL;
    const char* s;
    StringPool files;

    for (int arg = 1; arg < argc; arg++)
    {
	if (!strcmp("--trace", argv[arg]))
	{
	    trace = 1;
	}
	else if ((s = argval("--root", argv[arg])))
	{
	    root = s;
	}
	else if ((s = argval("--template-package", argv[arg])))
	{
	    if (!strcmp(s, "parameter"))
	    {
		ClassJar::templateMode = ClassJar::PARAMETER_PACKAGE;
	    }
	    else if (!strcmp(s, "invoking"))
	    {
		ClassJar::templateMode = ClassJar::INVOKING_PACKAGE;
	    }
	    else if (!strcmp(s, "auto"))
	    {
		ClassJar::templateMode = ClassJar::AUTO_PACKAGE;
	    }
	}
	else if ((s = argval("--package-prefix", argv[arg])))
	{
	    Class::setPackagePrefix(s);
	}
	else if (!strcmp("--jni-callbacks", argv[arg]))
	{
	    global.callbacks++;
	}
	else if (!strcmp("--jni-attributes", argv[arg]))
	{
	    global.attributes++;
	}
	else if (!strcmp("--verbose", argv[arg]))
	{
	    global.verbose++;
	}
	else if ((s = argval("--classpath", argv[arg])))
	{
	    classpath = s;
	}
	else if ((s = argval("--htmlpath", argv[arg])))
	{
	    htmlpath = s;
	}
	else if ((s = argval("--manpath", argv[arg])))
	{
	    manpath = s;
	}
	else if (!strcmp("--jni", argv[arg]))
	{
	    jni = 1;
	}
	else if ((s = argval("--write-cache", argv[arg])))
	{
	    writecache = s;
	}
	else if ((s = argval("--read-cache", argv[arg])))
	{
	    FILE* f = fopen(s, "rb");
	    if (f == NULL)
	    {
		fprintf(stderr, "Cannot open cache file \"%s\"\n", s);
		return 1;
	    }
	    else
	    {
		const char* version = Token::readStringFrom(f);
		if (!version || strcmp(version, CACHE_VERSION))
		{
		    printf("Incompatible cache \"%s\", not reading\n", s);
		}
		else
		{
		    int n = Token::readIntFrom(f);
		    printf("reading %d classes from cache \"%s\"\n", n, s);
		    while (n > 0)
		    {
			Class* c = (Class*) Token::readTokenFrom(f);
			if (c)
			{
			    global.classJar.add(c);
			    if (global.verbose)
			    {
				int nm = 0;
				Method* m;
				for (m = c->methods; m; m = m->next) nm++;

				printf("read cached class \"%s\" (%d methods) from \"%s\"\n",
				    c->fullName(), nm, s);
			    }
			}
			n--;
		    }
		}

		fclose(f);
	    }
	}
	else if (!strcmp("--cache-comments", argv[arg]))
	{
	    Token::cache_comments = 1;
	}
	else if (!strcmp("--html", argv[arg]))
	{
	    html = 1;
	}
	else if (!strcmp("--html-printable", argv[arg]))
	{
	    html_printable = 1;
	}
	else if (!strcmp("--public-doc", argv[arg]))
	{
	    global.public_doc = 1;
	}
	else if (!strcmp("--man", argv[arg]))
	{
	    man = 1;
	}
	else if ((s = argval("--analyze", argv[arg])))
	{
	    analyzepath = s;
	}
	else if ((s = argval("--depend", argv[arg])))
	{
	    dependpath = s;
	}
	else if (!strcmp("--help", argv[arg]) ||
		 !strcmp("-h", argv[arg]))
	{
	    print_usage(argv[0]);
	    return 0;
	}
	else if ((s = argval("--files", argv[arg])))
	{
	    FILE* fl = fopen(s, "r");
	    if (fl)
	    {
		char buf[FILENAME_MAX];
		while (fgets(buf, 512, fl))
		{
		    char* eol = strchr(buf, '\n');
		    if (eol) *eol = '\0';
		    eol = strchr(buf, ' ');
		    if (eol) *eol = '\0';
		    eol = strchr(buf, '\t');
		    if (eol) *eol = '\0';

		    if (buf[0] == PATHSEP)
		    {
			files.add(buf);
		    }
		    else
		    {
			char buf2[512];
			sprintf(buf2, "%s|%s", root, buf);
			files.add(buf2);
		    }
		}
		fclose(fl);
	    }
	    else
	    {
		fprintf(stderr, "cannot open header file \"%s\"\n", s);
		return 1;
	    }
	}
	else if (argv[arg][0] != '-')
	{
#ifdef WIN32
	    char pth[MAX_PATH];
	    char rt[MAX_PATH];
	    WIN32_FIND_DATA FindFileData;
	    HANDLE hFind;
	    BOOL more=TRUE;

	    if (strlen(argv[arg]) > 2 && ((argv[arg][0] == PATHSEP &&
					   argv[arg][1] == PATHSEP) ||
					  argv[arg][1] == ':') ||
					  (argv[arg][0] == '.' &&
					   argv[arg][1] == PATHSEP) ||
					  (argv[arg][0] == '.' &&
					   argv[arg][1] == '.' &&
					   argv[arg][2] == PATHSEP))
	    {
		// Fully qualified (or relative) path
		strcpy(pth, argv[arg]);
		strcpy(rt, argv[arg]);
		char* eop = strrchr(rt, PATHSEP);
		if (eop) *eop = '\0';
	    }
	    else
	    {
		// Just a file name specification
		sprintf(pth, "%s%c%s", root, PATHSEP, argv[arg]);
		strcpy(rt, root);
	    }
	    // Get first file
	    hFind = FindFirstFile(pth, &FindFileData);
	    if (hFind!=INVALID_HANDLE_VALUE)
	    {
		while (more)
		{
		    // Skip directories
		    if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		    {
			char buf[512];
			sprintf(buf, "%s|%s", rt, FindFileData.cFileName);
			files.add(buf);
		    }
		    // Get next file
		    more=FindNextFile(hFind, &FindFileData);
		}
		FindClose(hFind);
	    }
#else
	    if (argv[arg][0]== PATHSEP)
	    {
		files.add(argv[arg]);
	    }
	    else
	    {
		char buf[512];
		sprintf(buf, "%s|%s", root, argv[arg]);
		files.add(buf);
	    }
#endif
	}
    }

    // scan the headers
    int file_count = 0;
    const char** filelist = files.elements();
    if (filelist)
    {
	const char** f;
	for (f = filelist; *f; f++)
	{
	    char buf[FILENAME_MAX];
	    strcpy(buf, *f);

	    char* rt = NULL;
	    char* fn = buf;

	    char* p = strchr(fn, '|');
	    if (p)
	    {
		*p = '\0';
		rt = fn;
		fn = p + 1;
	    }

	    if (scan_cxx_file(rt, fn, trace) == 0)
	    {
		file_count++;
	    }
	}
	delete[] filelist;
    }

    printf("scanned %d header file%s.\n", file_count,
	file_count == 1 ? "" : "s");

    // fix up virtuals not declared explicitly as such

    Class::make_consistent(&(global.classJar));

    // write JNI wrappers

    if (jni)
    {
	JNIEmitter e(&global.classJar, classpath);
	e.emit(argc, argv);
    }

    // write HTML

    if (html)
    {
	if (html_printable)
	{
	    PrintableHTMLEmitter e(&global.classJar, htmlpath);
	    e.emit(argc, argv);
	}
	else
	{
	    HTMLEmitter e(&global.classJar, htmlpath);
	    e.emit(argc, argv);
	}
    }

    // write manual pages

    if (man)
    {
	ManualEmitter e(&global.classJar, manpath);
	e.emit(argc, argv);
    }

    // analyze

    if (analyzepath)
    {
	FILE* f = strcmp(analyzepath, "-") == 0 ?
	    stdout : fopen(analyzepath, "w");
	if (!f)
	{
	    fprintf(stderr, "%s: cannot open \"%s\"\n", 
		argv[0], analyzepath);
	    return 1;
	}
	else
	{
	    AnalyzeEmitter e(&global.classJar, f);
	    e.emit(argc, argv);
	    if (f != stdout) fclose(f);
	}
    }

    // depend

    if (dependpath)
    {
	FILE* f = strcmp(dependpath, "-") == 0 ?
	    stdout : fopen(dependpath, "w");
	if (!f)
	{
	    fprintf(stderr, "%s: cannot open \"%s\"\n", 
		argv[0], dependpath);
	    return 1;
	}
	else
	{
	    DependEmitter e(&global.classJar, f);
	    e.emit(argc, argv);
	    if (f != stdout) fclose(f);
	}
    }

    // cache write

    if (writecache)
    {
	FILE* f = fopen(writecache, "wb");
	if (!f)
	{
	    printf("could not open \"%s\"\n", writecache);
	}
	else
	{
	    printf("writing %d classes to cache \"%s\"\n",
		global.classJar.size(), writecache);

	    Token::writeStringTo(CACHE_VERSION, f);
	    Token::writeIntTo(global.classJar.size(), f);

	    Class* c;
	    ClassJarIterator iter(global.classJar.iterator());
	    while ((c = iter.next()))
	    {
		Token::writeTokenTo(c, f);
		if (global.verbose) {
		    int nm = 0;
		    Method* m;
		    for (m = c->methods; m; m = m->next) nm++;
		    printf("wrote class \"%s\" (%d methods) to cache\n",
			c->fullName(), nm);
		}
	    }

	    fclose(f);
	}
    }

    error_summary();
    return 0;
}
