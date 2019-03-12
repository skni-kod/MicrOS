GCC := /opt/cross/bin/i386-elf-gcc

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/Shell.elf

SOURCES := $(shell find $(SRC_DIR) -name "*.c" -o -name "*.h")
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

$(OUTPUT_FILE): $(OBJECTS)
	@echo "=== Linking output file $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@$(GCC) $(OBJECTS) -o $(OUTPUT_FILE) -L../../Library/bin -lc -g -ffreestanding -nostdlib 
	@echo "=== Done ==="

$(OBJ_DIR)/%.o: %.c
	@echo "=== Compiling file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(GCC) -c $(PWD)/$< -o $@ -I../../Library -g -ffreestanding -Wall -Wextra -Wno-unused-parameter

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="