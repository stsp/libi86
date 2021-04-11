/*
 * Copyright (c) 2020--2021 TK Chia
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
 * Simple diagnostic program which probes for floppy drives and hard drives
 * at the BIOS level, and tries to read the boot records on their disks.
 */

#include <stdio.h>
#include <bios.h>
#include <i86.h>

#ifndef _CV_FP
# define _CV_FP(ptr)	(ptr)
#endif

static void
test_drive (unsigned drive)
{
  unsigned short rv;
  unsigned retries = 5;
  struct diskinfo_t diskinfo;
  unsigned char buf[1024], *p;

  printf ("get parameters for drive %#x: ", drive);

  diskinfo.drive = drive;
  rv = _bios_disk (_DISK_DRIVEPARAMS, &diskinfo) >> 8;

  if (rv)
    printf ("failed: error %#x\n", rv);
  else 
    printf ("success\n"
	    "\tmax. track %u, max. head %u, max. sector %u\n",
	    diskinfo.track, diskinfo.head, diskinfo.sector);

  printf ("read boot sector from drive %#x: ", drive);

  while (retries-- != 0)
    {
      diskinfo.drive = drive;
      _bios_disk (_DISK_RESET, &diskinfo);

      diskinfo.drive = drive;
      diskinfo.track = diskinfo.head = 0;
      diskinfo.sector = diskinfo.nsectors = 1;
      p = buf;
      diskinfo.buffer = _CV_FP (p);

      rv = _bios_disk (_DISK_READ, &diskinfo) >> 8;
      if (rv)
	{
	  p = buf + 512;
	  diskinfo.buffer = _CV_FP (p);
	  rv = _bios_disk (_DISK_READ, &diskinfo) >> 8;
	}

      if (rv)
	continue;

      printf ("success\n"
	      "\t%02x %02x %02x %02x %02x %02x %02x %02x "
		"%02x %02x %02x %02x %02x %02x %02x %02x ...\n",
	      (unsigned) p[0], (unsigned) p[1], (unsigned) p[2],
	      (unsigned) p[3], (unsigned) p[4], (unsigned) p[5],
	      (unsigned) p[6], (unsigned) p[7], (unsigned) p[8],
	      (unsigned) p[9], (unsigned) p[10], (unsigned) p[11],
	      (unsigned) p[12], (unsigned) p[13], (unsigned) p[14],
	      (unsigned) p[15]);
      return;
    }

  printf ("failed: error %#x\n", rv);
}

int
main (void)
{
  test_drive(0x00);
  test_drive(0x01);
  test_drive(0x02);
  test_drive(0x03);
  test_drive(0x80);
  test_drive(0x81);
  test_drive(0x82);
  test_drive(0x83);
  return 0;
}
