# libi86

[![(Travis CI build status)](https://travis-ci.org/tkchia/libi86.svg?branch=master)](https://travis-ci.org/tkchia/libi86) · [![(GitLab pipeline status)](https://gitlab.com/tkchia/libi86/badges/master/pipeline.svg)](https://gitlab.com/tkchia/libi86/-/commits/master)

An attempt to reimplement non-standard C library facilities (e.g. `<conio.h>`) commonly used in classical MS-DOS programs, for use with a [GCC toolchain for 16-bit x86](https://github.com/tkchia/build-ia16/).

The current aim is to be compatible enough with the [Open Watcom](https://github.com/open-watcom/open-watcom-v2/) runtime — as described in the [_Open Watcom C Library Reference_](https://github.com/open-watcom/open-watcom-v2-wikidocs/blob/master/docs/clib.pdf) — to be useful for building existing MS-DOS code.

Defining the macro `_BORLANDC_SOURCE` will also enable some degree of compatibility with the [Borland Turbo C++](http://cc.embarcadero.com/Item/25636) compiler's C library, which is described in the [_Borland C++ 2.0 Library Reference_](https://archive.org/details/bitsavers_borlandborn2.0LibraryReference1991_17218611).

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

## Legend

|     | Meaning
| --: | :------
|   W | Behaves like the corresponding function in Open Watcom.
|   + | Behaves like the corresponding function in Open Watcom, but with some extended behaviours.
|   B | From Borland Turbo C++ — enable with `_BORLANDC_SOURCE`.
| W/B | By default, behaves as in Open Watcom; if `_BORLANDC_SOURCE` is defined, behaves as in Borland C++.
|  IW | From internal interfaces in Open Watcom's library code.
|   X | `libi86`-specific extension; not in Open Watcom or Borland C++.

## Implemented functions

| Compat.    | Function | Notes
| ---------: | :------- | :----
|     | **`▗▚▚▚▚ <bios.h> ▞▞▞▞▖`**
|   + | `_bios_disk (`_service_`,` \*_diskinfo_`);` | As an extension, also accepts _service_ = `_DISK_DRIVEPARAMS`, which returns drive parameters in \*_diskinfo_.
|   W | `_bios_equiplist ();`
|   B | `biosequip ();`
|   W | `_bios_memsize ();`
|   B | `biosmemory ();`
|   W | `_bios_keybrd (`_service_`);`
|   B | `bioskey (`_service_`);`
|   W | `_bios_timeofday (`_service_`,` \*_timeval_`);`
|   X | `_bios_joystick (`_service_`,` \*_joyinfo_`);` | Reads joystick status via `int 0x15` function `0x84`.
|     |
|     | **`▗▚▚▚▚ <conio.h> ▞▞▞▞▖`** | **If `_BORLANDC_SOURCE` is defined, `<conio.h>` switches to an alternate implementation of the console output routines which is based on `<graph.h>` facilities.**
| W/B | \*`cgets (`\*_buf_`);`
| W/B | `cprintf (`\*_fmt_`, ...);`
| W/B | `cputs (`\*_buf_`);`
| W/B | `cscanf (`\*_fmt_`, ...);`
|   W | `getch ();`
|   W | `_getch ();`
| W/B | `getche ();`
|   W | `_getche ();`
|   W | `kbhit ();`
|   W | `_kbhit ();`
|   W | `ungetch (`_ch_`);`
|   W | `_ungetch (`_ch_`);`
| W/B | `putch (`_ch_`);`
| W/B | `vcprintf (`\*_fmt_`,` _ap_`);`
| W/B | `vcscanf (`\*_fmt_`,` _ap_`);`
|     |
|   B | `clreol ();`
|   B | `clrscr ();`
|   B | `gotoxy (`_x_`,` _y_`);`
|   B | `highvideo ();`
|   B | `insline ();`
|   B | `lowvideo ();`
|   B | `normvideo ();`
|   B | `textmode (`_mode_`);` | Does not support _mode_ = `LASTMODE` yet.
|   B | `wherex ();`
|   B | `wherey ();`
|   B | `window (`_left_`,` _top_`,` _right_`,` _bottom_`);`
|     |
|   W | `inp (`_port_`);`
|   W | `_inp (`_port_`);`
|   B | `inportb (`_port_`);`
|   W | `inpw (`_port_`);`
|   W | `_inpw (`_port_`);`
|   B | `inportw (`_port_`);`
|   W | `outp (`_port_`,` _value_`);`
|   W | `_outp (`_port_`,` _value_`);`
|   B | `outportb (`_port_`,` _value_`);`
|   W | `outpw (`_port_`,` _value_`);`
|   W | `_outpw (`_port_`,` _value_`);`
|   B | `outportw (`_port_`,` _value_`);`
|     |
|     | **`▗▚▚▚▚ <dos.h> ▞▞▞▞▖`** | **`<dos.h>` also includes `<i86.h>`, described below.**
|   W | `bdos (`_dos-func_`,` _dx_`,` _al_`);`
|   W | `bdosptr (`_dos-func_`,` \*_dx_`,` _al_`);`
|   W | `intdos (`\*_in-regs_`,` \*_out-regs_`);`
|   W | `intdosx (`\*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
|   + | `_dos_allocmem (`_size_`,` \*_segment_`);` | Also works under DPMI; yields a starting protected-mode selector.
|   W | `_dos_close (`_handle_`);`
|   + | `_dos_freemem (`_segment_`);` |  Also works under DPMI; accepts a starting protected-mode selector.
|   W | `_dos_getdrive (`\*_drive_`);`
|   W | `_dos_getfileattr (`\*_path_`,` \*_attributes_`);`
|   W | `_dos_setfileattr (`\*_path_`,` _attributes_`);`
|   W | `_getdrive ();`
|     |
|     | **`▗▚▚▚▚ <dpmi.h> ▞▞▞▞▖`** | **Except for `__DPMI_hosted ()`, functions in `<dpmi.h>` should only be called when the caller knows it is running in DPMI mode.**
|  IW | `__DPMI_hosted ();` | Returns 1 if running in protected mode under DPMI, -1 otherwise.  If the underlying C library has an implementation of this function, `libi86` will use that instead.
|  IW | `_DPMIGetDescriptor (`_sel_`,` \*_desc_`);` | `int 0x31` function `0x000b`.  Returns 0 on success, -1 on error.
|  IW | `_DPMISegmentToDescriptor (`_seg-para_`);` | `int 0x31` function `0x0002`.  On success, returns a protected-mode selector value for the real-mode segment _seg-para_`:0`.  On failure, returns a negative value.
|  IW | `_DPMISimulateRealModeInterrupt (`_inter-no_`,` _reset_`,` _words-to-copy_`,` \*_call-struct_`);` | `int 0x31` function `0x0300`.  Returns 0 on success, -1 on error.  _words-to-copy_ should probably be 0.
|     |
|     | **`▗▚▚▚▚ <graph.h> ▞▞▞▞▖`** | **Unlike in Open Watcom, where all functions in `<graph.h>` are far, in `libi86` the far-ness of functions follows the chosen memory model.  Thus, in a small-memory-model program, `_setvideomode` is a near function.  However, pointers to data are still far.**
|   W | `_clearscreen (`_area_`);`
|   W | `_gettextposition ();`
|   X | `_getvideomode ();`
|   W | `_outmem (`\*_text_`,` _length_`);`
|   W | `_outtext (`\*_text_`);`
|   W | `_scrolltextwindow (`_rows_`);`
|   W | `_settextposition (`_row_`,` _col_`);`
|   W | `_settextwindow (`_row1_`,` _col1_`,` _row2_`,` _col2_`);`
|   W | `_setvideomode (`_mode_`);` | In the case of SuperVGA screen modes, only works with VESA interface.
|     |
|     | **`▗▚▚▚▚ <i86.h> ▞▞▞▞▖`**
|   W | `delay (`_ms_`);`
|   W | `nosound ();`
|   W | `sound (`_freq_`);`
|   W | `segread (`\*_seg-regs_`);`
|   W | `_disable ();`
|   W | `_enable ();`
|   W | `int86 (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);`
|   W | `int86x (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
|   W | `intr (`_inter-no_`,` \*_regs_`);`
|   X | `_int86f (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);` | Loads `SZAPC` flags before issuing interrupt.
|   X | `_int86xf (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);` | Loads `SZAPC` flags before issuing interrupt.
|   X | `_intrf (`_inter-no_`,` \*_regs_`);` | Loads `SZAPC` flags before issuing interrupt.
|   W | `FP_OFF (`\*_ptr_`);` | Macro.
|   W | `_FP_OFF (`\*_ptr_`);` | Macro.
|   W | `FP_SEG (`\*_ptr_`);` | Macro.
|   W | `_FP_SEG (`\*_ptr_`);` | Macro.
|   W | \*`MK_FP (`_seg_`,` _off_`);` | Macro.
|   W | \*`_MK_FP (`_seg_`,` _off_`);` | Macro.
|     |
|     | **`▗▚▚▚▚ <libi86/string.h> ▞▞▞▞▖`**
|   W | \*`_fmemcpy (`\*_dest_`,` \*_src_`,` _n_`);`
|   W | \*`_fmemmove (`\*_dest_`,` \*_src_`,` _n_`);`
|   W | `_fstrlen (`\*_s_`);`

## Implemented variables

| Compat.    | Variable | Notes
| ---------: | :------- | :----
|     | **`▗▚▚▚▚ <libi86/stdlib.h> ▞▞▞▞▖`**
|   W | `_osmajor`
|   W | `_osminor`
|   W | `_psp`
