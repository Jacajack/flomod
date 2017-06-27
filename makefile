# flomod - a floppy disk image editor
# Copyright (C) 2017 Jacek Wieczorek <mrjjot@gmail.com>
# This file is part of flomod.
# Flomod is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Flomod is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC = gcc
CFLAGS = -Wall

all: clean src/chsb.c src/flomod.c src/args.c
	$(CC) $(CFLAGS) -o flomod src/chsb.c src/flomod.c src/args.c

install:
	-mkdir -p $(DESTDIR)/usr
	-mkdir -p $(DESTDIR)/usr/bin
	cp -R flomod $(DESTDIR)/usr/bin

uninstall:
	-rm -rf $(DESTDIR)/usr/bin/flomod

clean:
	-rm flomod

manpage:
	md2man-roff doc/flomod.1.md > man/flomod.1
