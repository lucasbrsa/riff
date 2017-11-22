# riff
A ground up library for use in multimedia. Following the KISS (keep it simple stupid) philosophy, more specifically: implementation can be of any complexity as long as it is fast and reliable but interfaces should be simple.

### make guide
To build riff:
```bash
$ cd [dir where riff was cloned to]; make rebuild
```

This will create a file *build/libriff.a* which is a static library that can be linked to any program with:
```bash
$ gcc ... -lriff -L[dir containing libriff.a]
```

### additional build commands
+ `$ make clean` (remove libriff.a, object files and tests)
+ `$ make build` or `make` (build only if riff has changed since last build)
+ `$ make list` (list files in libriff)
+ `$ make run_[demo/test]` e.g. `$ make run_test` or `$ make run_demo_log` (build and run a .c in the test dir)
+ `$ make debug_[demo/test]` e.g. `$ make debug_test` (run a .c in the test dir with lldb)
+ `$ make valgrind_[demo/test]` (run a .c from test dir through valgrind)

### contribution workflow
+ make changes to files in include/ and src/
+ `$ make rebuild` and fix issues
+ write a test as *test/test_[something].h* and add it's call to *test/test.c*
+ `$ make run_test` and fix issues with `$ make debug_test` and `$ make valgrind_test`
+ commit

### demos
see test/README.md
