TARGET := coremark

C_SRCS := \
	core_list_join.c \
	core_main.c \
	core_matrix.c \
	core_state.c \
	core_util.c \
	core_portme.c \

HEADERS := \
	coremark.h \
	core_portme.h \

#CFLAGS := -O2 -fno-common -funroll-loops -finline-functions -falign-functions=4 -falign-jumps=4 -falign-loops=4
CFLAGS := -O2 -funroll-all-loops -finline-limit=600 -ftree-dominator-opts -fno-if-conversion2 -fselective-scheduling -fno-code-hoisting -fno-common -funroll-loops -finline-functions -falign-functions=4 -falign-jumps=4 -falign-loops=4
#CFLAGS := -O3  -funroll-loops -fpeel-loops -fgcse-sm -fgcse-las -flto

CFLAGS += -DFLAGS_STR=\""$(CFLAGS)"\"
CFLAGS += -DITERATIONS=10000 -DPERFORMANCE_RUN=1

BSP_BASE = ../../bsp

CORE_SERIE = $(shell echo $(BOARD) | sed 's/.*-//')

include $(BSP_BASE)/core/env/common.mk
