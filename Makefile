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
apps_src_dir		= $(src_dir)/apps

# build directories
dist_dir         	= $(build_dir)/dist
output_dir    	    = $(build_dir)/output
# tools directories
ifndef CROSS
	cross_dir 		= $(tools_dir)/cross
else
	cross_dir 		= ${CROSS}
endif

# files
src_header_files 	+= $(shell find $(kernel_src_dir) -name "*.h")
src_header_files 	+= $(shell find $(libk_src_dir) -name "*.h")
src_header_files 	+= $(shell find $(libc_src_dir) -name "*.h")
header_files 		= $(addprefix $(include_dir)/, $(patsubst $(src_dir)/%,%,$(src_header_files)))
src_data_files 		+= $(shell find $(data_dir))
data_files 			= $(patsubst $(data_dir)/%,%,$(src_data_files))
log_file 			= $(log_dir)/qemu.log
output_image 		= $(output_dir)/floppy.img
gdbconfig          	= .gdbconfig


# ifeq ($(UBSAN), 1)
# 	KERNEL_CFLAGS += -fsanitize=undefined
# endif
# ifeq ($(ENABLE_KERNEL_DEBUG), 1)
# 	DEBUG_CFLAGS += -DENABLE_KERNEL_DEBUG
# endif

# include bootloader-fat12
include $(bootloader_src_dir)/fat12/Makefile.include

# include libk
include $(libk_src_dir)/Makefile.include

# include kernel
include $(kernel_src_dir)/Makefile.include

# include libc
include $(libc_src_dir)/Makefile.include

$(include_dir)/%.h: $(src_dir)/%.h
	$(progress) "LN" $@
	$(MKDIR) -p $(dir $@)
	ln -sf $(CWD)/$< $(CWD)/$@

$(log_dir):
	$(progress) "MKDIR" $@
	$(MKDIR) -p $@

# generate output image
$(output_image):
	$(progress) "GENERATE OUTPUT IMAGE"
	$(MKDIR) -p $(@D)
	$(MKFS_DOS) -C $@ 1440
# copy files from data directory to 
$(data_files):
	$(progress) "MCOPY" $@ 
	$(MCOPY) -oi $(output_image) $(data_dir)/$@ ::$@

release: ## build the project in release mode
release: $(output_image) $(header_files) bootloader kernel $(data_files) $(libc_bin)
.PHONY: release

run-qemu: ## run qemu
run-qemu: $(log_dir)
	$(QEMU) $(QEMU_OPTIONS) \
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
