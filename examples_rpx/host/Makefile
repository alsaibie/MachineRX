TARGET = Host

DEBUG = 1
# optimization
ifeq ($(DEBUG), 1)
	OPT = -O0
  	BUILD_TYPE = Debug
else
	OPT = -Og
	BUILD_TYPE = Release
endif

BUILD_DIR ?= ./build
SRC_DIRS  ?= ./src
MRPX_DIR   ?= ../../machinerpx
APPS_DIR  ?= ../apps
TOPICS_DIR ?= ../topics

SRC_DIRS += $(APPS_DIR)/

CPP = g++
CC  = gcc
AS  = gcc -x assembler-with-cpp
CP  = objcopy
SZ  = size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPP_SOURCES = $(shell find $(SRC_DIRS) -name *.cpp)
C_SOURCES   = $(shell find $(SRC_DIRS) -name *.c)
ASM_SOURCE  = $(shell find $(SRC_DIRS) -name *.s)

# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS += $(MRPX_DIR)/include
INC_DIRS += $(APPS_DIR)
INC_DIRS += $(TOPICS_DIR)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++17 $(OPT)
CFLAGS ?= $(INC_FLAGS) -MMD -MP $(OPT) 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2 -DDEBUG 
CPPFLAGS += -g -gdwarf-2 -DDEBUG  
endif

LIBS = -lc -lm -lstdc++ -lpthread
LDFLAGS = $(LIBS)

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# List of CPP / C / ASM Objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# c++ source
$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	$(CPP) -pthread -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@  
# c source
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c -pthread $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
# assembly
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@	

.PHONY: all run clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)
