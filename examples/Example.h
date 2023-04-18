
#ifndef __Example__
#define __Example__

#include "Foreigner.h"

#define MY_EXPORT_SPEC
#define an_unknown_expr 45

/**
 * This is a bona fide javadoc style comment.  The first
 * sentance is a description, and the correct tags
 * are provided.
 *
 * @author Dave Deaven
 * @version $Id: Example.h,v 1.47 2004/01/03 22:15:12 deaven Exp $
 * @see Sun's javadoc guide, located at
 * {@link http://java.sun.com/products/jdk/javadoc/writingdoccomments.html}
 *
 * @see <a href="http://www.microsoft.com">The Evil Empire</a>
 * @see O'Reilly and Associates "Java in a nutshell"
 * @see Example
 * @see #number
 * @see DerivedExample#blah
 * @goober any tag which follows the general syntax is at least handled.
 * @header #include <sys/types.h> // the header tag is a cxxwrap extension
 *
 * @package
 * Provides some quick examples of how cxxwrap operates.
 */
class MY_EXPORT_SPEC ExampleData {
public:
    enum MyEnumeration {
	// uno
	ONE = 1,
	// dos
	TWO,
	// tres
	THREE,
	// quatro
	FOUR,
	// asi asi
	SYMBOL = 45
    };

    /**
     * Make a new number.
     *
     * @param x the number to make
     */
    ExampleData(int x = 0) : value(x) { }

    // test a bug for unsigned and regular integral in ctors
    ExampleData(long x) : value((int)x) { }
    ExampleData(unsigned long x) : value((int)x) { }
    ~ExampleData() { };

    /**
     * Access the number.
     *
     * @return the number
     */
    int number() { return value; }

    /**
     * Set the number
     *
     * $param x the number to set
     */
    void number(int x) { value = x; }

    /**
     * Access twice the number. This second is not part of this
     * comment's summary sentance.
     *
     * @return 2 * N, where N is the number
     * @deprecated since multiplication was added to the C language
     */
    int twiceNumber() { return 2 * number(); }

    // attributes can also be commented, this is the value
    int value;

protected:

    int Pnumber() { return value; }
};

template<class Type>
class Parameterized {
public:
    Parameterized(Type* t) { }
    Parameterized() { }
    ~Parameterized() { }

    int foo() { return 0; }
    void bar(Type* t, int x) { }
};

template<class Type>
class DerivedParameterized : public Parameterized<Type> {
public:
    DerivedParameterized() { }
    DerivedParameterized(const DerivedParameterized& other) { }
};

class SPar : public Parameterized<ExampleData> {
public:
    SPar(ExampleData* t) : Parameterized<ExampleData>(t) { }
    void bonobo(ExampleData& n) { }
};

/** A callback class.
 */
class Callback {
public:
    Callback() { x = 34; }
    virtual void performAction() = 0;
    virtual int iPerformAction() = 0;
    virtual int idPerformAction(double d) = 0;
    virtual int objPerformAction(ExampleData* e) = 0;
    virtual const SPar* testConstFunc() = 0;
    void hiya() { printf("x = %d\n", x); }
    void setX(int xx) { x = xx; }
    int getX() const { return x; }
    int x;
};

/** This example class shows how cxxwrap can be used
    to generate documentation and java code for a
    C++ class.
    @header #include <sys/types.h> // added in the class comment
    @author Dave Deaven
    @date 19990409
 */
class Example {
public:
    Example() : cb(NULL) { }

    /** in general, any html tags with matching <b>start and end</b>
       tags are recognized. */
    Example(const ExampleData& init, int) : cb(NULL) { num.value = init.value; }

    /** Helen asked me to make <A HREF="blah">links work, too</A>.
     * but she probably wants {@link ExampleData} style links.
     @param init and {@link #booboo a link to booboo} embedded
     */
    Example(const ExampleData* init) : cb(NULL) { num.value = init->value; }
    ~Example() { }

    /** This is the definition of an enumerated type.
     */
    enum KnownNumber {
	/** This is an embedded enumeration comment.
	 */
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR = 4
    };

    enum SecondEnumeration {
	// different style of comments in this one
	BOBO,

	// checking a bug
	RINGO
    };

    // if you use strict C type checking for no args, we'll remove the
    // "void" spec.  Also note that this comment is
    // done with double slashes ("//") at the beginning of each line.
    int booboo(void) { printf("C++ hello, world\n"); return 0; }

    /** A simple function
	@param x the first argument
	@param y the second argument
	@param z the third argument
	@return a status
     */
    int foo(int x = 0, double y = 0.0, double z = an_unknown_expr)
	{
	    printf("x=%d y=%f z=%f\n", x, y, z);
	    return x;
	}

    int foo2(int* x, int len)
	{
	    printf("got %d ints, adding index to all\n", len);
	    int i;
	    for (i = 0; i < len; i++) x[i] += i;
	    return x[0];
	}

    const char* foo3(const char* s)
	{
	    printf("got string \"%s\"\n", s);
	    return "hi, java";
	}
    
    const char* foo4(const char* s) { return s; }

    int testDefaults(int x = ExampleData::SYMBOL) { return x; }
    int testDefaults2(char* x = NULL) { return 0; }

    // multiple comment styles are recognized
    int bar() { return foo(0); }

#if 0
    // this should not get JNI wrapped
    void unknownClassPointerArray(UnknownClass *array[3]);
#endif

    // this tests a bug
    enum { B, C } AnonEnumeratedVariable;

    /* this is fun.

       there should be two lines in this comment.
     */
    void fun(int x, float f, double g, unsigned int i, long l) { }

    /* this comment has stars at
     * the beginning of each line.
     */
    char* str(char* s) { return s; }

    // this method tests an argument formatting bug
    static int getter(int x, int y, int z, const char* q) { return x * y; }

    // C++ namespaces, some like them, I don't
#if 0
    ACE::Interior::Nest badPractice() { }
#endif

    // parser cruft, egads
    friend class JobState;
    friend struct SetID;
    friend int main(int,char**);

    /******************************************
     * This comment is loaded with stars, including a 
     * passel of 'em on the first line.
     *
     * The next line has'em too.
     *
     */
    void pointers(int* x, long* y, float* f, double* g) { } // this illustrated a wierd bug
    /* plain jane C comment on one line follows slash-slash */
    void explicit_arrays(int x[3]) { }

    void other_arrays(int x[]) { }

    void string_arrays(const char* files[], int len) {
	int i;
	for (i = 0; i < len; i++) printf("string %d=\"%s\"\n", i, files[i]);
    }

    double h(KnownNumber n) { return 0.0; }

    // java defines these methods, so they must be aliased in wrappers.
    Example* clone() { return NULL; }

    /** This is a type definition local to this class, which 
        can be documented here.
     */
    typedef unsigned long my_size_t;

    // this is a recursive typedef
    typedef my_size_t* my_size_t_ptr;

    // this function uses the local type definition
    void how_big(my_size_t so_big) { }
    void how_big_ptr(my_size_t_ptr so_big) { }

    Example& operator = (const Example& other) { return *this; }
    void operator += (const Example& other) { exampleData.value += other.exampleData.value; }
    operator int () { return exampleData.value; }

    /** template instantiation woes illustrated by these.
     */
    void ti(DerivedParameterized<int>& dp) { }

    void setCallback(Callback* x) { cb = x; }
    void callCallback() {
	if (cb) {
	    cb->performAction();
	    int i0, i1, i2;
	    i0 = cb->iPerformAction();
	    i1 = cb->idPerformAction(3.4);
	    ExampleData e(56);
	    i2 = cb->objPerformAction(&e);
	    printf("C++ {i,id,obj}PerformAction={%d, %d, %d}\n", i0, i1, i2);
	}
    }

    // test NULL across JNI
    Example* nullTest(Example* in) { 
	printf("C++ nullTest(0x%x) called\n", (int) in);
	return NULL;
    }

    // test string arrays
    void stringarrayfunc(char** a) {
	printf("C++ stringarrayfunc got \"%s\"\n", *a); fflush(stdout);
	*a = "C++ rocks!"; }

    // test 32/64 bit int sizes
    void test64(long* l) { l[0] = 64; l[1] = 65; }
    void test32(int* i) { i[0] = 32; i[1] = 33; }

    // proper documenting of multiple indirection levels
    void indy_object(Example** twostar) { }
    void indy_int(int*** threestar) { }

public:
    // A public member field containing an object instance
    ExampleData exampleData;

    // A public member field containing an object pointer
    ExampleData* exampleDataPtr;

    // A public member field containing a primitive type
    double doubleField;

    typedef void(*callback_func)(int i, ExampleData* ed, void* userdata);
    void registerCallback(void* userdata, callback_func cbfunc) {
	printf("registered a callback user data 0x%x\n", (int) userdata);
    }
    
protected:
    // a protected typedef
    typedef unsigned long my_protected_size_t;
    void how_big_protected(my_protected_size_t so_big) { }
    ExampleData num;
    Callback* cb;
};

class DerivedExample : public Example {
public:
    int blah(int x) { return x; }
};

class DerivedExample2 : public Example {
public:
    int blah(int x) { return 2 * x; }
};


/* Classes may be derived from classes which cxxwrap does not
   know about, "Foreigner" in this class. */
class Interloper : public Foreigner {
public:
    int foo() { return 0; }
    int zany(Parameterized<Example>& pe) { return 1; }
};

// check the inheritance tree for a derived class of an interloper
class DerivedInterloper : public Interloper {
public:
    float bar() { return 0.0; }
};

/** A hidden class <<NOT_API>>
 */
class PrivateHelper {
public:
    void fubar() { }
};

// test underscores in class names
class One_Two {
public:
    void linked() { printf("One_Two linked OK\n"); }
};
class One_ {
public:
    void linked() { printf("One_ linked OK\n"); }
};
class CAP_under {
public:
    void linked() { printf("CAP_under linked OK\n"); }
};

#endif // __Example__
