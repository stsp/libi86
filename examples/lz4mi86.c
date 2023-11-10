/*
 * Copyright (c) 2023 TK Chia
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the developer(s) nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Convert an LZ4-compressed frame into a C module containing the contents
 * of the corresponding LZ4 block.  Use only on trusted input.
 */

#define _BORLANDC_SOURCE
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <i86.h>
#include <io.h>
#include <limits.h>
#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <libi86/stdlib.h>
#include <libi86/string.h>
#include <nexgen/kompress.h>
#include <nexgen/mzendian.h>

#ifdef __ACK
extern int getopt (int, char * const[], const char *);
extern char *optarg;
extern int optind;
#endif
#ifndef STDIN_FILENO
# define STDIN_FILENO	0
#endif
#ifndef STDOUT_FILENO
# define STDOUT_FILENO	1
#endif
#ifndef STDERR_FILENO
# define STDERR_FILENO	2
#endif

struct lz4_frame_hdr
{
  uint_le32_t magic;
  uint8_t flg;
  uint8_t bd;
  uint_le64_t xlen;
  uint8_t hc;
};

#define FH_MAG		0x184d2204
#define FLG_VER		(3 << 6)
#define FLG_VER_CURR	(1 << 6)
#define FLG_BCKSUM	(1 << 4)
#define FLG_XLEN	(1 << 3)
#define FLG_DICTID	(1 << 0)
#define BD_MASK		0xcf
#define BD_EXPECT	0x40
#define BSZ_UNCOMPR	((uint32_t) 1 << 31)
#ifdef __MSDOS__
# define NL		"\r\n"
#else
# define NL		"\n"
#endif

static char me[_MAX_FNAME];
static const char *in_path = NULL, *out_path = NULL, *symbol = "data_file";
static int in_fd = STDIN_FILENO, out_fd = STDOUT_FILENO;
static bool is_far = false;

static void fatal (const char *);

static void
ouch (void)
{
  if (out_path && out_fd != -1)
    {
      close (out_fd);
      unlink (out_path);
    }
  exit (2);
}

static void
output_fd (int fd, const void *buf, size_t n)
{
  const char *p = (const char *) buf;
  while (n)
    {
      ssize_t r;
      errno = 0;
      r = write (fd, p, n);
      if (r == 0 || r == -1)
	{
	  if (fd == STDERR_FILENO)
	    ouch ();
	  fatal ("cannot write output file");
	}
      p += r;
      n -= r;
    }
}

static void
errputs (const char *s)
{
  output_fd (STDERR_FILENO, s, strlen (s));
}

static void
outputs (const char *s)
{
  output_fd (out_fd, s, strlen (s));
}

static void
outputc (char c)
{
  output_fd (out_fd, &c, 1);
}

static void
outputu (size_t u)
{
  char buf[(sizeof (size_t) * CHAR_BIT + 2) / 3 + 1], *p, *q;
  p = q = buf + sizeof (buf) - 1;
  *p = 0;
  do
    {
      *--p = u % 10 + '0';
      u /= 10;
    }
  while (u != 0);
  output_fd (out_fd, p, q - p);
}

static void
usage (void)
{
  errputs ("usage: ");
  errputs (me);
  errputs (" [-F] [-o OUT-FILE] [-s SYMBOL] [IN-FILE]" NL);
  exit (1);
}

static void
fatal (const char *msg)
{
  int err = errno;
  errputs (me);
  errputs (": ");
  errputs (msg);
  errputs (": ");
  if (err)
    errputs (strerror (err));
  else
    errputs ("EOF or disk full");
  errputs (NL);
  ouch ();
}

static void
input (void *buf, size_t n)
{
  char *p = (char *) buf;
  while (n)
    {
      ssize_t r;
      errno = 0;
      r = read (in_fd, p, n);
      if (r == 0 || r == -1)
	fatal ("cannot read input file");
      p += r;
      n -= r;
    }
}

static void
far_input (_fptr_t buf, size_t n)
{
  char near_buf[0x100];
  const size_t NBUFSZ = sizeof (near_buf);
#ifdef __FAR
  char __far *p = buf;
#else
  _fptr_t p = buf;
#endif
  while (n)
    {
      ssize_t r;
      errno = 0;
      r = read (in_fd, near_buf, n < NBUFSZ ? n : NBUFSZ);
      if (r == 0 || r == -1)
	fatal ("cannot read input file");
      _fmemcpy (p, _CV_FP (near_buf), r);
#ifdef __FAR
      p += r;
#else
      p = MK_FP (FP_SEG (p), FP_OFF (p) + r);
#endif
      n -= r;
    }
}

int
main (int argc, char **argv)
{
  int opt;
  struct lz4_frame_hdr hdr;
  uint_le32_t blk_sz_le, blk_cksum_le, blk2_sz_le;
  uint32_t blk_sz32;
  size_t blk_sz, xlen = 0, i;
  _fptr_t blk;
#ifdef __FAR
  unsigned char __far *p;
#else
  _fptr_t p;
#endif

  _binmode (STDERR_FILENO);
  _splitpath (argv[0], NULL, NULL, me, NULL);

  while ((opt = getopt (argc, argv, "Fo:s:")) != -1)
    {
      switch (opt)
	{
	case 'F':
	  is_far = true;
	  break;
	case 'o':
	  out_path = optarg;
	  break;
	case 's':
	  symbol = optarg;
	  break;
	default:
	  usage ();
	}
    }

  switch (argc - optind)
    {
    default:
      usage ();
    case 0:
      break;  /* input is stdin */
    case 1:
      in_path = argv[optind];
    }

  if (in_path)
    {
      in_fd = open (in_path, O_RDONLY);
      if (in_fd == -1)
	fatal ("cannot open input file");
    }
  if (_binmode (in_fd) == -1)
    fatal ("cannot open input file in binary mode");

  input (&hdr, offsetof (struct lz4_frame_hdr, xlen));
  if (_leh32 (hdr.magic) != FH_MAG)
    fatal ("input is not LZ4");
  if ((hdr.flg & FLG_VER) != FLG_VER_CURR)
    fatal ("bad LZ4 version number");
  if ((hdr.flg & FLG_DICTID) != 0
      || (hdr.bd & BD_MASK) != BD_EXPECT)
    fatal ("LZ4 input uses unsupported feature");

  if ((hdr.flg & FLG_XLEN) != 0)
    {
#ifdef UINT64_MAX
      uint64_t xlen64;
#else
      uint32_t xlenlo;
#endif
      input (&hdr.xlen, sizeof (hdr.xlen));
#ifdef UINT64_MAX
      xlen64 = _leh64 (hdr.xlen);
      if (xlen64 > SIZE_MAX)
	fatal ("LZ4 input too large");
      xlen = (size_t) xlen64;
#else
      xlenlo = _leh64lo (hdr.xlen);
      if (xlenlo > (uint32_t) SIZE_MAX
	  || _leh64hi (hdr.xlen) != 0)
	fatal ("LZ4 input too large");
      xlen = (size_t) xlenlo;
#endif
    }
  input (&hdr.hc, sizeof (hdr.hc));

  input (&blk_sz_le, sizeof (blk_sz_le));
  blk_sz32 = _leh32 (blk_sz_le);
  if ((blk_sz32 & BSZ_UNCOMPR) != 0)
    fatal ("LZ4 uncompressed block unsupported");
  if (! blk_sz32)
    fatal ("unexpected LZ4 end mark");
  if (blk_sz32 > SIZE_MAX)
    fatal ("LZ4 input too large");
  blk_sz = (size_t) blk_sz32;

  blk = _fmalloc (blk_sz);
  if (_FP_EQ_NULL (blk))
    fatal ("cannot get memory for LZ4 input");
  far_input (blk, blk_sz);

  if ((hdr.flg & FLG_BCKSUM) != 0)
    input (&blk_cksum_le, sizeof (blk_cksum_le));
  input (&blk2_sz_le, sizeof (blk2_sz_le));
  if (_leh32 (blk2_sz_le) != 0)
    fatal ("multiple LZ4 blocks unsupported");
  if (in_path)
    close (in_fd);

  if (! xlen)
    {
      xlen = _lz4len (blk, blk_sz);
      if (! xlen)
	fatal ("LZ4 input too large");
    }

  if (out_path)
    {
      out_fd = open (out_path, O_WRONLY);
      if (out_fd == -1)
	fatal ("cannot open output file");
    }
  if (_binmode (out_fd) == -1)
    fatal ("cannot open output file in binary mode");

  outputs ("/* Automatically generated by " __FILE__ ". */" NL
	   "unsigned char ");
  if (is_far)
    outputs ("__far ");
  outputs (symbol);
  outputc ('[');
  outputu (xlen);
  outputs ("];" NL
	   "unsigned char ");
  if (is_far)
    outputs ("__far ");
  outputs (symbol);
  outputs ("_lz4[]" NL
	   "  = {" NL);
  p = blk;
  for (i = 0; i < blk_sz; ++i)
    {
      unsigned char b;
#ifdef __FAR
      b = *p++;
#else
      b = peekb (FP_SEG (p), FP_OFF (p));
      p = MK_FP (FP_SEG (p), FP_OFF (p) + 1);
#endif
      outputs ("      ");
      outputu ((size_t) b);
      outputs ("," NL);
    }
  outputs ("    };" NL);

  if (out_path)
    if (close (out_fd) == -1)
      fatal ("cannot close output file");

  return 0;
}
