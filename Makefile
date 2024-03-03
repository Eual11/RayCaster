# OBJ = ./src/test5.cpp
OBJ = ./src/tm.cpp
BUILD_NAME = e
INCLUDE_PATH =-I"${CPLUS_INCLUDE_PATH}/SDL2" 
COMPILE_FLAGS =   -Wall -Wpedantic -Wextra -g -std=c++17
LINKER_FLAGS = -lmingw32  -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
LIB_PATH = -L."${C_LIBRARY_PATH}"
CC = g++

all: $(OBJ)
	$(CC) $(OBJ) $(INCLUDE_PATH) $(LIB_PATH) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o ./build/$(BUILD_NAME)
