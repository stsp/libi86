/*
 * Copyright (c) 2020 TK Chia
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; see the file COPYING3.LIB.  If not see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <bios.h>

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
      diskinfo.buffer = p;

      rv = _bios_disk (_DISK_READ, &diskinfo) >> 8;
      if (rv)
	{
	  p = buf + 512;
	  diskinfo.buffer = p;
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
