# flomod 1 "27 June 2017" "v0.6"

## NAME
**flomod** - floppy disk image editor.

## SYNOPSIS
**flomod** [`OPTIONS`] *filename*

## DESCRIPTION
**flomod** is a floppy disk image editor - it allows user to read or modify given sectors, tracks or bytes on floppy disk or its image.
It's relatively easy to use and can be pretty powerful tool when used along with other UNIX commands.

Supported command line options:
 - `-1` / `--baseone` - Sector numbers begin with 1 (like in BIOS interrupt 13h)
 - `-e` / `--end` - Specify end point of read/write operation in C:H:S:B format (overrides `-n` flag)
 - `-d` / `--dump` - Display written/read data along with its addresses in hex format
 - `-l` / `--limits` - Manually define disk geometry
 - `-n` / `--num` - Specify length of read/write operation in C:H:S:B format
 - `-r` / `--read` - Read data from the image file
 - `-s` / `--start` - Specify start point of read/write operation in C:H:S:B format
 - `-t` / `--type` - Specify disk geometry preset
 - `-u` / `--uppercase` - Output uppercase text, when decrypting
 - `-v` / `--verbose` - Be more verbose about what's going on
 - `-w` / `--write` - Write data from `stdin` to the image file


Predefined disk geometries:
 - `FLOPPY_3.5_740K` - 3.5 inch floppy, 740KB (80:2:9:512)
 - `FLOPPY_3.5_1.44M` - 3.5 inch floppy, 1.44MB (80:2:18:512)
 - `FLOPPY_3.5_2.88M` - 3.5 inch floppy, 2.88MB (80:2:36:512)
 - `FLOPPY_5.25_360K` - 5.25 inch floppy, 360KB (80:2:15:512)
 - `FLOPPY_5.25_1.2M` - 5.25 inch floppy, 1.2MB (40:2:9:512)

## EXAMPLES
Read 12th sector from track 5, side 1:
	`flomod -s 5:0:12:0 -n 0:0:1:0 file.bin`
Read sectors from 3 to 12 from track 7, side 2:
	`flomod -s 7:1:3:0 -e 7:1:5:0 file.bin`
Read bytes 100-200 from first sector from track 0, side 1 and be verbose (base 1 sector numbering):
	`flomod -1 -v -s 0:0:1:100 -e 0:0:1:200 file.bin`
Dump data from floppy that has 2 sides (heads) and 45 sectors (512b) on each one of 70 tracks:
	`flomod -d -l 70:2:45:512 file.bin`

## AUTHOR
Flomod has been created by Jacek Wieczorek <mrjjot@gmail.com>

## REPORTING BUGS
Bugs can be reported as issues on Flomod's Github page: https://github.com/Jacajack/flomod/issues

## COPYRIGHT
Flomod is licensed under GNU GPLv3 license.

Copyright (C) 2017 Jacek Wieczorek <mrjjot@gmail.com>
Free Software Foundation, Inc.  License GPLv3+: GNU
GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free  to  change  and  redistribute  it.
There is NO WARRANTY, to the extent permitted by law.
