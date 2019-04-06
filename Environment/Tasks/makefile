GCC := /opt/cross/bin/i386-elf-gcc

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/Tasks.elf

C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
C_OBJECTS := $(C_SOURCES:%.c=$(OBJ_DIR)/%.o)

ASM_SOUCES := $(shell find $(SRC_DIR) -name "*.asm")
ASM_OBJECTS := $(ASM_SOUCES:%.asm=$(OBJ_DIR)/%.o)

DEPS := $(C_OBJECTS:.o=.d)

$(OUTPUT_FILE): $(C_OBJECTS) $(ASM_OBJECTS)
	@echo "=== Linking output file $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@$(GCC) $(C_OBJECTS) -o $(OUTPUT_FILE) -L../../Library/bin -lc -lgcc -nostdlib 
	@echo "=== $(OUTPUT_FILE) done ==="

$(OBJ_DIR)/%.o: %.c
	@echo "=== Compiling C file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(GCC) -c $(PWD)/$< -o $@ -MMD -MF $(@:.o=.d) -I../../Library -ggdb3 -O0 -ffreestanding -Wall -Wextra -Wno-unused-parameter

$(OBJ_DIR)/%.o: %.asm
	@echo "=== Compiling asm file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(NASM) $(PWD)/$< -f elf32 -o $@

-include $(DEPS)

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="