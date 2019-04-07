GCC := /opt/cross/bin/i386-elf-gcc

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/tasks.elf

C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
C_OBJECTS := $(C_SOURCES:%.c=$(OBJ_DIR)/%.o)

ASM_SOUCES := $(shell find $(SRC_DIR) -name "*.asm")
ASM_OBJECTS := $(ASM_SOUCES:%.asm=$(OBJ_DIR)/%.o)

DEPS := $(C_OBJECTS:.o=.d)

GCC_COMPILE_FLAGS := -I../../Library -ffreestanding -Wall -Wextra -Wno-unused-parameter
GCC_LINK_FLAGS := -L../../Library/bin -lc -lgcc -nostdlib

ifeq ($(MODE),release)
GCC_COMPILE_FLAGS += -O2
GCC_LINK_FLAGS += -s -DNDEBUG
else
GCC_COMPILE_FLAGS += -O0 -ggdb3
endif

$(OUTPUT_FILE): $(C_OBJECTS) $(ASM_OBJECTS)
	@echo "=== Linking output file $(OUTPUT_FILE) ==="
	@mkdir -p '$(subst $(SRC_DIR),,$(@D))'
	@$(GCC) $(subst $(SRC_DIR),,$(C_OBJECTS)) -o $(OUTPUT_FILE) $(GCC_LINK_FLAGS)
	@echo "=== $(OUTPUT_FILE) done ==="

$(OBJ_DIR)/%.o: %.c
	@echo "=== Compiling C file $< into object $(subst $(SRC_DIR),,$@) ==="
	@mkdir -p '$(subst $(SRC_DIR),,$(@D))'
	@$(GCC) -c $(PWD)/$< -o $(subst $(SRC_DIR),,$@) -MMD -MF $(subst $(SRC_DIR),,$(@:.o=.d)) $(GCC_COMPILE_FLAGS)

$(OBJ_DIR)/%.o: %.asm
	@echo "=== Compiling asm file $< into object $(subst $(SRC_DIR),,$@) ==="
	@mkdir -p '$(subst $(SRC_DIR),,$(@D))'
	@$(NASM) $(PWD)/$< -f elf32 -o $(subst $(SRC_DIR),,$@)

-include $(DEPS)

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="