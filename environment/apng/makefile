GCC := /opt/cross/bin/i386-elf-gcc
AR := /opt/cross/bin/i386-elf-ar

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/libpng.a

C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))

ASM_SOUCES := $(shell find $(SRC_DIR) -name "*.asm")
ASM_OBJECTS := $(ASM_SOUCES:%.asm=$(OBJ_DIR)/%.o)

DEPS := $(C_OBJECTS:.o=.d)

GCC_COMPILE_FLAGS := -I../../library/src -I../../lib/zlib -Wall -Wextra -Wno-unused-parameter -Wno-packed-bitfield-compat -ffreestanding -Wno-implicit-fallthrough

ifeq ($(MODE),release)
#GCC_COMPILE_FLAGS += -O2
#GCC_LINK_FLAGS += -s -DNDEBUG
else
#GCC_COMPILE_FLAGS += -O0 -ggdb3
endif

OBJS =  png.o pngerror.o pngget.o pngmem.o pngpread.o \
	pngread.o pngrio.o pngrtran.o pngrutil.o pngset.o \
	pngtrans.o pngwio.o pngwrite.o pngwtran.o pngwutil.o


$(OUTPUT_FILE): $(C_OBJECTS) $(ASM_OBJECTS)
	@echo "=== Linking output file $(OUTPUT_FILE) ==="
	@mkdir -p '$(@D)'
	@$(AR) rcs $(OUTPUT_FILE) $(C_OBJECTS) $(ASM_OBJECTS)
	@echo "=== $(OUTPUT_FILE) done ==="

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "=== Compiling C file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(GCC) -c $(CURDIR)/$< -o $@ -MMD -MF $(@:.o=.d) $(GCC_COMPILE_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	@echo "=== Compiling asm file $< into object $@ ==="
	@mkdir -p '$(@D)'
	@$(NASM) $(CURDIR)/$< -f elf32 -o $@


-include $(DEPS)

png.o:      png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngerror.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngget.o:   png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngmem.o:   png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngpread.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngread.o:  png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngrio.o:   png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngrtran.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngrutil.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngset.o:   png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngtrans.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngwio.o:   png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngwrite.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngwtran.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h
pngwutil.o: png.h pngconf.h pnglibconf.h pngpriv.h pngstruct.h pnginfo.h pngdebug.h

pngtest.o:  png.h pngconf.h pnglibconf.h


clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="