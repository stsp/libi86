#### Building and installing `libi86` from sources, on Linux

 1. &nbsp;`git clone https://gitlab.com/tkchia/libi86.git`

 2. &nbsp;Install `gcc-ia16-elf`.
    * If using Ubuntu, you can get `gcc-ia16-elf` from [my `build-ia16` PPA](https://launchpad.net/~tkchia/+archive/ubuntu/build-ia16/).
    * Alternatively, you can build the compiler [from sources too](https://github.com/tkchia/build-ia16).

 3. &nbsp;`(cd libi86 && ./autogen.sh)`

 4. &nbsp;`mkdir build-libi86`

 5. &nbsp;`cd build-libi86`

 6. &nbsp;`../libi86/configure --prefix=`_install-path_

 7. &nbsp;`make`

 8. &nbsp;`make check`

 9. &nbsp;`make install`

#### Customizing the test suite run

Instead of just saying `make check`, you can specify options to the test suite to control how the tests are run, via the `$(TESTSUITEFLAGS)` makefile variable.  E.g.:

  * &nbsp;`make check TESTSUITEFLAGS='--x-test-underlying --x-with-dosemu=/`_path-to_`/dosemu'`
  * &nbsp;`make check TESTSUITEFLAGS='16'` &nbsp;# run _only_ test #16
  * &nbsp;`make check TESTSUITEFLAGS='10 --x-test-underlying'`

To see a complete list of test suite options you can put into `$(TESTSUITEFLAGS)`, run

  * &nbsp;`make helpcheck`

To list all the test cases and their test numbers:

  * &nbsp;`make listcheck`
