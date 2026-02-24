.PHONY: all i386 x86_64 clean help

PROJECT := paping

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

CXX ?= g++

CPPFLAGS ?= -I$(SRC_DIR)
WARNFLAGS ?= -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++11 -O2 -pipe $(WARNFLAGS)

SOURCES_CPP := \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/cli/arguments.cpp \
	$(SRC_DIR)/cli/i18n.cpp \
	$(SRC_DIR)/net/host.cpp \
	$(SRC_DIR)/net/socket.cpp \
	$(SRC_DIR)/utils/print.cpp \
	$(SRC_DIR)/utils/stats.cpp \
	$(SRC_DIR)/utils/timer.cpp

OBJ_DIR_I386 := $(BUILD_DIR)/i386
OBJ_DIR_X86_64 := $(BUILD_DIR)/x86_64

OBJECTS_I386 := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR_I386)/%.o,$(SOURCES_CPP))
OBJECTS_X86_64 := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR_X86_64)/%.o,$(SOURCES_CPP))

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
	$(CXX) -m32 $^ -o $@

$(BIN_X86_64): $(OBJECTS_X86_64)
	@mkdir -p $(BIN_DIR)
	$(CXX) -m64 $^ -o $@

$(OBJ_DIR_I386)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -m32 $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR_X86_64)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -m64 $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS_I386)
-include $(DEPS_X86_64)
