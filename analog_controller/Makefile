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

# build path
build_path := build

source_path := src

library_path := lib/src

library_include_path := lib/inc

# variable definitions
main_file := $(source_path)/main.c

library := $(library_path)/gpio.c

objects := $(build_path)/gpio.rel


# path objects are inlcuded via -I<path_name>
include_path := -Iinc/stm -Iinc -Ilib/inc -Ilib

# include macros with -D
# for fast speed --opt-code-speed
# for small size --opt-code-siz
flags := --std-c11 -mstm8 --out-fmt-elf

lib_flags := $(flags) -c

binary_target := $(build_path)/firmware.elf


.PHONY: help
help:
	$(info | ================================================================ |)
	$(info | commands list                                                    |)
	$(info |    make help          => brings up this command list             |)
	$(info |    make verify        => runs a version check for the compliler  |)
	$(info |    make flash         => flashes the hex to the target           |)
	$(info |    make flash-nucleo  => flashes the hex to the target           |)
	$(info |    make build         => builds the elf binary target            |)
	$(info |    make clean         => removes all build artifacts             |)
	$(info | ================================================================ |)


.PHONY: verify
verify:
	sdcc -v
	STVP_CmdLine -help


.PHONY: build
build: $(binary_target)


.PHONY: clean
clean:
	$(info removing all build artifacts)
	rm $(build_path)/*.*
	touch marco.log
	rm *.log
	touch $(build_path)/marco.txt
	echo "HERE SO BUILD FOLDER SHOWS UP IN GIT. ALSO FUNNY" > $(build_path)/marco.txt


.PHONY: flash
flash: $(binary_target)
	STVP_CmdLine \
	-BoardName=ST-Link \
	-Tool_ID=0 \
	-Port=USB \
	-ProgMode=SWIM \
	-Device=STM8L15xK4 \
	-verbose \
	-progress \
	-erase \
	-FileProg=$(binary_target) \
	-verif


.PHONY: flash-nucleo
flash-nucleo: $(binary_target)
	STVP_CmdLine \
	-BoardName=ST-Link \
	-Tool_ID=0 \
	-Port=USB \
	-ProgMode=SWIM \
	-Device=STM8L15xR8 \
	-verbose \
	-progress \
	-erase \
	-FileProg=$(binary_target) \
	-verif




# recipes for each file

$(binary_target): $(objects)
	sdcc $(main_file) $(objects) $(flags) $(include_path) -o $(build_path)/firmware.elf


# NOTE: Page 31 of https://sdcc.sourceforge.net/doc/sdccman.pdf
# compiler can only handle 1 file at a time
# complile all non-main files then link them together when you compile main
# example on page 31
# linked files are .rel extension


$(build_path)/gpio.rel: $(library_include_path)/gpio.h
	sdcc $(library_path)/gpio.c $(lib_flags) $(include_path) -o $(build_path)/gpio.rel

