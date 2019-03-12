GCC := /opt/cross/bin/i386-elf-gcc
AR := /opt/cross/bin/i386-elf-ar

SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/libc.a

SOURCES := $(shell find $(SRC_DIR) -name "*.c")
OBJECTS := $(SOURCES:%.c=$(OBJ_DIR)/%.o)

DEPS := $(OBJECTS:.o=.d)

$(OUTPUT_FILE): $(OBJECTS)
	@echo "=== Linking output file $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@$(AR) rcs $(OUTPUT_FILE) $(OBJECTS)
	@echo "=== Done ==="

$(OBJ_DIR)/%.o: %.c
	@echo "=== Compiling file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(GCC) -c $(PWD)/$< -o $@ -MMD -MF $(@:.o=.d) -ffreestanding -Wall -Wextra -Wno-unused-parameter

-include $(DEPS)

clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="