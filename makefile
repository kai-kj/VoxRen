#------------------------------------------------------------------------------#
# important variables                                                          #
#------------------------------------------------------------------------------#

# binary name 
BIN := VoxRen

# include dir
INC := -I include/

# library dir
LIB := -L lib/

# libraries 
LIBS := -lOpenCL -lm -pthread -llua5.4 -lraylib -lGL -ldl -lrt -lX11

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
	./$(BIN) g data/scenes/car

debug: CC += -g
debug: clean $(BIN)
	gdb $(BIN)

$(BUILD):
	$(MKDIR) $(BUILD)
	$(MKDIR) $(BUILD)/renderer
	$(MKDIR) $(BUILD)/gui
	$(MKDIR) $(BUILD)/scripting
	$(MKDIR) $(BUILD)/kGui
	$(MKDIR) $(BUILD)/headless

$(BIN): $(BUILD)
	$(CC) -c $(SRC)/renderer/management.c -o $(BUILD)/renderer/management.a
	$(CC) -c $(SRC)/renderer/scene.c -o $(BUILD)/renderer/scene.a
	$(CC) -c $(SRC)/renderer/camera.c -o $(BUILD)/renderer/camera.a
	$(CC) -c $(SRC)/renderer/material.c -o $(BUILD)/renderer/material.a
	$(CC) -c $(SRC)/renderer/global.c -o $(BUILD)/renderer/global.a
	$(CC) -c $(SRC)/renderer/io.c -o $(BUILD)/renderer/io.a

	$(CC) -c $(SRC)/gui/window.c -o $(BUILD)/gui/window.a
	$(CC) -c $(SRC)/gui/main_loop.c -o $(BUILD)/gui/main_loop.a
	$(CC) -c $(SRC)/gui/global.c -o $(BUILD)/gui/global.a
	$(CC) -c $(SRC)/gui/mouse_input.c -o $(BUILD)/gui/mouse_input.a
	$(CC) -c $(SRC)/gui/kb_input.c -o $(BUILD)/gui/kb_input.a

	$(CC) -c $(SRC)/kGui/kGui.c -o $(BUILD)/kGui/kGui.a
	$(CC) -c $(SRC)/kGui/global.c -o $(BUILD)/kGui/global.a
	$(CC) -c $(SRC)/kGui/utils.c -o $(BUILD)/kGui/utils.a
	$(CC) -c $(SRC)/kGui/component.c -o $(BUILD)/kGui/component.a
	$(CC) -c $(SRC)/kGui/grid.c -o $(BUILD)/kGui/grid.a
	$(CC) -c $(SRC)/kGui/window.c -o $(BUILD)/kGui/window.a
	$(CC) -c $(SRC)/kGui/label.c -o $(BUILD)/kGui/label.a
	$(CC) -c $(SRC)/kGui/button.c -o $(BUILD)/kGui/button.a
	$(CC) -c $(SRC)/kGui/textbox.c -o $(BUILD)/kGui/textbox.a
	$(CC) -c $(SRC)/kGui/pos_tracker.c -o $(BUILD)/kGui/pos_tracker.a

	$(CC) -c $(SRC)/scripting/script.c -o $(BUILD)/scripting/script.a
	$(CC) -c $(SRC)/scripting/command.c -o $(BUILD)/scripting/command.a
	$(CC) -c $(SRC)/scripting/command_wrapper.c -o $(BUILD)/scripting/command_wrapper.a
	$(CC) -c $(SRC)/scripting/global.c -o $(BUILD)/scripting/global.a

	$(CC) -c $(SRC)/headless/headless.c -o $(BUILD)/headless/headless.a

	$(CC) -c $(SRC)/main.c -o $(BUILD)/main.o

	$(CC) $(BUILD)/main.o $(BUILD)/renderer/* $(BUILD)/gui/* $(BUILD)/scripting/* $(BUILD)/kGui/* $(BUILD)/headless/* -o $(BIN) $(LIBS)

clean:
	$(RM) $(BIN) $(BUILD) log.txt
