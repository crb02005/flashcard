#TABS NOT SPACES! - Richard Hendricks
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:src/%.c=obj/%.o)

# Output directory
BIN_DIR = bin

# Executable name
TARGET = $(BIN_DIR)/flashcard

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf obj $(BIN_DIR)

# Phony targets
.PHONY: all clean