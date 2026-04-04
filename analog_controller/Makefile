######################################
# target
######################################
TARGET := firmware

######################################
# source
######################################

CORE_APP_PATH := Core
CORE_APP_SRC_PATH := $(CORE_APP_PATH)/Src
STMLIBS_PATH := lib/GTSR/stmlibs/g4
STMHAL_PATH := lib/ST/STM32G4xx_HAL_Driver
CMSIS_STM32G4_PATH := lib/ARM/CMSIS/Device/ST/STM32G4xx
CMSIS_ARM_PATH := lib/ARM/CMSIS
THIRD_PARTY_PATH := lib/third-party
TOOLS_PATH := tools

# C application sources
C_APP_SOURCES :=  \
$(CORE_APP_SRC_PATH)/main.c \

# C++ application sources
CXX_APP_SOURCES := $(CORE_APP_SRC_PATH)/test.cpp

# Other C library source files
C_LIB_SOURCES := $(wildcard $(THIRD_PARTY_PATH)/printf/src/printf/*.c)

# Other C++ library source files
CXX_LIB_SOURCES :=

# Get all STMlibs source files
STMLIB_C_SOURCES := $(wildcard $(STMLIBS_PATH)/src/*.c)
# Get all STM hal source files
STMHAL_C_SOURCES := $(wildcard $(STMHAL_PATH)/Src/*.c)

# Files for initalization
INIT_FILES := \
init/system_stm32g4xx.c \
init/stm32g4xx_hal_msp.c \

# C_APP_SOURCES should be first, so those files are most likely to have compile bugs
# We want to exit early if compile error
C_SOURCES := $(C_APP_SOURCES)
C_SOURCES += $(C_LIB_SOURCES)
C_SOURCES += $(STMLIB_C_SOURCES)
C_SOURCES += $(STMHAL_C_SOURCES)
C_SOURCES += $(INIT_FILES)

# C++ sources
CXX_SOURCES := $(CXX_APP_SOURCES)
CXX_SOURCES += $(CXX_LIB_SOURCES)

# Directory for header files
APP_INCLUDE_PATHS += \
$(CORE_APP_PATH)/Inc \
$(CORE_APP_PATH)/Config \

# Directory for library header files
LIB_INCLUDE_PATHS += \
$(STMHAL_PATH)/Inc \
$(CMSIS_STM32G4_PATH)/Include \
$(CMSIS_ARM_PATH)/Include \
$(STMLIBS_PATH)/inc \
$(THIRD_PARTY_PATH)/printf/src \
$(THIRD_PARTY_PATH)

INCLUDE_PATHS += $(APP_INCLUDE_PATHS) $(LIB_INCLUDE_PATHS) 

# ASM sources
ASM_SOURCES :=  \
init/startup_stm32g474xx.s

# ASMM sources
ASMM_SOURCES := 

# ASMMC sources
ASMMC_SOURCE := 

######################################
# Variables
######################################

# Flags specific to each project if needed
EXTRA_CFLAGS := 
EXTRA_CXXFLAGS :=
EXTRA_LDFLAGS :=
# If a specific file need an additional specific compile flag, then use the following to do so
# Replace <FILE_NAME> with variable that holds the group of files you want with the specific flag
# EXTRA_CFLAGS += $(addprefix $(OBJ_PATH)/,$(notdir $(<FILE_NAME>:.c=.o)))


################################################################
################### Do not modify beyond here ##################
################################################################

RELEASE := release
ERROR := error
WARN := warn
INFO := info
DEBUG := debug
VERBOSE := verbose
ANALYZE := analyze

OPENOCD_CFG_SCRIPT := $(TOOLS_PATH)/stm32g4x-stlink-swd.cfg


#######################################
# paths
#######################################

# Build path
BUILD_DIR := build
RELEASE_DIR ?= release

# Change build directory based on what build type
ifeq ($(ERROR), $(firstword $(MAKECMDGOALS)))
    RELEASE_DIR := $(ERROR)
else ifeq ($(WARN), $(firstword $(MAKECMDGOALS)))
    RELEASE_DIR := $(WARN)
else ifeq ($(INFO), $(firstword $(MAKECMDGOALS)))
    RELEASE_DIR := $(INFO)
else ifeq ($(DEBUG), $(firstword $(MAKECMDGOALS)))
    RELEASE_DIR := $(DEBUG)
else ifeq ($(VERBOSE), $(firstword $(MAKECMDGOALS)))
    RELEASE_DIR := $(VERBOSE)
else ifeq ($(ANALYZE), $(firstword $(MAKECMDGOALS)))
    RELEASE_DIR := $(ANALYZE)
endif

BUILD_PATH := $(BUILD_DIR)/$(RELEASE_DIR)
DEPS_PATH := $(BUILD_PATH)/.deps
OBJDUMP_PATH := $(BUILD_PATH)/.objdump
OBJ_PATH := $(BUILD_PATH)/objs
VERSION_PATH := $(BUILD_PATH)/version

#####################################
# Compilation targets
####################################

# Compile with 8 jobs to make jobs faster
MAKEFLAGS += -j8

# Rules for various compile configurations
.PHONY: $(RELEASE) $(ERROR) $(WARN) $(INFO) $(DEBUG) $(VERBOSE) $(ANALYZE) 
.PHONY: dirs clean distclean flash erase prologue epilogue force format format-all format-check format-check-all

# default action: build release
$(RELEASE): prologue $(BUILD_PATH)/$(TARGET).elf $(BUILD_PATH)/$(TARGET).hex $(BUILD_PATH)/$(TARGET).bin epilogue
# Each rule does samething but with different configuration variables
$(ERROR): $(RELEASE)
$(WARN): $(RELEASE)
$(INFO): $(RELEASE)
$(DEBUG): $(RELEASE)
$(VERBOSE): $(RELEASE)
$(ANALYZE): $(RELEASE)

#######################################
# binaries
#######################################
PREFIX := arm-none-eabi-

CC := $(PREFIX)gcc
CXX := $(PREFIX)g++
AS := $(PREFIX)gcc -x assembler-with-cpp
CP := $(PREFIX)objcopy
SZ := $(PREFIX)size
OBJDUMP := $(PREFIX)objdump
HEX := $(CP) -O ihex
BIN := $(CP) -O binary -S
FMT := clang-format

#######################################
# CFLAGS
#######################################

# Do not use := for assigning any variable that uses DEBUG_FLAG or OPT or ANALYZER
# Debug flags
DEBUG_FLAG ?= -DNDEBUG -DGTSR_LOG_LEVEL=GTSR_LOG_NONE
$(ERROR): DEBUG_FLAG := -DNDEBUG -DGTSR_ERROR -DGTSR_LOG_LEVEL=GTSR_LOG_ERROR
$(WARN): DEBUG_FLAG := -DNDEBUG -DGTSR_WARN -DGTSR_LOG_LEVEL=GTSR_LOG_WARN -DUSE_FULL_ASSERT
$(INFO): DEBUG_FLAG := -DDEBUG -g1 -ggdb -DGTSR_INFO -DGTSR_LOG_LEVEL=GTSR_LOG_INFO -DUSE_FULL_ASSERT
$(DEBUG): DEBUG_FLAG := -DDEBUG -g2 -ggdb -DGTSR_DEBUG -DGTSR_LOG_LEVEL=GTSR_LOG_DEBUG -DUSE_FULL_ASSERT
$(VERBOSE): DEBUG_FLAG := -DDEBUG -g2 -ggdb -DGTSR_VERBOSE -DGTSR_LOG_LEVEL=GTSR_LOG_VERBOSE -DUSE_FULL_ASSERT
$(ANALYZE): DEBUG_FLAG := -DDEBUG -DGTSR_VERBOSE -DGTSR_LOG_LEVEL=GTSR_LOG_VERBOSE -DUSE_FULL_ASSERT

# optimization flag
OPT ?= -O2 -flto
$(ERROR): OPT := -O2
$(WARN): OPT := -O2
$(INFO): OPT := -O1
$(DEBUG): OPT := -Og
$(VERBOSE): OPT := -Og
$(ANALYZE): OPT := -O2

# Static analyzer flag
$(ANALYZE): ANALYZER := -fanalyzer


# cpu
CPU := -mcpu=cortex-m4

# fpu
FPU := -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI := -mfloat-abi=hard

# mcu
MCU := $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS := 

# C/C++ defines
C_DEFS :=  \
-DUSE_HAL_DRIVER \
-DSTM32G474xx \
-D_FORTIFY_SOURCE=1 \
-DUSE_FULL_LL_DRIVER \
-DPRINTF_INCLUDE_CONFIG_H

# AS includes
AS_INCLUDES := 

# C/C++ includes
C_INCLUDES := $(foreach dir, $(INCLUDE_PATHS), -I$(dir))
C_INCLUDES += -I$(VERSION_PATH)

# Warnings share between C and C++
SHARED_WARNINGS := \
-Wall \
-Wextra \
-Wdouble-promotion \
-Wformat-nonliteral \
-Wnull-dereference \
-Winit-self \
-Wuninitialized \
-Wshift-overflow=2 \
-Wswitch-default \
-Wstrict-overflow=4 \
-Wfloat-equal \
-Wshadow \
-Wpointer-arith \
-Wundef \
-Wcast-qual \
-Wfloat-conversion \
-Wpacked \
-Werror=format-nonliteral 

# Find third-party files so we don't compile with warnings. 
# Don't compile with warnings on 3rd party since we won't be fixing them usually
# Use flag option if 3rd party files should have warnings enabled
ifndef FULL_WARNING
C_OBJS_NO_WARNINGS := $(addprefix $(OBJ_PATH)/,$(notdir $(C_LIB_SOURCES:.c=.o)))
C_OBJS_NO_WARNINGS += $(addprefix $(OBJ_PATH)/,$(notdir $(STMHAL_C_SOURCES:.c=.o)))

CXX_OBJS_NO_WARNINGS := $(addprefix $(OBJ_PATH)/,$(notdir $(CXX_LIB_SOURCES:.cpp=.o)))
endif
# C warning flags
C_WARNINGS = \
$(SHARED_WARNINGS) \
$(ANALYZER) \
-Werror=implicit-function-declaration

$(C_OBJS_NO_WARNINGS): C_WARNINGS := 

# C++ warning flags
CXX_WARNINGS = \
$(ANALYZER) \
$(SHARED_WARNINGS)

$(C_OBJS_NO_WARNINGS): CXX_WARNINGS := 

# C standard
C_STD := -std=gnu17

# C++ standard
CXX_STD := -std=c++23

# Use newlib nano spec
C_SPEC := --specs=nano.specs

# Seperate each function and data into seperate objects for dead code elmination in linker
C_SEP := -fdata-sections -ffunction-sections

# Generate dependency information (Do not use := for this assignment)
CDEPS = -MMD -MP -MF"$(@:$(OBJ_PATH)/%.o=$(DEPS_PATH)/%.d)"

# asm assembler flags
ASFLAGS = \
$(MCU) \
$(AS_DEFS) \
$(AS_INCLUDES) \
$(OPT) \
$(C_WARNINGS) \
$(C_SEP)

# C compiler flags
CFLAGS = \
$(MCU) \
$(C_STD) \
$(C_DEFS) \
$(C_INCLUDES) \
$(OPT) \
$(C_WARNINGS) \
$(C_SPEC) \
$(C_SEP) \
-ffreestanding \
$(CDEPS) \
$(DEBUG_FLAG) \
$(EXTRA_CFLAGS)

# C++ compiler flags
CXXFLAGS = \
$(MCU) \
$(CXX_STD) \
$(C_DEFS) \
$(C_INCLUDES) \
$(OPT) \
$(CXX_WARNINGS) \
$(C_SPEC) \
$(C_SEP) \
-ffreestanding \
-fno-exceptions \
$(CDEPS) \
$(DEBUG_FLAG) \
$(EXTRA_CXXFLAGS)

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT := init/STM32G474XX_FLASH.ld

# linker specific options
LDOPTS := \
--static \
-Wl,-Map=$(BUILD_PATH)/$(TARGET).map \
-Wl,--gc-sections \
-Wl,--start-group \
-Wl,--end-group

# libraries
LIBS := -lc -lm
LIBDIR := 
LDFLAGS = $(MCU) $(OPT) $(C_SPEC) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) $(LDOPTS) $(EXTRA_LDFLAGS)

#######################################
# build the application
#######################################

# list of C++ objects
OBJECTS := $(addprefix $(OBJ_PATH)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))

# list of C objects
OBJECTS += $(addprefix $(OBJ_PATH)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(OBJ_PATH)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(OBJ_PATH)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))
OBJECTS += $(addprefix $(OBJ_PATH)/,$(notdir $(ASMMC_SOURCES:.c=.o)))
vpath %.S $(sort $(dir $(ASMMC_SOURCES)))

# Compile C files
$(OBJ_PATH)/%.o: %.c Makefile | dirs prologue
	@$(CC) -c $(CFLAGS) $< -o $@
	@$(OBJDUMP) -D -x -C -l -S $@ > $(OBJDUMP_PATH)/$(notdir $(<:.c=.as))

# Compile C++ files
$(OBJ_PATH)/%.o: %.cpp Makefile | dirs prologue
	@$(CXX) -c $(CXXFLAGS) $< -o $@
	@$(OBJDUMP) -D -x -C -l -S $@ > $(OBJDUMP_PATH)/$(notdir $(<:.cpp=.as))

# Assemble asm files
$(OBJ_PATH)/%.o: %.s Makefile | dirs prologue
	@$(AS) -c $(CFLAGS) $< -o $@
$(OBJ_PATH)/%.o: %.S Makefile | dirs prologue
	@$(AS) -c $(CFLAGS) $< -o $@

# Link all objects together and report size
$(BUILD_PATH)/$(TARGET).elf: $(OBJECTS) Makefile init/STM32G474XX_FLASH.ld | dirs prologue
	@echo "(Re)linking ELF binary"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Make a hex dump of elf file
$(BUILD_PATH)/%.hex: $(BUILD_PATH)/%.elf Makefile | dirs prologue
	$(HEX) $< $@
	
# Make a binary dump of elf file
$(BUILD_PATH)/%.bin: $(BUILD_PATH)/%.elf Makefile | dirs prologue
	$(BIN) $< $@

# Generate version info header
$(VERSION_PATH)/version.h: force | dirs
	$(TOOLS_PATH)/version_info_generator.py "$(TARGET)" "$@"


# Make required directories
dirs:
	@mkdir -p $(BUILD_PATH) $(DEPS_PATH) $(OBJDUMP_PATH) $(OBJ_PATH) $(VERSION_PATH)

# Phony to require a target to rebuild
force:

# Inital prints to indicate what is happening
prologue: $(VERSION_PATH)/version.h
	@echo "The C compile flags are:"
	@for flag in $(CFLAGS) ; do \
    	echo "\t $$flag" ; \
	done
# only print C++ flags if having C++ sources
ifneq ($(CXX_SOURCES),)
	@echo "The C++ compile flags are:"
	@for flag in $(CXXFLAGS) ; do \
    	echo "\t $$flag" ; \
	done
endif
	@echo "The linking flags are:"
	@for flag in $(LDFLAGS) ; do \
    	echo "\t $$flag" ; \
	done
	@echo "The C source files are:"
	@for file in $(C_SOURCES) ; do \
    	echo "\t $$file" ; \
	done
	@echo "The C++ source files are:"
	@for file in $(CXX_SOURCES) ; do \
    	echo "\t $$file" ; \
	done
	@echo "The ASM source files are:"
	@for file in $(ASM_SOURCES) ; do \
    	echo "\t $$file" ; \
	done
	@for file in $(ASMM_SOURCES) ; do \
    	echo "\t $$file" ; \
	done
	@for file in $(ASMMC_SOURCES) ; do \
    	echo "\t $$file" ; \
	done
	@echo "The include paths are:"
	@for dir in $(C_INCLUDES) ; do \
    	echo "\t $$dir" ; \
	done

# Do not change order of prerequisites
epilogue: $(BUILD_PATH)/$(TARGET).elf $(BUILD_PATH)/$(TARGET).hex $(BUILD_PATH)/$(TARGET).bin
	@echo "Linking has finished"
	$(SZ) $<

#######################################
# clean up
#######################################

# Delete object files and end binaries
clean:
	find $(BUILD_DIR) -type f -name "*.o" -delete
	find $(BUILD_DIR) -type f -name "$(TARGET).*" -delete

# Delete build directory
distclean:
	-rm -fR $(BUILD_DIR)

#######################################
# Flashing
######################################

# Flash with openocd

# flash the target
flash : $(BUILD_PATH)/$(TARGET).elf
	openocd -f $(OPENOCD_CFG_SCRIPT) \
		-c "init" \
		-c "reset halt" \
		-c "program $< verify reset exit"

# erase the flash on the target
erase :
	openocd -f $(OPENOCD_CFG_SCRIPT) \
		-c "init" \
		-c "halt" \
		-c "flash erase_sector 0 0 last" \
		-c "reset run" \
		-c "exit"


# Flash using ST Prog

# flash the target
flash-stprog : $(BUILD_PATH)/$(TARGET).elf
	STM32_Programmer_CLI -c port=SWD
	STM32_Programmer_CLI -c port=SWD -e all
	STM32_Programmer_CLI -c port=SWD -d $<
	STM32_Programmer_CLI -c port=SWD -rst

# erase the flash on the target
erase-stprog :
	STM32_Programmer_CLI -c port=SWD
	STM32_Programmer_CLI -c port=SWD -e all


# gdb : $(BUILD_PATH)/$(TARGET).elf
# 	openocd -f $(OPENOCD_CFG_SCRIPT)
# 	gdb $<

#######################################
# Formatting
######################################

APP_INCLUDES := $(shell find $(APP_INCLUDE_PATHS) -name "*.h")
APP_INCLUDES += $(shell find $(APP_INCLUDE_PATHS) -name "*.hpp")

# Format application files
format:
	echo $(CXX_APP_SOURCES) $(C_APP_SOURCES) $(APP_INCLUDES) | xargs -n 4 -P 4 $(FMT) -i --style=file

# Check if applications files are formatted properly (for CI)
format-check:
	echo $(CXX_APP_SOURCES) $(C_APP_SOURCES) $(APP_INCLUDES) | xargs -n 4 -P 4 $(FMT) --style=file --dry-run -Werror

LIB_INCLUDES := $(shell find $(LIB_INCLUDE_PATHS) -name "*.h")
LIB_INCLUDES += $(shell find $(LIB_INCLUDE_PATHS) -name "*.hpp")

# Format application and lib files
format-all:
	echo $(CXX_SOURCES) $(C_SOURCES) $(APP_INCLUDES) $(LIB_INCLUDES) | xargs -n 4 -P 4 $(FMT) -i --style=file

# Check if application and lib files are formatted properly (for CI)
format-check-all:
	echo $(CXX_SOURCES) $(C_SOURCES) $(APP_INCLUDES) $(LIB_INCLUDES) | xargs -n 4 -P 4 $(FMT) --style=file --dry-run -Werror

#######################################
# dependencies
#######################################
-include $(wildcard $(DEPS_PATH)/*.d)

# *** EOF ***