# flomod
[![The GPL License](https://img.shields.io/badge/license-GPL-yellow.svg?style=flat-square)](https://opensource.org/licenses/GPL-3.0)
[![Travis CI](https://img.shields.io/travis/Jacajack/flomod.svg?style=flat-square)](https://travis-ci.org/Jacajack/flomod)

[Flomod on launchpad...](https://launchpad.net/flomod)

`flomod` is a floppy disk image editor - it allows user to read or modify given sectors, tracks or bytes on floppy disk or its image.
It's relatively easy to use and can be pretty powerful tool when used along with other UNIX commands.

## Installing flomod
 - Add PPA to your system - `sudo add-apt-repository ppa:mrjjot/mrjjot`
 - Update software lists - `sudo apt-get update`
 - Install `flomod` package - `sudo apt-get install flomod`


## Usage
`flomod [OPTIONS] diskfile.bin`

Supported command line options:
 - `-1` / `--baseone` - Sector numbers begin with 1 (like in BIOS interrupt 13h)
 - `-s` / `--start` - Specify start point of read/write operation in C:H:S:B format
 - `-e` / `--end` - Specify end point of read/write operation in C:H:S:B format (overrides `-n` flag)
 - `-n` / `--num` - Specify length of read/write operation in C:H:S:B format
 - `-g` / `--geom` - Manually define disk geometry
 - `-t` / `--type` - Specify disk geometry preset
 - `-d` / `--dump` - Display written/read data along with its addresses in hex format
 - `-v` / `--verbose` - Be more verbose about what's going on
 - `-w` / `--write` - Write data from `stdin` to the image file
 - `-r` / `--read` - Read data from the image file

Disk geometry presets include:
 - `FLOPPY_3.5_740K` - 3.5 inch floppy, 740KB (equivalent of 80:2:9:512)
 - `FLOPPY_3.5_1.44M` - 3.5 inch floppy, 1.44MB (equivalent of 80:2:18:512)
 - `FLOPPY_3.5_2.88M` - 3.5 inch floppy, 2.88MB (equivalent of 80:2:36:512)
 - `FLOPPY_5.25_360K` - 5.25 inch floppy, 360KB (equivalent of 80:2:15:512)
 - `FLOPPY_5.25_1.2M` - 5.25 inch floppy, 1.2MB (equivalent of 40:2:9:512)

For example, if you want to read sector 3 from track 7 (side 2), you can use following command:<br>`flomod -s 7:1:3:0 -n 0:0:1:0 floppy.bin`.

## Building from source
 - Clone git repository - `git clone https://github.com/Jacajack/flomod.git && cd flomod`
 - Invoke makefile - `make all`
 - To install use `sudo make install`, and to uninstall - `sudo make uninstall`.
