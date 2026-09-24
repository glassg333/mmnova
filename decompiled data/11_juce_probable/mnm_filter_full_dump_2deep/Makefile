# Makefile для mnm_filter
# Стандарт: C++17, без зависимостей (только стандартная библиотека).
# Использование:
#   make          — собрать demo + compare
#   make run      — собрать и запустить demo
#   make compare  — собрать и запустить сравнение (convenience vs faithful)
#   make clean    — очистить

CXX      ?= g++
CXXSTD   ?= c++17
WARN     ?= -Wall -Wextra -Wpedantic -O2
ROOT     := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
BUILD    := $(ROOT)/build

DEMO_SRC := $(ROOT)/demo/demo.cpp
DEMO_BIN := $(BUILD)/demo
CMP_SRC  := $(ROOT)/demo/compare.cpp
CMP_BIN  := $(BUILD)/compare

.PHONY: all run clean dirs compare

all: dirs $(DEMO_BIN) $(CMP_BIN)

dirs:
	@mkdir -p $(BUILD)

$(DEMO_BIN): $(DEMO_SRC) $(ROOT)/src/MnmFilter.h $(ROOT)/src/MnmFilterFaithful.h $(ROOT)/src/mnm_filter_tables.h
	$(CXX) -std=$(CXXSTD) $(WARN) -I$(ROOT)/src $(DEMO_SRC) -o $(DEMO_BIN)

$(CMP_BIN): $(CMP_SRC) $(ROOT)/src/MnmFilter.h $(ROOT)/src/MnmFilterFaithful.h $(ROOT)/src/mnm_filter_tables.h
	$(CXX) -std=$(CXXSTD) $(WARN) -I$(ROOT)/src $(CMP_SRC) -o $(CMP_BIN)

run: $(DEMO_BIN)
	$(DEMO_BIN)

compare: $(CMP_BIN)
	$(CMP_BIN)

clean:
	rm -rf $(BUILD)
