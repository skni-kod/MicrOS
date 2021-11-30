GCC := /opt/cross/bin/i386-elf-gcc
AR := /opt/cross/bin/i386-elf-ar

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
OUTPUT_FILE := $(BIN_DIR)/libz.a

C_SOURCES := $(shell find $(SRC_DIR) -name "*.c")
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))

ASM_SOUCES := $(shell find $(SRC_DIR) -name "*.asm")
ASM_OBJECTS := $(ASM_SOUCES:%.asm=$(OBJ_DIR)/%.o)

DEPS := $(C_OBJECTS:.o=.d)

#GCC_COMPILE_FLAGS := -I../../library/src -ffreestanding -Wall -Wextra -Wno-unused-parameter -Wno-packed-bitfield-compat
GCC_COMPILE_FLAGS := -I../../library/src -Wall -Wextra -Wno-unused-parameter -Wno-packed-bitfield-compat -ffreestanding -Wno-implicit-fallthrough
#GCC_LINK_FLAGS := -L../../library/bin -lc -lgcc -nostdlib 

ifeq ($(MODE),release)
#GCC_COMPILE_FLAGS += -O2
#GCC_LINK_FLAGS += -s -DNDEBUG
else
#GCC_COMPILE_FLAGS += -O0 -ggdb3
endif

STATICLIB = libz.a
SHAREDLIB = zlib1.dll
IMPLIB    = libzdll.a

#LOC = -DASMV
#LOC = -DDEBUG -g

#CC = $(GCC)
#CFLAGS = $(GCC_COMPILE_FLAGS) $(LOC) -O0

#AS = $(CC)
#ASFLAGS = $(LOC) -Wall

#LD = $(CC)
#LDFLAGS = $(GCC_LINK_FLAGS) $(LOC) -s

#AR = ar
#ARFLAGS = rcs

#RC = windres
#RCFLAGS = --define GCC_WINDRES

#CP = cp -fp
# If GNU install is available, replace $(CP) with install.
#INSTALL = $(CP)
#RM = rm -f

#prefix = /usr/local
#exec_prefix = $(prefix)

OBJS = adler32.o compress.o crc32.o deflate.o gzio.o infback.o \
       inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o
OBJA =

#libz.a: $(OBJS)

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
#$(OUTPUT_FILE): $(C_OBJECTS) $(ASM_OBJECTS)
#	@echo "=== Linking output file $(OUTPUT_FILE) ==="
#	@mkdir -p '$(@D)'
#	@$(GCC) $(C_OBJECTS) -o $(OUTPUT_FILE) $(GCC_LINK_FLAGS)
#	@echo "=== $(OUTPUT_FILE) done ==="

#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
#	@echo "=== Compiling C file $< into object $@ ==="
#	@mkdir -p '$(@D)'
#	@$(GCC) -c $(CURDIR)/$< -o $@ -MMD -MF $(@:.o=.d) $(GCC_COMPILE_FLAGS)

#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
#	@echo "=== Compiling asm file $< into object $@ ==="
#	@mkdir -p '$(@D)'
#	@$(NASM) $(CURDIR)/$< -f elf32 -o $@

#-include $(DEPS)

#$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
#	$(CC) $(CFLAGS) -c -o $@ $<

#.S.o:
#	$(AS) $(ASFLAGS) -c -o $@ $<

#$(STATICLIB): $(OBJS) $(OBJA)
#	$(AR) $(ARFLAGS) $@ $(OBJS) $(OBJA)

#$(IMPLIB): $(SHAREDLIB)



#inftrees.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#adler32.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ 
#compress.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#crc32.o: $(SRC_DIR)/crc32.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#deflate.o: $(SRC_DIR)/deflate.h $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#example.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#gzio.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#inffast.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h $(SRC_DIR)/inflate.h $(SRC_DIR)/inffast.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#inflate.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h $(SRC_DIR)/inflate.h $(SRC_DIR)/inffast.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#infback.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h $(SRC_DIR)/inflate.h $(SRC_DIR)/inffast.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#minigzip.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#trees.o: $(SRC_DIR)/deflate.h $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/trees.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#uncompr.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
#zutil.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
#	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

adler32.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
compress.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
crc32.o: $(SRC_DIR)/crc32.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
deflate.o: $(SRC_DIR)/deflate.h $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
example.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
gzio.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
inffast.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h $(SRC_DIR)/inflate.h $(SRC_DIR)/inffast.h
inflate.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h $(SRC_DIR)/inflate.h $(SRC_DIR)/inffast.h
infback.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h $(SRC_DIR)/inflate.h $(SRC_DIR)/inffast.h
inftrees.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/inftrees.h
minigzip.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
trees.o: $(SRC_DIR)/deflate.h $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h $(SRC_DIR)/trees.h
uncompr.o: $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h
zutil.o: $(SRC_DIR)/zutil.h $(SRC_DIR)/zlib.h $(SRC_DIR)/zconf.h



#$(OUTPUT_FILE): win32/zlib1.rc
#	$(RC) $(RCFLAGS) -o $@ win32/zlib1.rc
clean:
	@echo "=== Clearing $(BIN_DIR) and $(OBJ_DIR) directories ==="
	rm -r -f $(BIN_DIR) $(OBJ_DIR)
	@echo "=== Done ==="