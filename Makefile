.PHONY: all i386 x86_64 clean help

PROJECT := paping

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

CC ?= gcc

CPPFLAGS ?= -I$(SRC_DIR)
WARNFLAGS ?= -Wall -Wextra -Wpedantic
CFLAGS ?= -std=gnu89 -O2 -pipe $(WARNFLAGS)

SOURCES_C := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/cli/arguments.c \
	$(SRC_DIR)/cli/i18n.c \
	$(SRC_DIR)/net/host.c \
	$(SRC_DIR)/net/socket.c \
	$(SRC_DIR)/utils/gettimeofday.c \
	$(SRC_DIR)/utils/print.c \
	$(SRC_DIR)/utils/stats.c \
	$(SRC_DIR)/utils/timer.c

OBJ_DIR_I386 := $(BUILD_DIR)/i386
OBJ_DIR_X86_64 := $(BUILD_DIR)/x86_64

OBJECTS_I386 := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_I386)/%.o,$(SOURCES_C))
OBJECTS_X86_64 := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR_X86_64)/%.o,$(SOURCES_C))

DEPS_I386 := $(OBJECTS_I386:.o=.d)
DEPS_X86_64 := $(OBJECTS_X86_64:.o=.d)

BIN_I386 := $(BIN_DIR)/i386-$(PROJECT)
BIN_X86_64 := $(BIN_DIR)/x86-64-$(PROJECT)

all: i386 x86_64

help:
	@echo "Targets:"
	@echo "  make all     - build i386 + x86_64"
	@echo "  make i386    - build $(BIN_I386)"
	@echo "  make x86_64  - build $(BIN_X86_64)"
	@echo "  make clean   - remove build artifacts"

i386: $(BIN_I386)

x86_64: $(BIN_X86_64)

$(BIN_I386): $(OBJECTS_I386)
	@mkdir -p $(BIN_DIR)
	$(CC) -m32 $^ -o $@

$(BIN_X86_64): $(OBJECTS_X86_64)
	@mkdir -p $(BIN_DIR)
	$(CC) -m64 $^ -o $@

$(OBJ_DIR_I386)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -m32 $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR_X86_64)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -m64 $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS_I386)
-include $(DEPS_X86_64)
