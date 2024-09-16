# Compiler and flags
CC = g++
CFLAGS = -O1 -Wall -std=c++17 -Wno-missing-braces -I../include/

# Platform-specific libraries
ifeq ($(OS),Windows_NT)
    LDFLAGS = -L../lib/windows  # Path for Windows libraries
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = $(BIN_DIR)/Game.exe
else
    LDFLAGS = -L/usr/local/lib  # Path where shared libs are installed (usually /usr/local/lib for Linux)
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET = $(BIN_DIR)/Game
endif

# Directories for source, build, and binaries
SRC_DIR = ../src
BUILD_DIR = ../build
BIN_DIR = ../exe

# Source and object files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Default target to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Rule for building object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean run

# Clean target to remove object files and the executable
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Run the target based on the platform
run: $(TARGET)
ifeq ($(OS),Windows_NT)
	./$(TARGET)
else
	./$(TARGET)
endif
