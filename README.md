# riff
A lightweight software rendering library.  

The core motivator of the project is to provide a media layer that is **easy to use** following the KISS philosophy**, ****portable** such that components will run on the majority of systems, **lightweight** and small in both implementation and interface, and providing **sufficient** functionality for most applications.

### installation
To build riff:
```bash
$ make rebuild
```

This will create a file build/libriff.a which is a static library that can be linked to any program with:
```bash
$ gcc ... -lriff -L[dir containing libriff.a]
```

### functionality
*all still in progress*
* Audio
* Graphics (OpenGL)
* Input

### demos
see test/README.md
