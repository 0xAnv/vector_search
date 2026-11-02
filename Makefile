# Compiler settings
CC = clang
CFLAGS = -Wall -Wextra -O3

# Target executable name
TARGET = vec_generator

# Source and object files
SRC = vec_generator.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link the object file to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile the source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

# Declare non-file targets
.PHONY: all clean

