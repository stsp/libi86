### Running `libi86` 's self-test suite from pre-compiled Ubuntu Linux packages, for cross development

 1. Grab the `libi86-testsuite-ia16-elf` package from [my `build-ia16` PPA](https://launchpad.net/~tkchia/+archive/ubuntu/build-ia16/).

 2. Install [`dosemu2`](http://dosemu2.github.io/dosemu2/).

 3. &nbsp;`mkdir testing`

 4. &nbsp;`/usr/ia16-elf/libexec/libi86/tests/testsuite -C testing --x-installcheck`

### Customizing the test suite run

You can specify options to the test suite to control how the tests are run.  To see a complete list of test suite options:

  * &nbsp;`/usr/ia16-elf/libexec/libi86/tests/testsuite --help`

To list all the test cases and their test numbers:

  * &nbsp;`/usr/ia16-elf/libexec/libi86/tests/testsuite --list`

You should always specify `--x-installcheck` when running tests.

([↵](../README.asciidoc))
