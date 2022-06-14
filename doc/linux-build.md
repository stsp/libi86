### Building and installing `libi86` from sources, on Linux, for IA-16 GCC

 1. &nbsp;`git clone https://gitlab.com/tkchia/libi86.git`

 2. &nbsp;Install `gcc-ia16-elf`.
    * If using Ubuntu, you can get `gcc-ia16-elf` from [my `build-ia16` PPA](https://launchpad.net/~tkchia/+archive/ubuntu/build-ia16/).
    * Alternatively, you can build the compiler [from sources too](https://github.com/tkchia/build-ia16).

 3. &nbsp;`mkdir build-libi86`

 4. &nbsp;`cd build-libi86`

 5. &nbsp;`../libi86/configure --prefix=`_install-path_

 6. &nbsp;`make`

 7. &nbsp;`make check`

 8. &nbsp;`make install`

### Building and installing `libi86` from sources, on Linux, for the Amsterdam Compiler Kit

 1. &nbsp;`git clone https://gitlab.com/tkchia/libi86.git`

 2. &nbsp;Install the ACK.
    * If using Ubuntu, you can get a _very unofficial_ `ack-compiler` package from [my `de-rebus` PPA](https://launchpad.net/~tkchia/+archive/ubuntu/de-rebus/).
    * Alternatively, you can build the compiler [from sources too](https://github.com/davidgiven/ack).

 3. &nbsp;`mkdir build-libi86`

 4. &nbsp;`cd build-libi86`

 5. &nbsp;`../libi86/configure --ack --prefix=`_install-path_

 6. &nbsp;`make`

 7. &nbsp;`make check` &nbsp;# optional

 8. &nbsp;`make install`

 9. &nbsp;`make installcheck` &nbsp;# optional; tests the installed copy

### Customizing the test suite run

Instead of just saying `make check` or `make installcheck`, you can specify options to the test suite to control how the tests are run, via the `$(TESTSUITEFLAGS)` makefile variable.  E.g.:

  * &nbsp;`make check TESTSUITEFLAGS='--x-test-underlying --x-with-dosemu=/`_path-to_`/dosemu'`
  * &nbsp;`make check TESTSUITEFLAGS='16'` &nbsp;# run _only_ test #16
  * &nbsp;`make installcheck TESTSUITEFLAGS='10 --x-test-underlying'`

To see a complete list of test suite options you can put into `$(TESTSUITEFLAGS)`, run

  * &nbsp;`make helpcheck`

To list all the test cases and their test numbers:

  * &nbsp;`make listcheck`

([↵](../README.asciidoc))
