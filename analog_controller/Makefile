# Copyright (C) 2026 Kyle de Nobel

# ==================================================================== #

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights 
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
# copies of the Software, and to permit persons to whom the Software is furnished 
# to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
# OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# ==================================================================== #

# Makefile for STM8 compliation with sdcc

# variable definitions
main_file := 

objects := 

# path objects are inlcuded via -I<path_name>
include_path := -Iinc/stm -Iinc

# include macros with -D
# for fast speed --opt-code-speed
# for small size --opt-code-siz
flags := --std-c11 -mstm8

binary_target := firmware.hex


.PHONY: help
help:
	$(info | ==============================================================)
	$(info | commands list)
	$(info | 	make help   => brings up this command list)
	$(info |	make verify => runs a version check for the compliler)
	$(info |	make flash  => flashes the hex to the target)
	$(info |	make build  => builds the hex")
	$(info | ==============================================================)

.PHONY: verify
verify:
	sdcc -v
	STVP_CmdLine -help

.PHONY: build
build: $(binary_target)

flash: $(binary_target)
	STVP_CmdLine -BoardName=ST-Link -Tool_ID=0 -ProgMode=SWIM -verbose -progress -erase -FileProg=$(binary_target) -verify



# recipes for each file

firmware.hex: $(objects)
	sdcc $(main_file) $(objects) -o firmware.hex


# NOTE: Page 31 of https://sdcc.sourceforge.net/doc/sdccman.pdf
# compiler can only handle 1 file at a time
# complile all non-main files then link them together when you compile main
# example on page 31
# linked files are .rel extension

