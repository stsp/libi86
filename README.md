# libi86

[![(build status)](https://travis-ci.org/tkchia/libi86.svg?branch=master)](https://travis-ci.org/tkchia/libi86)

An attempt to reimplement non-standard C library facilities (e.g. `<conio.h>`) commonly used in classical MS-DOS programs, for use with a [GCC toolchain for 16-bit x86](https://github.com/tkchia/build-ia16/).

The current aim is to be compatible enough with the [Open Watcom](https://github.com/open-watcom/open-watcom-v2/) runtime — as described in the [_Open Watcom C Library Reference_](https://github.com/open-watcom/open-watcom-v2-wikidocs/blob/master/docs/clib.pdf) — to be useful for building existing MS-DOS code.

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

 6. &nbsp;`../libi86/configure --host=ia16-elf --prefix=`_install-path_ `--exec-prefix=`_install-path_`/ia16-elf --disable-elks-libc`

 7. &nbsp;`make`

 8. &nbsp;`make check`

 9. &nbsp;`make install`

## Implemented functions

### `<bios.h>`

  * `_bios_disk (`_service_`,` \*_diskinfo_`);`
    - as an extension, also accepts _service_ = `_DISK_DRIVEPARAMS`, which returns drive parameters in \*_diskinfo_
  * `_bios_equiplist ();`
  * `_bios_memsize ();`
  * `_bios_keybrd (`_service_`);`
  * `_bios_timeofday (`_service_`,` \*_timeval_`);`
  * `_bios_joystick (`_service_`,` \*_joyinfo_`);`
    - not in Open Watcom; reads joystick status via `int 0x15` function `0x84`

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
  * `vcprintf (`\*_fmt_`,` _ap_`);`
  * `vcscanf (`\*_fmt_`,` _ap_`);`
  * `inp (`_port_`);`
    - also `_inp`
  * `inpw (`_port_`);`
    - also `_inpw`
  * `outp (`_port_`,` _value_`);`
    - also `_outp`
  * `outpw (`_port_`,` _value_`);`
    - also `_outpw`

### `<dos.h>`

  * `bdos (`_dos-func_`,` _dx_`,` _al_`);`
  * `bdosptr (`_dos-func_`,` \*_dx_`,` _al_`);`
  * `intdos (`\*_in-regs_`,` \*_out-regs_`);`
  * `intdosx (`\*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
  * `_dos_allocmem (`_size_`,` \*_segment_`);`
  * `_dos_close (`_handle_`);`
  * `_dos_freemem (`_segment_`);`
  * `_dos_getdrive (`\*_drive_`);`
  * `_dos_getfileattr (`\*_path_`,` \*_attributes_`);`
  * `_dos_setfileattr (`\*_path_`,` _attributes_`);`
  * `_getdrive ();`
  * `<dos.h>` also includes `<i86.h>`

### `<graph.h>`

  * `_setvideomode (`_mode_`)`
    - only implemented
      - for constant _mode_'s
       - for MDPA and CGA modes

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
  * `_int86f (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);`
    - extension --- loads flags
  * `_int86xf (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
    - extension --- loads flags
  * `_intrf (`_inter-no_`,` \*_regs_`);`
    - extension --- loads flags
  * `FP_OFF (`\*_ptr_`);`
    - macro; also `_FP_OFF`
  * `FP_SEG (`\*_ptr_`);`
    - macro; also `_FP_SEG`
  * `MK_FP (`_seg_`,` _off_`);`
    - macro; also `_MK_FP`

### `<libi86/string.h>`

  * \*`_fmemcpy (`\*_dest_`,` \*_src_`,` _n_`);`
  * \*`_fmemmove (`\*_dest_`,` \*_src_`,` _n_`);`

## Implemented variables

### `<libi86/stdlib.h>`

  * `_osmajor` `_osminor` `_psp`
