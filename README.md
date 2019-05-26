# libi86

[![(build status)](https://travis-ci.org/tkchia/libi86.svg?branch=master)](https://travis-ci.org/tkchia/libi86)

An attempt to reimplement non-standard C library facilities (e.g. `<conio.h>`) commonly used in classical MS-DOS programs, for use with a [GCC toolchain for 16-bit x86](https://github.com/tkchia/build-ia16/).

The current aim is to be compatible enough with the [Open Watcom](https://github.com/open-watcom/open-watcom-v2/) runtime to be useful for building existing MS-DOS code.

## Implemented functions

### `<bios.h>`

  * `_bios_equiplist ();`
  * `_bios_memsize ();`
  * `_bios_keybrd (`_service_`);`
  * `_bios_timeofday (`_service_`,` \*_timeval_`);`

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
  * `inp (`_port_`);` // also `_inp`
  * `inpw (`_port_`);` // also `_inpw`
  * `outp (`_port_`,` _value_`);` // also `_outp`
  * `outpw (`_port_`,` _value_`);` // also `_outpw`

### `<dos.h>`

  * `bdos (`_dos-func_`,` _dx_`,` _al_`);`
  * `bdosptr (`_dos-func_`,` \*_dx_`,` _al_`);`
  * `intdos (`\*_in-regs_`,` \*_out-regs_`);`
  * `intdosx (`\*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
  * `<dos.h>` also includes `<i86.h>`

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
  * `_int86 (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);` // extension --- loads flags
  * `_int86xf (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);` // extension --- loads flags
  * `_intrf (`_inter-no_`,` \*_regs_`);` // extension --- loads flags
  * `FP_OFF (`_ptr_`);` // macro; also `_FP_OFF`
  * `FP_SEG (`_ptr_`);` // macro; also `_FP_SEG`
  * `MK_FP (`_seg_`,` _off_`);` // macro; also `_MK_FP`

### `<libi86/string.h>`

  * \*`_fmemcpy (`\*_dest_`,` \*_src_`,` _n_`);`
  * \*`_fmemmove (`\*_dest_`,` \*_src_`,` _n_`);`
