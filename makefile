#------------------------------------------------------------------------------#
# important variables                                                          #
#------------------------------------------------------------------------------#

# binary name 
BIN := pathtracer

# include dir
INC := -I include/

# library dir
LIB := -L lib/

# libraries 
LIBS := -lOpenCL -lm -lSDL2 -lSDL2_ttf -pthread -llua5.4

# flags 
FLAGS := -Wall -Wno-missing-braces

# defines
DEFS := -D CL_TARGET_OPENCL_VERSION=300 -D K_UTIL_DEBUG
#------------------------------------------------------------------------------#
# other variables                                                              #
#------------------------------------------------------------------------------#

# commands 
CC := gcc $(FLAGS) $(DEFS) $(INC) $(LIB)
MV := mv
RM := rm -rf
CP := cp
MKDIR := mkdir -p
ECHO := @echo

# folders 
BUILD := build
SRC := src

#------------------------------------------------------------------------------#
# build process                                                                #
#------------------------------------------------------------------------------#

.PHONY: default run clean

default: clean $(BIN)

run: clean $(BIN)
	./$(BIN)

debug: CC += -g
debug: clean $(BIN)
	gdb $(BIN)

$(BUILD):
	$(MKDIR) $(BUILD)
	$(MKDIR) $(BUILD)/renderer
	$(MKDIR) $(BUILD)/gui
	$(MKDIR) $(BUILD)/scripting

$(BIN): $(BUILD)
	$(CC) -c $(SRC)/renderer/management.c -o $(BUILD)/renderer/management.a
	$(CC) -c $(SRC)/renderer/scene.c -o $(BUILD)/renderer/scene.a
	$(CC) -c $(SRC)/renderer/camera.c -o $(BUILD)/renderer/camera.a
	$(CC) -c $(SRC)/renderer/material.c -o $(BUILD)/renderer/material.a
	$(CC) -c $(SRC)/renderer/global.c -o $(BUILD)/renderer/global.a

	$(CC) -c $(SRC)/gui/window.c -o $(BUILD)/gui/window.a
	$(CC) -c $(SRC)/gui/main_loop.c -o $(BUILD)/gui/main_loop.a
	$(CC) -c $(SRC)/gui/input.c -o $(BUILD)/gui/input.a
	$(CC) -c $(SRC)/gui/place_voxel.c -o $(BUILD)/gui/place_voxel.a
	$(CC) -c $(SRC)/gui/global.c -o $(BUILD)/gui/global.a

	$(CC) -c $(SRC)/scripting/script.c -o $(BUILD)/scripting/script.a
	$(CC) -c $(SRC)/scripting/command_wrapper.c -o $(BUILD)/scripting/command_wrapper.a
	$(CC) -c $(SRC)/scripting/global.c -o $(BUILD)/scripting/global.a

	$(CC) -c $(SRC)/main.c -o $(BUILD)/main.o

	$(CC) $(BUILD)/main.o $(BUILD)/renderer/* $(BUILD)/gui/* $(BUILD)/scripting/* -o $(BIN) $(LIBS)

clean:
	$(RM) $(BIN) $(BUILD) log.txt
