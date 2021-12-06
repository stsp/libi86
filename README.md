# libi86

[![(GitLab pipeline status)](https://gitlab.com/tkchia/libi86/badges/master/pipeline.svg)](https://gitlab.com/tkchia/libi86/-/commits/master)

This is a code library which attempts to reimplement non-standard C library facilities (e.g. `<conio.h>`) commonly used in classical MS-DOS programs.  The library also implements a few standard facilities which are part of newer C language standards, where the underlying C library lacks them.

For now, the library is meant to be used with the following compilers and targets:
  * either [GCC toolchain for 16-bit x86](https://github.com/tkchia/build-ia16/);
  * or the [Amsterdam Compiler Toolkit](https://github.com/davidgiven/ack) with the `msdos86` target platform.

It may be ported to other compilers and targets in the future.

The current aim is to be compatible enough with the [Open Watcom](https://github.com/open-watcom/open-watcom-v2/) runtime — as described in the [_Open Watcom C Library Reference_](https://github.com/open-watcom/open-watcom-v2-wikidocs/blob/master/docs/clib.pdf) — to be useful for building existing MS-DOS code.

Defining the macro `_BORLANDC_SOURCE` will also enable some degree of compatibility with the [Borland Turbo C++](http://cc.embarcadero.com/Item/25636) compiler's C library, which is described in the [_Borland C++ 2.0 Library Reference_](https://archive.org/details/bitsavers_borlandborn2.0LibraryReference1991_17218611).

## Synopsis

### Use

When using IA-16 GCC:
  * `ia16-elf-gcc` [..._gcc-options_...] `-li86` [...]
  * On a MS-DOS host: `i16gcc` [..._gcc-options_...] `-li86` [...]

When using ACK:
  * `ack-cc -mmsdos86` [..._ack-options_...] `-li86.a` [...]
  * Or (if compiler is installed as `ack`): `ack -mmsdos86` [..._ack-options_...] `-li86.a` [...]

### Installing from pre-compiled Ubuntu Linux packages, for cross development

When using IA-16 GCC:
  * Grab the `gcc-ia16-elf` and `libi86-ia16-elf` packages from [my `build-ia16` PPA](https://launchpad.net/~tkchia/+archive/ubuntu/build-ia16/).

When using ACK, you currently need to build `libi86` from sources.

### Building from sources, installing, and testing, on Linux

[Detailed instructions](README.linux-build.md) for doing so are available.

## License

`libi86` as a whole is now distributed under the [3-clause BSD License](LICENSE).

(A few files are distributed under other licenses, but in ways that are ultimately compatible with the BSD License.)

## Implemented facilities

### Legend

| Compat. | Meaning
| ------: | :------
| C99 | Part of the [C99](https://www.iso.org/standard/29237.html) standard (final draft: [N1256](http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf)) — enable with a macro `_ISOC99_SOURCE` if necessary.
|   W | Behaves like the corresponding function in Open Watcom.
|  W+ | Behaves like the corresponding function in Open Watcom, but with some extended behaviours.
|   B | From Borland Turbo C++ — enable with `_BORLANDC_SOURCE`.
| W/B | By default, behaves as in Open Watcom; if `_BORLANDC_SOURCE` is defined, behaves as in Borland C++.
|  IW | From internal interfaces in Open Watcom's library code.
|   X | `libi86`-specific extension; not in Open Watcom or Borland C++.  Enabled even if the macros `_BORLANDC_SOURCE` and `_ISOC99_SOURCE` are not defined.
|  XB | `libi86`-specific extension.  Behaves slightly differently if `_BORLANDC_SOURCE` is defined.

| Avail. | Meaning
| -----: | :------
|   = | Available for all compilers and platforms that `libi86` targets.
| (=) | Available for all compilers and platforms that do not already define this facility themselves.
|   G | Available (only) under IA-16 GCC.

### Functions

| Compat.    | Avail. | Function | Notes
| ---------: | -----: | :------- | :----
|     |     | **`▗▚▚▚▚ <bios.h> ▞▞▞▞▖`**
|  W+ |   = | `_bios_disk (`_service_`,` \*_diskinfo_`);` | As an extension, also accepts _service_ = `_DISK_DRIVEPARAMS`, which returns drive parameters in \*_diskinfo_.
|   W |   = | `_bios_equiplist ();`
|   B |   = | `biosequip ();`
|   W |   = | `_bios_memsize ();`
|   B |   = | `biosmemory ();`
|   W |   = | `_bios_keybrd (`_service_`);`
|   B |   = | `bioskey (`_service_`);`
|   W |   = | `_bios_printer (`_service_`,` _port_`,` _data_`);`
|   W |   = | `_bios_serialcom (`_service_`,` _port_`,` _data_`);`
|   W |   = | `_bios_timeofday (`_service_`,` \*_timeval_`);`
|   X |   = | `_bios_joystick (`_service_`,` \*_joyinfo_`);` | Reads joystick status via `int 0x15` function `0x84`.
|     |     |
|     |     | **`▗▚▚▚▚ <conio.h> ▞▞▞▞▖`** | **If `_BORLANDC_SOURCE` is defined, `<conio.h>` switches to an alternate implementation of the console output routines which is based on `<graph.h>` facilities.**
| W/B |   = | \*`cgets (`\*_buf_`);`
| W/B |   = | `cprintf (`\*_fmt_`, ...);`
| W/B |   = | `cputs (`\*_buf_`);`
| W/B |   = | `cscanf (`\*_fmt_`, ...);`
|   W |   = | `getch ();`
|   W |   = | `_getch ();`
| W/B |   = | `getche ();`
|   W |   = | `_getche ();`
|   W |   = | `kbhit ();`
|   W |   = | `_kbhit ();`
|   W |   = | `ungetch (`_ch_`);`
|   W |   = | `_ungetch (`_ch_`);`
| W/B |   = | `putch (`_ch_`);`
| W/B |   = | `vcprintf (`\*_fmt_`,` _ap_`);`
| W/B |   = | `vcscanf (`\*_fmt_`,` _ap_`);`
|     |     |
|   B |   = | `clreol ();`
|   B |   = | `clrscr ();`
|   B |   = | `delline ();`
|   B |   = | \*`getpass (`\*_prompt_`);`
|   B |   = | `gettextinfo (`\*_text-info_`);` | If the active video mode is a SuperVGA mode, _text-info_`->currmode` may be invalid.
|   B |   = | `gotoxy (`_x_`,` _y_`);`
|   B |   = | `highvideo ();`
|   B |   = | `insline ();`
|   B |   = | `lowvideo ();`
|   B |   = | `normvideo ();`
|   B |   = | `textattr (`_new-attr_`);`
|   B |   = | `textbackground (`_new-color_`);`
|   B |   = | `textcolor (`_new-color_`);`
|   B |   = | `textmode (`_mode_`);` | Does not support _mode_ = `LASTMODE` yet.
|   B |   = | `wherex ();`
|   B |   = | `wherey ();`
|   B |   = | `window (`_left_`,` _top_`,` _right_`,` _bottom_`);`
|     |     |
|   W |   = | `inp (`_port_`);`
|   W |   = | `_inp (`_port_`);`
|   B |   = | `inportb (`_port_`);`
|   W |   = | `inpw (`_port_`);`
|   W |   = | `_inpw (`_port_`);`
|   B |   = | `inport (`_port_`);` | Returns a signed value.
|   B |   = | `inportw (`_port_`);` | Returns an unsigned value.
|   W |   = | `outp (`_port_`,` _value_`);`
|   W |   = | `_outp (`_port_`,` _value_`);`
|   B |   = | `outportb (`_port_`,` _value_`);`
|   W |   = | `outpw (`_port_`,` _value_`);`
|   W |   = | `_outpw (`_port_`,` _value_`);`
|   B |   = | `outport (`_port_`,` _value_`);` | Accepts a signed value to write.
|   B |   = | `outportw (`_port_`,` _value_`);` | Accepts an unsigned value to write.
|     |     |
|     |     | **`▗▚▚▚▚ <dir.h> ▞▞▞▞▖`**
|   B |   G | `searchpath (`_file_`);` | Currently only available for `gcc-ia16`.
|   X |   G | `_searchpath (`_file_`);` | Currently only available for `gcc-ia16`.
|     |     |
|     |     | **`▗▚▚▚▚ <dos.h> ▞▞▞▞▖`** | **`<dos.h>` also includes `<i86.h>`, described below.  If `_BORLANDC_SOURCE` is defined, the `union REGS` type gets an additional `.x.flags` field, and `<dos.h>` switches accordingly to a different version of the `intdos` and `intdosx` routines.**
|   W |   = | `bdos (`_dos-func_`,` _dx_`,` _al_`);`
|   B |   = | `bdosptr (`_dos-func_`,` \*_dx_`,` _al_`);`
| W/B |   = | `intdos (`\*_in-regs_`,` \*_out-regs_`);`
| W/B |   = | `intdosx (`\*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
|  W+ |   = | `_dos_allocmem (`_size_`,` \*_segment_`);` | Also works under DPMI; yields a starting protected-mode selector.
|   W |   = | `_dos_close (`_handle_`);`
|   W |   = | `_dos_commit (`_handle_`);`
|   W |   = | `_dos_creat (`\*_path_`,` _attr_`,` \*_handle_`);`
|   W |   = | `_dos_creatnew (`\*_path_`,` _attr_`,` \*_handle_`);`
|   W |   = | `_dos_findfirst (`\*_path_`,` _attributes_`,` \*_buffer_`);`
|   W |   = | `_dos_findnext (`\*_buffer_`);`
|   W |   = | `_dos_findclose (`\*_buffer_`);`
|  W+ |   = | `_dos_freemem (`_segment_`);` | Also works under DPMI; accepts a starting protected-mode selector.
|   W |   = | `_dos_getdate (`\*_date_`);`
|   W |   = | `_dos_getdiskfree (`_drive_`,` \*_disk-space_`);`
|   W |   = | `_dos_getdrive (`\*_drive_`);`
|   W |   = | `_dos_getfileattr (`\*_path_`,` \*_attributes_`);`
|   W |   = | `_dos_getftime (`_handle_`,` \*_date_`,` \*_time_`);`
|   W |   = | `_dos_gettime (`\*_time_`);`
|   W |   = | \*`_dos_getvect (`_intr-no_`);` | Some versions of `gcc-ia16` and ACK may not understand the `interrupt` function attribute.  In that case, this function will return a far data pointer.
|   W |   = | `_dos_keep (`_status_`,` _keep-paras_`);`
|   B |   = | `keep (`_status_`,` _keep-paras_`);`
|   W |   = | `_dos_open (`\*_path_`,` _mode_`,` \*_handle_`);`
|   W |   = | `_dos_read (`_handle_`,` \*_buf_`,` _count_`,` \*_bytes_`);`
|   W |   = | `_dos_setblock (`_size_`,` _seg_`,` \*_max-size_`);`
|   W |   = | `_dos_setdrive (`_drive_`,` \*_total_`);`
|   W |   = | `_dos_setfileattr (`\*_path_`,` _attributes_`);`
|   W |   = | `_dos_setvect (`_intr-no_`,` \*_handler_`);` | Some versions of `gcc-ia16` and ACK may not understand the `interrupt` function attribute.  In that case, this function will not be supported.
|   W |   = | `_dos_write (`_handle_`,` \*_buf_`,` _count_`,` \*_bytes_`);`
|   W |   = | `_getdrive ();`
|     |     |
|   B |   = | `peek (`_segment_`,` _offset_`);`
|   B |   = | `peekb (`_segment_`,` _offset_`);`
|   B |   = | `poke (`_segment_`,` _offset_`,` _word-value_`);`
|   B |   = | `pokeb (`_segment_`,` _offset_`,` _byte-value_`);`
|   B |   = | `inportb (`_port_`);`
|   B |   = | `inport (`_port_`);` | Returns a signed value.
|   B |   = | `inportw (`_port_`);` | Returns an unsigned value.
|   B |   = | `outportb (`_port_`,` _value_`);`
|   B |   = | `outport (`_port_`,` _value_`);` | Accepts a signed value to write.
|   B |   = | `outportw (`_port_`,` _value_`);` | Accepts an unsigned value to write.
|     |     |
|     |     | **`▗▚▚▚▚ <dpmi.h> ▞▞▞▞▖`** | **Except for `__DPMI_hosted ()`, functions in `<dpmi.h>` should only be called when the caller knows it is running in DPMI mode.  `<dpmi.h>` is not supported for ACK.**
|  IW |   G | `__DPMI_hosted ();` | Returns 1 if running in protected mode under DPMI, -1 otherwise.  If the underlying C library has an implementation of this function, `libi86` will use that instead.
|  IW |   G | `_DPMIAllocateDOSMemoryBlock (`_paras_`);` | `int 0x31` function `0x0100`.  Returns a structure giving the real mode segment and protected mode selector for the DOS memory block.  On failure, returns `{ 0, 0 }`.
|  IW |   G | `_DPMIAllocateLDTDescriptors (`_count_`);` | `int 0x31` function `0x0000`.  Returns a starting protected-mode selector, case to an `int32_t`.  On failure, returns a negative value.
|  IW |   G | `_DPMIFreeDOSMemoryBlock (`_sel_`);` | `int 0x31` function `0x0101`.  Returns 0 on success, -1 on error.
|  IW |   G | `_DPMIFreeLDTDescriptor (`_sel_`);` | `int 0x31` function `0x0001`.  Returns 0 on success, -1 on error.
|  IW |   G | `_DPMIGetDescriptor (`_sel_`,` \*_desc_`);` | `int 0x31` function `0x000b`.  Returns 0 on success, -1 on error.
|  IW |   G | `_DPMIGetSegmentBaseAddress (`_sel_`);` | `int 0x31` function `0x0006`.  Returns _sel_'s base address on success; return value is undefined on error.
|  IW |   G | `_DPMISegmentToDescriptor (`_seg-para_`);` | `int 0x31` function `0x0002`.  On success, returns a protected-mode selector value for the real-mode segment _seg-para_`:0`.  On failure, returns a negative value.
|  IW |   G | `_DPMISetDescriptor (`_sel_`,` \*_desc_`);` | `int 0x31` function `0x000c`.  Returns 0 on success, -1 on error.
|  IW |   G | `_DPMISimulateRealModeInterrupt (`_inter-no_`,` _reset_`,` _words-to-copy_`,` \*_call-struct_`);` | `int 0x31` function `0x0300`.  Returns 0 on success, -1 on error.  _words-to-copy_ should probably be 0.
|     |     |
|     |     | **`▗▚▚▚▚ <graph.h> ▞▞▞▞▖`** | **Unlike in Open Watcom, where all functions in `<graph.h>` are far, in `libi86` the far-ness of functions follows the chosen memory model.  Thus, in a small-memory-model program, `_setvideomode` is a near function.  However, pointers to data are still far.**
|   W |   = | `_clearscreen (`_area_`);`
|   W |   = | `_gettextposition ();`
|   X |   = | `_getvideomode ();`
|   W |   = | `_outmem (`\*_text_`,` _length_`);`
|   W |   = | `_outtext (`\*_text_`);`
|   W |   = | `_scrolltextwindow (`_rows_`);`
|   W |   = | `_settextcolor (`_pix-val_`);`
|   W |   = | `_settextposition (`_row_`,` _col_`);`
|   W |   = | `_settextwindow (`_row1_`,` _col1_`,` _row2_`,` _col2_`);`
|   W |   = | `_setvideomode (`_mode_`);` | In the case of SuperVGA screen modes, only works with VESA interface.
|     |     |
|     |     | **`▗▚▚▚▚ <i86.h> ▞▞▞▞▖`** | **If `_BORLANDC_SOURCE` is defined, the `union REGS` type gets an additional `.x.flags` field, and `<i86.h>` switches accordingly to a different version of the `int86`, `int86x`, `_int86f`, and `_int86xf` routines.**
|   W |   = | `delay (`_ms_`);`
|   W |   = | `nosound ();`
|   W |   = | `sound (`_freq_`);`
|   W |   = | `segread (`\*_seg-regs_`);`
|   W |   = | `_disable ();`
|   W |   = | `_enable ();`
| W/B |   = | `int86 (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);`
| W/B |   = | `int86x (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);`
|   W |   = | `intr (`_inter-no_`,` \*_regs_`);` | Clears `SZAPC` flags to 0 before issuing interrupt.  (This follows a documentation change in Open Watcom versions after Oct 2018.)
|  XB |   = | `_int86f (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`);` | Loads carry flag before issuing interrupt.
|  XB |   = | `_int86xf (`_inter-no_`,` \*_in-regs_`,` \*_out-regs_`,` \*_seg-regs_`);` | Loads carry flag before issuing interrupt.
|   W |   = | `intrf (`_inter-no_`,` \*_regs_`);` | Loads `SZAPC` flags before issuing interrupt.
|   X |   = | `_intrf (`_inter-no_`,` \*_regs_`);` | Loads `SZAPC` flags before issuing interrupt.
|   W |   = | `FP_OFF (`\*_ptr_`);` | Macro.
|   W |   = | `_FP_OFF (`\*_ptr_`);` | Macro.
|   W |   = | `FP_SEG (`\*_ptr_`);` | Macro.
|   W |   = | `_FP_SEG (`\*_ptr_`);` | Macro.
|   W |   = | \*`MK_FP (`_seg_`,` _off_`);` | Macro.
|   W |   = | \*`_MK_FP (`_seg_`,` _off_`);` | Macro.
|   X |   = | \*`_CV_FP (`\*_ptr_`);` | Convert a default-sized pointer to a far pointer.  This is mainly useful for ACK, which lacks built-in far pointer support.
|   X |   = | \*`_FP_EQ (`\*_ptr1_`,` _ptr2_`);` | Test whether two far pointers are exactly equal.  This is mainly useful for ACK, which lacks built-in far pointer support.
|     |     |
|     |     | **`▗▚▚▚▚ <libi86/stdio.h> ▞▞▞▞▖`** | **`<libi86/stdio.h>` also includes the underlying C library's `<stdio.h>`.**
| C99 | (=) | `vsscanf (`\*_s_`,` \*_fmt_`,` _ap_`);`
|   X | (=) | `_vsscanf (`\*_s_`,` \*_fmt_`,` _ap_`);`
|     |     |
|     |     | **`▗▚▚▚▚ <libi86/stdlib.h> ▞▞▞▞▖`** | **`<libi86/stdlib.h>` also includes the underlying C library's `<stdlib.h>`.**
|   W |   G | \*`lltoa (`_value_`,` \*_buffer_`,` _radix_`);` | Not yet supported on ACK &mdash; it lacks `long long` support for IA-16.
|   W |   G | \*`_lltoa (`_value_`,` \*_buffer_`,` _radix_`);` | Not yet supported on ACK &mdash; it lacks `long long` support for IA-16.
|   W |   = | \*`ltoa (`_value_`,` \*_buffer_`,` _radix_`);`
|   W |   = | \*`_ltoa (`_value_`,` \*_buffer_`,` _radix_`);`
|  W+ |   = | `_makepath (`\*_path_`,` \*_drive_`,` \*_dir_`,` \*_fname_`,` \*_ext_`);` | As extensions, this function (1) checks for buffer overflow, and (2) gives a return value.  Upon an error, the return value is non-zero, `errno` is set, and _path_`[]` holds either an empty string or a truncated path.  Network _drive_`[]` values starting with two backslashes (`\\`) are not supported.
|   W |   = | `_splitpath (`\*_path_`,` \*_drive_`,` \*_dir_`,` \*_fname_`,` \*_ext_`);` | Long filenames, and network paths starting with two backslashes (`\\`), are not supported.
|   W |   G | \*`ulltoa (`_value_`,` \*_buffer_`,` _radix_`);` | Not yet supported on ACK &mdash; it lacks `long long` support for IA-16.
|   W |   G | \*`_ulltoa (`_value_`,` \*_buffer_`,` _radix_`);` | Not yet supported on ACK &mdash; it lacks `long long` support for IA-16.
|   W |   = | \*`ultoa (`_value_`,` \*_buffer_`,` _radix_`);`
|   W |   = | \*`_ultoa (`_value_`,` \*_buffer_`,` _radix_`);`
|     |     |
|     |     | **`▗▚▚▚▚ <libi86/string.h> ▞▞▞▞▖`** | **`<libi86/string.h>` also includes the underlying C library's `<string.h>`.**
|   W |   = | \*`_fmemcpy (`\*_dest_`,` \*_src_`,` _n_`);`
|   W |   = | \*`_fmemmove (`\*_dest_`,` \*_src_`,` _n_`);`
|   W |   = | `_fstrlen (`\*_s_`);`

### Variables

| Compat.    | Avail. | Variable | Notes
| ---------: | -----: | :------- | :----
|     |     | **`▗▚▚▚▚ <libi86/stdlib.h> ▞▞▞▞▖`**
|   W |   = | `_osmajor` | Implemented as a function call on ACK.
|   W |   = | `_osminor` | Implemented as a function call on ACK.
|   W |   = | `_psp` | Implemented as a function call on ACK.
