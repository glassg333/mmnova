# Makefile для mnm_filter
# Стандарт: C++17, без зависимостей (только стандартная библиотека).
# Использование:
#   make        — собрать demo
#   make run    — собрать и запустить
#   make clean  — очистить

CXX      ?= g++
CXXSTD   ?= c++17
WARN     ?= -Wall -Wextra -Wpedantic -O2
ROOT     := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BUILD    := $(ROOT)/build

DEMO_SRC := $(ROOT)/demo/demo.cpp
DEMO_BIN := $(BUILD)/demo

.PHONY: all run clean dirs

all: dirs $(DEMO_BIN)

dirs:
	@mkdir -p $(BUILD)

$(DEMO_BIN): $(DEMO_SRC) $(ROOT)/src/MnmFilter.h $(ROOT)/src/mnm_filter_tables.h
	$(CXX) -std=$(CXXSTD) $(WARN) -I$(ROOT)/src $(DEMO_SRC) -o $(DEMO_BIN)

run: $(DEMO_BIN)
	$(DEMO_BIN)

clean:
	rm -rf $(BUILD)
