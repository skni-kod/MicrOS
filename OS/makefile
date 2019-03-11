GCC := /opt/cross/bin/i386-elf-gcc
OBJCOPY := /opt/cross/i386-elf/bin/objcopy
NASM := "../Tools/nasm"

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/kernel.elf

SOURCES := $(shell find $(SRC_DIR)/Kernel -name "*.c")
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

A := $(shell find $(SRC_DIR)/Kernel -name "*.asm")
B := $(A:%.asm=$(OBJ_DIR)/%.o)

DEPS := $(OBJECTS:.o=.d)

$(OUTPUT_FILE): $(OBJECTS) $(B) $(Y)
	@echo "=== Linking output file $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@$(GCC) $(OBJECTS) $(B) -o $(OUTPUT_FILE) -T linker.ld -L../Library/bin -lc -g3 -ffreestanding -nostdlib 
	@echo "=== Creating binary version of kernel elf ==="
	@$(OBJCOPY) -O binary $(OUTPUT_FILE) $(BIN_DIR)/kernel.bin
	@echo "=== Done ==="

$(OBJ_DIR)/%.o: %.c
	@echo "=== Compiling file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(GCC) -c $< -o $@ -MMD -MF $(@:.o=.d) -I../Library -IKernel/Library -g3 -ffreestanding -Wall -Wextra -Wno-unused-parameter -fshort-enums
	
$(OBJ_DIR)/%.o: %.asm
	@echo "=== Compiling asm $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(NASM) $< -f elf32 -o $@
	
-include $(DEPS)

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="