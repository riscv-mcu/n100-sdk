# See LICENSE for license details.

ifndef _NUCLEI_MK_COMMON
_NUCLEI_MK_COMMON := # defined

.PHONY: all
all: $(TARGET)

# download has three option:  flashxip, flash, ilm
DOWNLOAD    := flash
SIMULATION    := 0

SOC_DRIVER_DIR = $(BSP_BASE)/soc/drivers

DRIVER_DIR = $(BSP_BASE)/core/drivers
ENV_DIR = $(BSP_BASE)/core/env
STUB_DIR = $(BSP_BASE)/core/stubs

   # The start-up assembly program
ASM_SRCS += $(ENV_DIR)/start.S
   # The vector table
ifeq ($(DOWNLOAD),flash) 
ASM_SRCS += $(ENV_DIR)/vtable_ilm.S
else
ASM_SRCS += $(ENV_DIR)/vtable.S
endif
   # The system initilization program
C_SRCS += $(ENV_DIR)/init.c
   # The interrupt/exception/nmi entry program
ASM_SRCS += $(ENV_DIR)/entry.S
   # The interrupt/exception/nmi handler program
C_SRCS += $(ENV_DIR)/handlers.c
   # The processor core common functions
C_SRCS += $(DRIVER_DIR)/func.c
   # The newlib stubs functions
C_SRCS += $(STUB_DIR)/write.c
C_SRCS += $(STUB_DIR)/isatty.c
#C_SRCS += $(STUB_DIR)/close.c
#C_SRCS += $(STUB_DIR)/read.c
#C_SRCS += $(STUB_DIR)/fstat.c
#C_SRCS += $(STUB_DIR)/lseek.c


   # The SoC common functions
C_SRCS += $(SOC_DRIVER_DIR)/soc_func.c

ifeq ($(YOUR_PRINTF),1) 
C_SRCS += $(ENV_DIR)/your_printf.c
endif

ifeq ($(DOWNLOAD),flash) 
LINKER_SCRIPT := $(ENV_DIR)/link_flash.lds
endif

ifeq ($(DOWNLOAD),ilm) 
LINKER_SCRIPT := $(ENV_DIR)/link_ilm.lds
endif


INCLUDES += -I$(BSP_BASE)/
INCLUDES += -I$(BSP_BASE)/core
INCLUDES += -I$(BSP_BASE)/soc

LDFLAGS += -T $(LINKER_SCRIPT)  -nostartfiles   -Wl,--check-sections  #-Wl,--gc-sections
LDFLAGS += --specs=nano.specs --specs=nosys.specs

ifeq ($(PFLOAT),1) 
ifeq ($(YOUR_PRINTF),0)
LDFLAGS += -u _printf_float 
endif
endif


LDFLAGS += -L$(ENV_DIR)

ASM_OBJS := $(ASM_SRCS:.S=.o)
C_OBJS := $(C_SRCS:.c=.o)
DUMP_OBJS := $(C_SRCS:.c=.dump)
VERILOG_OBJS := $(C_SRCS:.c=.verilog)

LINK_OBJS += $(ASM_OBJS) $(C_OBJS)
LINK_DEPS += $(LINKER_SCRIPT)

CLEAN_OBJS += $(TARGET) $(LINK_OBJS) $(DUMP_OBJS) $(VERILOG_OBJS)

CFLAGS += -g
CFLAGS += -march=$(RISCV_ARCH)
CFLAGS += -mabi=$(RISCV_ABI)
CFLAGS += -mcmodel=medlow 
CFLAGS += -ffunction-sections -fdata-sections -fno-common
ifeq ($(SIMULATION),1) 
CFLAGS += -DCFG_SIMULATION
endif
ifeq ($(YOUR_PRINTF),1) 
CFLAGS += -fno-builtin-printf 
endif


$(TARGET): $(LINK_OBJS) $(LINK_DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LINK_OBJS) -o $@ $(LDFLAGS)
	$(SIZE) $@

$(ASM_OBJS): %.o: %.S $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(C_OBJS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -include sys/cdefs.h -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(CLEAN_OBJS)

endif # _NUCLEI_MK_COMMON
