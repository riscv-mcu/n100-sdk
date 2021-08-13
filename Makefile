#############################################################
# Configuration
#############################################################

# Allows users to create Makefile.local or ../Makefile.project with
# configuration variables, so they don't have to be set on the command-line
# every time.
extra_configs := $(wildcard Makefile.local ../Makefile.project)
ifneq ($(extra_configs),)
$(info Obtaining additional make variables from $(extra_configs))
include $(extra_configs)
endif

SIMULATION     := 0

# default the program is downloaded to ilm
DOWNLOAD    := ilm

CORE        := n101

core_name = $(shell echo $(CORE) | tr A-Z a-z)

ifeq ($(core_name),n101) 
RISCV_ARCH := rv32ec
RISCV_ABI  := ilp32e
endif



PFLOAT     := 0
YOUR_PRINTF     := 0

# Default target
PROGRAM ?= dhrystone
GDB_PORT ?= 3333


#############################################################
# Prints help message
#############################################################
.PHONY: help
help:
	@echo "  Nuclei N-series RISC-V Embedded Processor Software Development Kit "
	@echo "  Makefile targets:"
	@echo ""
	@echo " dasm [PROGRAM=$(PROGRAM)]:"
	@echo "    Build a software program to load with the"
	@echo "    debugger."
	@echo ""
	@echo " upload [PROGRAM=$(PROGRAM)]:"
	@echo "    Launch OpenOCD to flash your program to the"
	@echo "    on-board Flash."
	@echo ""
	@echo " run_openocd :"
	@echo " run_gdb     [PROGRAM=$(PROGRAM) ]:"
	@echo "     Launch OpenOCD or GDB seperately. Allows Ctrl-C to halt running"
	@echo "     programs."
	@echo ""
	@echo ""


# Pointers to various important tools in the toolchain.
RISCV_COMPILER_PATH ?= ./prebuilt_tools/riscv-nuclei-elf-gcc

RISCV_GCC     := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-gcc)
RISCV_GXX     := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-g++)
RISCV_OBJDUMP := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-objdump)
RISCV_OBJCOPY := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-objcopy)
RISCV_GDB     := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-gdb)
RISCV_AR      := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-ar)
RISCV_SIZE    := $(abspath $(RISCV_COMPILER_PATH)/bin/riscv-nuclei-elf-size)

PATH := $(abspath $(RISCV_PATH)/bin):$(PATH)

$(RISCV_GCC) $(RISCV_GXX) $(RISCV_OBJDUMP) $(RISCV_GDB) $(RISCV_AR): $(toolchain_builddir)/install.stamp
	touch -c $@

.PHONY: 
clean: 

#############################################################
# This Section is for Software Compilation
#############################################################
PROGRAM_DIR = software/$(PROGRAM)
PROGRAM_ELF = software/$(PROGRAM)/$(PROGRAM)

.PHONY: clean
clean:
	$(MAKE) -C $(PROGRAM_DIR) BSP_BASE=$(abspath bsp)  DOWNLOAD=$(DOWNLOAD) clean

.PHONY: software
software: clean
	$(MAKE) -C $(PROGRAM_DIR) SIZE=$(RISCV_SIZE) CC=$(RISCV_GCC) RISCV_ARCH=$(RISCV_ARCH) YOUR_PRINTF=$(YOUR_PRINTF)  PFLOAT=$(PFLOAT)  DOWNLOAD=$(DOWNLOAD)  RISCV_ABI=$(RISCV_ABI) AR=$(RISCV_AR) BSP_BASE=$(abspath bsp) SIMULATION=$(SIMULATION)

dasm: software 
	$(RISCV_OBJDUMP) -S -D $(PROGRAM_ELF) >& $(PROGRAM_ELF).dump
	$(RISCV_OBJCOPY) $(PROGRAM_ELF) -O verilog $(PROGRAM_ELF).verilog
	sed -i 's/@0008/@0000/g' $(PROGRAM_ELF).verilog



#############################################################
# This Section is for uploading a program to SPI Flash
#############################################################
RISCV_OPENOCD_PATH ?= ./prebuilt_tools/openocd
RISCV_OPENOCD ?= $(RISCV_OPENOCD_PATH)/bin/openocd


ifeq ($(DOWNLOAD),ilm)
OPENOCDCFG ?= bsp/core/env/openocd_hbird_ilm.cfg
else
OPENOCDCFG ?= bsp/core/env/openocd_hbird.cfg
endif

OPENOCDARGS += -f $(OPENOCDCFG)

GDB_UPLOAD_ARGS ?= --batch

GDB_UPLOAD_CMDS += -ex "set remotetimeout 240"
GDB_UPLOAD_CMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
GDB_UPLOAD_CMDS += -ex "monitor reset halt"
GDB_UPLOAD_CMDS += -ex "monitor flash protect 0 0 last off"
GDB_UPLOAD_CMDS += -ex "load"
GDB_UPLOAD_CMDS += -ex "monitor resume"
GDB_UPLOAD_CMDS += -ex "monitor shutdown"
GDB_UPLOAD_CMDS += -ex "quit"

upload:
	$(RISCV_OPENOCD) $(OPENOCDARGS) & \
	$(RISCV_GDB) $(PROGRAM_DIR)/$(PROGRAM) $(GDB_UPLOAD_ARGS) $(GDB_UPLOAD_CMDS) && \
	echo "Successfully uploaded '$(PROGRAM)' "

#############################################################
# This Section is for launching the debugger
#############################################################

run_openocd:
	$(RISCV_OPENOCD) $(OPENOCDARGS)

GDBCMDS += -ex "set remotetimeout 240"
GDBCMDS += -ex "target extended-remote localhost:$(GDB_PORT)"

run_gdb:
	$(RISCV_GDB) $(PROGRAM_DIR)/$(PROGRAM) $(GDBARGS) $(GDBCMDS)

