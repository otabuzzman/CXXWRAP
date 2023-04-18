import examples.*;

class AutoJCallback implements callback_funcCallback {
    public AutoJCallback() { }
    public void callback_func(int i, ExampleData ed) {
	System.out.println("AutoJCallback called with int " + i);
    }
}

class JCallback extends Callback {
    public JCallback()
    {
	super();
	setX(23);
    }

    public void performAction()
    {
	System.out.println("performAction() in java and x = " + getX());
    }
    public int iPerformAction()
    {
	System.out.println("iPerformAction() in java and x = " + getX());
	return 3;
    }
    public int idPerformAction(double x)
    {
	System.out.println("idPerformAction() in java and x = " + x);
	return (int) (x * 2.0);
    }
    public int objPerformAction(ExampleData e)
    {
	System.out.println("objPerformAction() in java and x = " + e.number());
	return (int) (e.number() * 2.0);
    }
}

public class JavaTest {

    public static void main(String argc[])
    {
	System.out.println("Java jello, world\n");
	Example e = new Example();

	e.booboo();

	e.foo(1, 2., 3.);
	int[] x = new int[3];
	x[0] = 1;
	x[1] = 2;
	x[2] = 3;
	e.foo2(x, x.length);
	if (x[0] != 1 || x[1] != 3 || x[2] != 5)
	    System.out.println("FAIL: java int array");

	String s = e.foo3("hello, C++");
	System.out.println("C++ returned \"" + s + "\"");
	s = e.foo4(null);
	System.out.println("C++ returned \"" + s + "\" for null");

	String args[] = new String[3];
	args[0] = "zero";
	args[1] = "one";
	args[2] = "two";
	e.string_arrays(args, 3);

	JCallback jcb = new JCallback();
	e.setCallback(jcb);
	e.callCallback();

	Example nullE = e.nullTest(null);
	System.out.println("nullTest(null) = " + nullE);

	{ One_Two obj = new One_Two(); obj.linked(); }
	{ One_ obj = new One_(); obj.linked(); }
	{ CAP_under obj = new CAP_under(); obj.linked(); }

	long[] lbuf = new long[2];
	e.test64(lbuf);
	if (lbuf[0] != 64 || lbuf[1] != 65)
	{
	    System.out.println("failed 64 bit test");
	}

	int[] ibuf = new int[2];
	e.test32(ibuf);
	if (ibuf[0] != 32 || ibuf[1] != 33)
	{
	    System.out.println("failed 32 bit test");
	}

	String[] sa = new String[2];
	e.stringarrayfunc(sa);
	System.out.println("stringarrayfunc returned \"" + sa[0] + "\"");

        // Test wrapping of public member fields
        e.exampleData().number();

	// Test automatic callbacks
	AutoJCallback ajc = new AutoJCallback();
	e.registerCallback(ajc);

        // Test operators
        Example e2 = new Example();
        e.exampleData().number(5);
        e2.exampleData().number(42);
        e.opAddEq(e2);
        if (e.exampleData().number() != 47)
        {
            System.out.println("failed operator+= test");
        }
        if (e2.opCastInt() != 42)
        {
            System.out.println("failed operator int test");
        }

	e.delete();
        e2.delete();
    }

    static {
	System.loadLibrary("wrapper");
    }
}

