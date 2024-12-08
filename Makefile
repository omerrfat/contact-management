# Compiler and flags
CC = gcc
CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0)

# Source files
LOGIC_SRC = PandACW_logic.c
GUI_SRC = contact_manager_gui.c

# Object files
LOGIC_OBJ = $(LOGIC_SRC:.c=.o)
GUI_OBJ = $(GUI_SRC:.c=.o)

# Executable name
TARGET = contact_manager

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(LOGIC_OBJ) $(GUI_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to compile logic source file
$(LOGIC_OBJ): $(LOGIC_SRC)
	$(CC) -c $< $(CFLAGS)

# Rule to compile GUI source file
$(GUI_OBJ): $(GUI_SRC)
	$(CC) -c $< $(CFLAGS)

# Clean up generated files
clean:
	rm -f $(TARGET) $(LOGIC_OBJ) $(GUI_OBJ)

# Phony targets (not real files)
.PHONY: all clean
