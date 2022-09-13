# By default, we tell `make` to remain silent. We can enable a more verbose
# output by passing `VERBOSE=1` to `make`.
VERBOSE 			?= 0
ifeq ($(VERBOSE), 0)
.SILENT:
endif

MAKEFLAGS 			+= --warn-undefined-variables

.DEFAULT_GOAL 		:= help

.SUFFIXES:

# include config
include ./Makefile.include

# We (more or less) follow the PFL project structure:
# https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#intro.dirs
build_dir        	= build
data_dir         	= data
external_dir     	= external
include_dir      	= include
log_dir          	= log
src_dir          	= src
tests_dir        	= tests
tools_dir        	= tools

# source directories
bootloader_src_dir	= $(src_dir)/bootloader
libk_src_dir     	= $(src_dir)/libk
libc_src_dir     	= $(src_dir)/libc
kernel_src_dir 		= $(src_dir)/kernel

# build directories
rootfs_dir		 	= $(build_dir)/rootfs
dist_dir         	= $(build_dir)/dist
kernel_objs_dir    	= $(build_dir)/obj/kernel
output_dir    	    = $(build_dir)/output
CWD 				= $(shell pwd)

# tools directories
cross_dir 			= $(tools_dir)/cross

# files
src_header_files 		+= $(shell find $(kernel_src_dir) -name "*.h")
src_header_files 		+= $(shell find $(libk_src_dir) -name "*.h")
src_header_files 		+= $(shell find $(libc_src_dir) -name "*.h")
header_files = $(addprefix $(include_dir)/, $(patsubst src/%,%,$(src_header_files)))


output_image 		= $(output_dir)/floppy.img
gdbconfig          	= .gdbconfig

# This is from: https://github.com/nuta and it allows to print nice progress messages
progress := printf "  \\033[1;96m%-8s\\033[0m  \\033[1;m%s\\033[0m\\n"

###############################################################################
# Tools
###############################################################################
CC       = $(cross_dir)/bin/$(ARCH)-elf-gcc
LD       = $(cross_dir)/bin/$(ARCH)-elf-ld
AR       = $(cross_dir)/bin/$(ARCH)-elf-ar
NM       = $(cross_dir)/bin/$(ARCH)-elf-nm
OBJCOPY  = $(cross_dir)/$(ARCH)-elf/bin/objcopy

DD		 = dd
MCOPY    = mcopy
CP 		 = cp
LN		 = ln
MKFS_DOS = /sbin/mkfs.msdos
ASM 	 = nasm
QEMU     = qemu-system-$(QEMU_ARCH)
TAR      = tar
MKDIR    = mkdir
CURL     = curl
RM		 = rm

# ifeq ($(UBSAN), 1)
# 	KERNEL_CFLAGS += -fsanitize=undefined
# endif

# ###############################################################################
# # Flags
# ###############################################################################
LD_FLAGS += -nostdlib

WERRORS  += -Wall
WERRORS  += -Wextra 
WERRORS  += -Wno-unused-parameter 
WERRORS  += -Wno-packed-bitfield-compat 
WERRORS  += -Wno-implicit-fallthrough

$(include_dir)/%.h: $(src_dir)/%.h
	$(progress) "LN" $@
	$(MKDIR) -p $(dir $@)
	ln -sf $(CWD)/$< $(CWD)/$@

#INCLUDES += -I$(include_dir)/libc/

# DEBUG_CFLAGS  += -O0 -g3 -DDEBUG_MODE

# ifeq ($(ENABLE_KERNEL_DEBUG), 1)
# 	DEBUG_CFLAGS += -DENABLE_KERNEL_DEBUG
# endif

# include bootloader-fat12
include $(bootloader_src_dir)/fat12/Makefile.include

# include libk
include $(libk_src_dir)/Makefile.include

# include kernel
include $(kernel_src_dir)/Makefile.include

# generate output image
$(output_image):
	$(progress) "GENERATE OUTPUT IMAGE"
	$(MKDIR) -p $(@D)
	$(MKDIR) -p $(rootfs_dir)
	$(MKFS_DOS) -C $@ 1440

files:
	$(CP) -f $(data_dir)/HELP.TXT $(rootfs_dir)/HELP.TXT
	$(CP) -f $(data_dir)/KERNEL.BIN $(rootfs_dir)/KERNEL.BIN
	$(MCOPY) -vi $(output_image) $(rootfs_dir)/KERNEL.BIN ::KERNEL.BIN
.PHONY: files

regenerate-image: 
	$(RM) -rf $(output_dir)
.PHONY: regenerate-image

includes: ## copy header files to header directory
includes: $(header_files)
.PHONY: includes

release: ## build the project in release mode
release: regenerate-image $(output_image) bootloader files
.PHONY: release

run-qemu: ## run qemu
run-qemu:
	$(QEMU) -m 128M\
	-drive file=$(output_image),format=raw,if=floppy
.PHONY: run-qemu

version: ## show version of tools
version:
	$(CC) --version
	$(ASM) --version
	$(LD) --version
	$(AR) --version
	$(QEMU) --version
.PHONY: version

docs: ## generate docs
docs: 
	rm -rf docs/*.* docs/search/
	doxygen ./Doxyfile
.PHONY: docs

clean-include: ## clean include
clean-include: 
	$(progress) "CLEAN $(include_dir)"
	rm -rf $(include_dir)
.PHONY: clean-include

clean: ## clean project output
clean: clean-include
	$(progress) "CLEAN"
	rm -rf $(build_dir)
.PHONY: clean

cross-setup: ## install cross compiler
cross-setup:
	$(progress) "Installing cross compiler"
	$(MKDIR) -p $(cross_dir)
	$(CURL) $(TOOLS_REPO)/$(CROSS_COMPILER) | tar -xz --strip-components=1 -C $(cross_dir)
.PHONY: cross-setup

help: ## show help
help:
	@/bin/echo "$(NAME) $(VERSION) - build system"
	@grep -hE '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
.PHONY: help
