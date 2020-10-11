# libi86

[![(Travis CI build status)](https://travis-ci.org/tkchia/libi86.svg?branch=master)](https://travis-ci.org/tkchia/libi86) · [![(GitLab pipeline status)](https://gitlab.com/tkchia/libi86/badges/master/pipeline.svg)](https://gitlab.com/tkchia/libi86/-/commits/master)

An attempt to reimplement non-standard C library facilities (e.g. `<conio.h>`) commonly used in classical MS-DOS programs, for use with a [GCC toolchain for 16-bit x86](https://github.com/tkchia/build-ia16/).

The current aim is to be compatible enough with the [Open Watcom](https://github.com/open-watcom/open-watcom-v2/) runtime — as described in the [_Open Watcom C Library Reference_](https://github.com/open-watcom/open-watcom-v2-wikidocs/blob/master/docs/clib.pdf) — to be useful for building existing MS-DOS code.

Defining the macro `_BORLANDC_SOURCE` will also enable some degree of compatibility with the [Borland Turbo C++](http://cc.embarcadero.com/Item/25636) compiler's C library.

## Synopsis

#### Use

`ia16-elf-gcc` [..._gcc-options_...] `-li86` [...]

#### Installing from pre-compiled Ubuntu Linux packages, for cross development

Grab the `gcc-ia16-elf` and `libi86-ia16-elf` packages from [my `build-ia16` PPA](https://launchpad.net/~tkchia/+archive/ubuntu/build-ia16/).

#### Building and installing from sources, on Linux

 1. &nbsp;`git clone https://gitlab.com/tkchia/libi86.git`

 2. &nbsp;Install `gcc-ia16-elf`.

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

## License

`libi86` is now distributed under the [GNU Lesser General Public License version 2](COPYING2.LIB) or above.

## Implemented functions

### Legend

  * +: behaves like the corresponding function in Open Watcom, but with some extended behaviours.
  * BC: from Borland Turbo C++ — enable with `_BORLANDC_SOURCE`.
  * IW: from internal interfaces in Open Watcom's library code.
  * X: `libi86`-specific extension; not in Open Watcom or Borland C++.

### `<bios.h>`

  * `_bios_disk (`_service_`,` \*_diskinfo_`);` &nbsp; // [+](#legend)
    - As an extension, also accepts _service_ = `_DISK_DRIVEPARAMS`, which returns drive parameters in \*_diskinfo_.
  * `_bios_equiplist ();` \
    `biosequip ();` &nbsp; // [BC](#legend)
  * `_bios_memsize ();` \
    `biosmemory ();` &nbsp; // [BC](#legend)
  * `_bios_keybrd (`_service_`);` \
    `bioskey (`_service_`);` &nbsp; // [BC](#legend)
  * `_bios_timeofday (`_service_`,` \*_timeval_`);`
  * `_bios_joystick (`_service_`,` \*_joyinfo_`);` &nbsp; // [X](#legend)
    - Reads joystick status via `int 0x15` function `0x84`.

### `<conio.h>`

  * \*`cgets (`\*_buf_`);`
  * `cprintf (`\*_fmt_`, ...);`
  * `cputs (`\*_buf_`);`
  * `cscanf (`\*_fmt_`, ...);`
  * `_getch ();`
  * `_getche ();`
  * `_kbhit ();`
  * `_ungetch (`_ch_`);`
  * `putch (`_ch_`);`
  * `textmode (`_mode_`);` &nbsp; // [BC](#legend)
    - Does not support _mode_ = `LASTMODE` yet.
  * `vcprintf (`\*_fmt_`,` _ap_`);`
  * `vcscanf (`\*_fmt_`,` _ap_`);`
  * `inp (`_port_`);` \
    `_inp (`_port_`);` \
    `inportb (`_port_`);` &nbsp; // [BC](#legend)
  * `inpw (`_port_`);` \
    `_inpw (`_port_`);` \
    `inportw (`_port_`);` &nbsp; // [BC](#legend)
  * `outp (`_port_`,` _value_`);` \
    `_outp (`_port_`,` _value_`);` \
    `outportb (`_port_`,` _value_`);` &nbsp; // [BC](#legend)
  * `outpw (`_port_`,` _value_`);` \
    `_outpw (`_port_`,` _value_`);` \
    `outportw (`_port_`,` _value_`);` &nbsp; // [BC](#legend)

### `<dos.h>`

`<dos.h>` also includes `<i86.h>`.

  * `bdos (`_dos-func_`,` _dx_`,` _al_`);`
  * `bdosptr (`_dos-func_`,` \*_dx_`,` _al_`);`
  * `intdos (`\*_in-regs_`,` \*_out-regs_`);`
  * `intdosx (`\*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
  * `_dos_allocmem (`_size_`,` \*_segment_`);` &nbsp; // [+](#legend)
    - Also works under DPMI; yields a starting protected-mode selector.
  * `_dos_close (`_handle_`);`
  * `_dos_freemem (`_segment_`);` &nbsp; // [+](#legend)
    - Also works under DPMI; accepts a starting protected-mode selector.
  * `_dos_getdrive (`\*_drive_`);`
  * `_dos_getfileattr (`\*_path_`,` \*_attributes_`);`
  * `_dos_setfileattr (`\*_path_`,` _attributes_`);`
  * `_getdrive ();`

### `<dpmi.h>`

  * `__DPMI_hosted ();` &nbsp; // [IW](#legend)
    - Returns 1 if running in protected mode under DPMI, -1 otherwise.
    - If the underlying C library has an implementation of this function, `libi86` will use that instead.

Code should only use the following functions if it knows it is running in DPMI mode.

  * `_DPMIGetDescriptor (`_sel_`,` \*_desc_`);` &nbsp; // [IW](#legend)
    - Returns 0 on success, -1 on error.
  * `_DPMISegmentToDescriptor (`_seg-para_`);` &nbsp; // [IW](#legend)
    - On success, returns a protected-mode selector value for the real-mode segment _seg-para_`:0`.  On failure, returns a negative value.
  * `_DPMISimulateRealModeInterrupt (`_inter-no_`,` _reset_`,` _words-to-copy_`,` \*_call-struct_`);` &nbsp; // [IW](#legend)
    - Returns 0 on success, -1 on error.
    - _words-to-copy_ should probably be 0.

### `<graph.h>`

Unlike in Open Watcom, where all functions in `<graph.h>` are far, in `libi86` the far-ness of functions follows the chosen memory model.  Thus, in a small-memory-model program, `_setvideomode` is a near function.  However, pointers to data are still far.

  * `_gettextposition ();`
  * `_getvideomode ();` &nbsp; // [X](#legend)
  * `_scrolltextwindow (`_rows_`);`
  * `_setvideomode (`_mode_`);`
    - In the case of SuperVGA screen modes, only works with VESA interface.

### `<i86.h>`

  * `delay (`_ms_`);`
  * `nosound ();`
  * `sound (`_freq_`);`
  * `segread (`\*_seg-regs_`);`
  * `_disable ();`
  * `_enable ();`
  * `int86 (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);`
  * `int86x (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
  * `intr (`_inter-no_`,` \*_regs_`);`
  * `_int86f (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);` &nbsp; // [X](#legend)
    - Loads `SZAPC` flags before issuing interrupt.
  * `_int86xf (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);` &nbsp; // [X](#legend)
    - Loads `SZAPC` flags before issuing interrupt.
  * `_intrf (`_inter-no_`,` \*_regs_`);` &nbsp; // [X](#legend)
    - Loads `SZAPC` flags before issuing interrupt.
  * `FP_OFF (`\*_ptr_`);` \
    `_FP_OFF (`\*_ptr_`);`
    - Macro.
  * `FP_SEG (`\*_ptr_`);` \
    `_FP_SEG (`\*_ptr_`);`
    - Macro.
  * `MK_FP (`_seg_`,` _off_`);` \
    `_MK_FP (`_seg_`,` _off_`);`
    - Macro.

### `<libi86/string.h>`

  * \*`_fmemcpy (`\*_dest_`,` \*_src_`,` _n_`);`
  * \*`_fmemmove (`\*_dest_`,` \*_src_`,` _n_`);`

## Implemented variables

### `<libi86/stdlib.h>`

  * `_osmajor`
  * `_osminor`
  * `_psp`
