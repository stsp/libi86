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
 * - ENTER4_BX_ (N) does the same as ENTER_BX_ (N), except that for the
 *   `regparmcall' convention, it assuems that the function has N + 4 bytes
 *   of arguments.
 *
 * - ARG0W_BX_, ARG2W_BX_, ARG4W_BX_, or ARG6W_BX_ refers respectively to the
 *   first, second, third, or fourth argument shortword.  Each of these
 *   macros should be used only after ENTER_BX_.
 *
 * - MOV_ARG0W_BX_ (REG), MOV_ARG2W_BX_ (REG), MOV_ARG4W_BX_ (REG), or
 *   MOV_ARG6W_BX_ (REG) moves the first, second, third, or fourth argument
 *   shortword into REG, respectively, if necessary.  Each of these macros
 *   should be used only after ENTER_BX_.
 *
 * - MOV_ARG0B_BX_ (REG), MOV_ARG2B_BX_ (REG), MOV_ARG4B_BX_ (REG), or
 *   MOV_ARG6B_BX_ (REG) moves the low byte of the first, second, third, or
 *   fourth argument shortword into REG, respectively, if necessary.  Each
 *   of these macros should be used only after ENTER_BX_.
 *
 *   When using these macros, be careful not to clobber a `regparmcall'
 *   argument register before it is read, and be careful not to clobber %bx
 *   itself for the `cdecl' and `stdcall' conventions.
 *
 * - RET_(N) emits an instruction to return from a function with N bytes of
 *   arguments.
 *
 * - RET4_(N) does the same as RET_(N), except that for the `regparmcall'
 *   convention, it assumes that the function has N + 4 bytes of arguments.
 */

#if defined __IA16_CALLCVT_STDCALL
# define ENTER_BX_(n)		movw %sp, %bx
# define ENTER4_BX_(n)		movw %sp, %bx
# define ARG0W_BX_		2(%bx)
# define ARG2W_BX_		4(%bx)
# define ARG4W_BX_		6(%bx)
# define ARG6W_BX_		8(%bx)
# define MOV_ARG0W_BX_(reg)	movw 2(%bx), reg
# define MOV_ARG2W_BX_(reg)	movw 4(%bx), reg
# define MOV_ARG4W_BX_(reg)	movw 6(%bx), reg
# define MOV_ARG6W_BX_(reg)	movw 8(%bx), reg
# define MOV_ARG0B_BX_(reg)	movb 2(%bx), reg
# define MOV_ARG2B_BX_(reg)	movb 4(%bx), reg
# define MOV_ARG4B_BX_(reg)	movb 6(%bx), reg
# define MOV_ARG6B_BX_(reg)	movb 8(%bx), reg
# define RET_(n)		ret $(n)
# define RET4_(n)		ret $(n)
#elif defined __IA16_CALLCVT_REGPARMCALL
# if __IA16_FEATURE_ATTRIBUTE_REGPARMCALL != 20180813L
#   warning "regparmcall convention is not 20180813L, output code may be bogus"
# endif
# define ENTER_BX_(n)		.if (n)>6; \
				movw %sp, %bx; \
				.endif
# define ENTER4_BX_(n)		.if (n)+4>6; \
				movw %sp, %bx; \
				.endif
# define ARG0W_BX_		%ax
# define ARG2W_BX_		%dx
# define ARG4W_BX_		%cx
# define ARG6W_BX_		2(%bx)
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
# define RET_(n)		.if (n)>6; \
				ret $((n)-6); \
				.else; \
				ret; \
				.endif
# define RET4_(n)		.if (n)+4>6; \
				ret $((n)+4-6); \
				.else; \
				ret; \
				.endif
#else
# ifndef __IA16_CALLCVT_CDECL
#   warning "not sure which calling convention is in use; assuming cdecl"
# endif
# define ENTER_BX_(n)		movw %sp, %bx
# define ENTER4_BX_(n)		movw %sp, %bx
# define ARG0W_BX_		2(%bx)
# define ARG2W_BX_		4(%bx)
# define ARG4W_BX_		6(%bx)
# define ARG6W_BX_		8(%bx)
# define MOV_ARG0W_BX_(reg)	movw 2(%bx), reg
# define MOV_ARG2W_BX_(reg)	movw 4(%bx), reg
# define MOV_ARG4W_BX_(reg)	movw 6(%bx), reg
# define MOV_ARG6W_BX_(reg)	movw 8(%bx), reg
# define MOV_ARG0B_BX_(reg)	movb 2(%bx), reg
# define MOV_ARG2B_BX_(reg)	movb 4(%bx), reg
# define MOV_ARG4B_BX_(reg)	movb 6(%bx), reg
# define MOV_ARG6B_BX_(reg)	movb 8(%bx), reg
# define RET_(n)		ret
# define RET4_(n)		ret
#endif
