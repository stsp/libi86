/*
 * Macros to handle different function calling conventions.
 *
 * Copyright (c) 2018 TK Chia
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */

/*
 * These macros partially abstract away the differences between the
 * `stdcall', `cdecl', and `regparmcall' calling conventions implemented in
 * ia16-elf-gcc.
 *
 * - ENTER_BX_ (N) loads %sp into %bx to access stack arguments, if necessary.
 *   N is the number of bytes of arguments passed to the function.
 *
 *   ENTER_BX_ (.) should occur before any other pushes to save registers
 *   (e.g. `pushw %si').
 *
 * - ENTER2_BX_ (N) does the same as ENTER_BX_ (N), except that for the
 *   `regparmcall' convention, it assumes that the function uses only two
 *   registers (%ax and %dx) to hold parameters.
 *
 * - ARG0W_BX_, ARG2W_BX_, ARG4W_BX_, ARG6W_BX_, or ARG8W_BX_ refers
 *   respectively to the first, second, third, fourth, or fifth argument
 *   shortword.  Each of these macros should be used only after ENTER_BX_.
 *
 * - MOV_ARG0W_BX_ (REG), MOV_ARG2W_BX_ (REG), MOV_ARG4W_BX_ (REG),
 *   MOV_ARG6W_BX_ (REG), or MOV_ARG8W_BX_ (REG) moves the first, second,
 *   third, fourth, or fifth argument shortword into REG, respectively, if
 *   necessary.  Each of these macros should be used only after ENTER_BX_.
 *
 * - MOV_ARG8W2_BX_(REG) does the same as MOV_ARG8W_BX_ (REG), except that
 *   for the `regparmcall' convention, it assumes that the function uses
 *   only two registers (%ax and %dx) to hold parameters.
 *
 * - MOV_ARG0B_BX_ (REG), MOV_ARG2B_BX_ (REG), MOV_ARG4B_BX_ (REG), or
 *   MOV_ARG6B_BX_ (REG), or MOV_ARG8B_BX_ (REG) moves the low byte of the
 *   first, second, third, fourth, or fifth argument shortword into REG,
 *   respectively, if necessary.  Each of these macros should be used only
 *   after ENTER_BX_.
 *
 * - LDS_ARG0W_BX_ (REG) moves the first shortword into REG, and the second
 *   into %ds; LES_ARG0W_BX_ (REG) moves the shortwords into REG and %es.
 *   Each of these macros should be used only after ENTER_BX_.
 *
 * - LDS_ARG4W2_BX_ (REG) moves the third shortword into REG, and the fourth
 *   into %ds; LES_ARG4W2_BX_ (REG) moves the shortwords into REG and %es. 
 *   For the `regparmcall' convention, these macros assume that the function
 *   uses only two registers (%ax and %dx) to hold parameters.  Each of
 *   these macros should be used only after ENTER_BX_.
 *
 * - RET_(N) emits an instruction to return from a function with N bytes of
 *   arguments.
 *
 * - RET2_(N) does the same as RET_(N), except that for the `regparmcall'
 *   convention, it assumes that the function uses only two registers (%ax and
 *   %dx) to hold parameters.
 *
 * When using the MOV_*, LDS_*, or LES_* macros, be careful not to clobber a
 * `regparmcall' argument register before it is read, and be careful not to
 * clobber %bx itself for the `cdecl' and `stdcall' conventions.
 */

#if defined __IA16_CALLCVT_REGPARMCALL
# if __IA16_REGPARMCALL_ABI - 0 != 20180814L
#   warning "regparmcall convention is not 20180814L, output code may be bogus"
# endif
# define ENTER_BX_(n)		.if (n)>6; \
				movw %sp, %bx; \
				.endif
# define ENTER2_BX_(n)		.if (n)>4; \
				movw %sp, %bx; \
				.endif
# define ARG0W_BX_		%ax
# define ARG2W_BX_		%dx
# define ARG4W_BX_		%cx
# define ARG6W_BX_		2(%bx)
# define ARG8W_BX_		4(%bx)
# define MOV_ARG0W_BX_(reg)	.ifnc %ax, reg; \
				movw %ax, reg; \
				.endif
# define MOV_ARG2W_BX_(reg)	.ifnc %dx, reg; \
				movw %dx, reg; \
				.endif
# define MOV_ARG4W_BX_(reg)	.ifnc %cx, reg; \
				movw %cx, reg; \
				.endif
# define MOV_ARG6W_BX_(reg)	movw 2(%bx), reg
# define MOV_ARG8W_BX_(reg)	movw 4(%bx), reg
# define MOV_ARG8W2_BX_(reg)	movw 6(%bx), reg
# define MOV_ARG0B_BX_(reg)	.ifnc %al, reg; \
				movb %al, reg; \
				.endif
# define MOV_ARG2B_BX_(reg)	.ifnc %dl, reg; \
				movb %dl, reg; \
				.endif
# define MOV_ARG4B_BX_(reg)	.ifnc %cl, reg; \
				movb %cl, reg; \
				.endif
# define MOV_ARG6B_BX_(reg)	movb 2(%bx), reg
# define MOV_ARG8B_BX_(reg)	movb 4(%bx), reg
# define LDS_ARG0W_BX_(reg)	movw %dx, %ds; \
				.ifnc %ax, reg; \
				movw %ax, reg; \
				.endif
# define LES_ARG0W_BX_(reg)	movw %dx, %es; \
				.ifnc %ax, reg; \
				movw %ax, reg; \
				.endif
# define LDS_ARG4W2_BX_(reg)	ldsw 2(%bx), reg
# define LES_ARG4W2_BX_(reg)	lesw 2(%bx), reg
# define RET_(n)		.if (n)>6; \
				ret $((n)-6); \
				.else; \
				ret; \
				.endif
# define RET2_(n)		.if (n)>4; \
				ret $((n)-4); \
				.else; \
				ret; \
				.endif
#else
# define ENTER_BX_(n)		movw %sp, %bx
# define ENTER2_BX_(n)		movw %sp, %bx
# define ARG0W_BX_		2(%bx)
# define ARG2W_BX_		4(%bx)
# define ARG4W_BX_		6(%bx)
# define ARG6W_BX_		8(%bx)
# define ARG8W_BX_		10(%bx)
# define MOV_ARG0W_BX_(reg)	movw 2(%bx), reg
# define MOV_ARG2W_BX_(reg)	movw 4(%bx), reg
# define MOV_ARG4W_BX_(reg)	movw 6(%bx), reg
# define MOV_ARG6W_BX_(reg)	movw 8(%bx), reg
# define MOV_ARG8W_BX_(reg)	movw 10(%bx), reg
# define MOV_ARG8W2_BX_(reg)	movw 10(%bx), reg
# define MOV_ARG0B_BX_(reg)	movb 2(%bx), reg
# define MOV_ARG2B_BX_(reg)	movb 4(%bx), reg
# define MOV_ARG4B_BX_(reg)	movb 6(%bx), reg
# define MOV_ARG6B_BX_(reg)	movb 8(%bx), reg
# define MOV_ARG8B_BX_(reg)	movb 10(%bx), reg
# define LDS_ARG0W_BX_(reg)	ldsw 2(%bx), reg
# define LES_ARG0W_BX_(reg)	lesw 2(%bx), reg
# define LDS_ARG4W2_BX_(reg)	ldsw 6(%bx), reg
# define LES_ARG4W2_BX_(reg)	lesw 6(%bx), reg
# ifdef __IA16_CALLCVT_STDCALL
#   define RET_(n)		ret $(n)
#   define RET2_(n)		ret $(n)
# else
#   ifndef __IA16_CALLCVT_CDECL
#     warning "not sure which calling convention is in use; assuming cdecl"
#   endif
#   define RET_(n)		ret
#   define RET2_(n)		ret
# endif
#endif
