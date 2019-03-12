GCC := /opt/cross/bin/i386-elf-gcc
OBJCOPY := /opt/cross/i386-elf/bin/objcopy
NASM := "../../Tools/nasm"

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/kernel.elf

ASM_SOUCES := $(shell find $(SRC_DIR) -name "*.asm" -type f -exec readlink -f {} \;)
ASM_OBJECTS := $(ASM_SOUCES:%.asm=$(OBJ_DIR)/%.o)

$(OUTPUT_FILE): $(C_OBJECTS) $(ASM_OBJECTS)
	@echo "=== Copying output bootloader $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@cp $< $(BIN_DIR)/bootloader.bin
	@echo "=== Done ==="

$(OBJ_DIR)/%.o: %.asm
	@echo "=== Compiling bootloader $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(NASM) $< -f bin -o $@

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="