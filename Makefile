PROJECT := timetablegen

CXX := g++
FLAGS := -std=c++20 -O2 -Wall -pedantic

BIN_DIR := bin
OBJ_DIR := obj
SRC_DIR := src
DOC_DIR := doc

TARGET := $(BIN_DIR)/$(PROJECT)

SOURCES := $(wildcard ${SRC_DIR}/*.cpp  ${SRC_DIR}/*/*.cpp)
OBJECTS := $(patsubst ${SRC_DIR}/%.cpp, ${OBJ_DIR}/%.o, ${SOURCES})
ROOT := -I ./src

.PHONY: default run clean doc

default: ${TARGET}

${TARGET}: ${OBJECTS}
	@mkdir -p $(dir $@)
	${CXX} ${FLAGS} $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	${CXX} ${FLAGS} ${ROOT} -c $< -o $@

run: ${TARGET}
	./bin/${NAME}

clean: 
	@rm -rf ${BIN_DIR}
	@rm -rf ${OBJ_DIR}
	@rm -rf ${DOC_DIR}

doc: 
	doxygen Doxyfile