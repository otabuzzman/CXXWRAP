# CXXWRAP
A JNI wrapper generator for C++ classes. According to the change log of  the [original repository on Sourceforge](https://cxxwrap.sourceforge.net/), development appears to have ended in 2006. This is an offshoot of the original repository to implement and maintain the changes necessitated by new C++ standards since then. However, its scope is limited to the needs of new versions of the [AA+ library](http://www.naughter.com/aa.html), a C++ implementation by PJ Naughter of the formulas in Jean Meeus' book [Astronomical Algorithms](https://www.amazon.com/Astronomical-Algorithms-Jean-Meeus/dp/0943396611). More specifically, it's therefore a JNI wrapper generator for the C++ classes found in AA+ that might work for others as well.

### Build
- Install GCC development tools including flex and bison (use [Cygwin](https://www.cygwin.com/) on Windows).
- Clone repository and run `./configure ; ./make` in top-level directory.

### Acknowledgements
Copyright (c) 1999, David Deaven<br>
GNU GENERAL PUBLIC LICENSE Version 2
