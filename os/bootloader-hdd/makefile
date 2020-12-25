GCC := /opt/cross/bin/i386-elf-gcc
NASM := "../../Tools/nasm"

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/bootloader.bin

ASM_SOUCES := $(shell find $(SRC_DIR) -name "*.asm")
ASM_OBJECTS := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOUCES))

$(OUTPUT_FILE): $(ASM_OBJECTS)
	@echo "=== Copying output bootloader $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@cp $< $(OUTPUT_FILE)
	@echo "=== $(OUTPUT_FILE) done ==="

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@echo "=== Compiling asm file $(PWD)/$< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(NASM) $< -f bin -o $@

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="