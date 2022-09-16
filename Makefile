# By default, we tell `make` to remain silent. We can enable a more verbose
# output by passing `VERBOSE=1` to `make`.
VERBOSE				?= 0
ifeq ($(VERBOSE), 0)
.SILENT:
endif

MAKEFLAGS			+= --warn-undefined-variables

.DEFAULT_GOAL		:= help

.SUFFIXES:

# include config
include ./Makefile.include

# We (more or less) follow the PFL project structure:
# https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#intro.dirs
ifndef BUILD_DIR
	build_dir		= build
else
	build_dir		= ${BUILD_DIR}
endif
data_dir			= data
external_dir		= external
include_dir			= $(build_dir)/include
ifndef LOG_DIR
	log_dir			= log
else
	log_dir			= ${LOG_DIR}
endif
src_dir				= src
tests_dir			= tests
tools_dir			= tools

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
src_data_files 		+= $(shell find $(data_dir)/*)
data_files 			= $(patsubst $(data_dir)/%,%,$(src_data_files))
log_file 			= $(log_dir)/qemu.log
output_image 		= $(output_dir)/floppy.img
apps_dirs 			= $(shell find $(apps_src_dir)/ -maxdepth 1 -mindepth 1 -type d )

CFLAGS_DEBUG		+= -std=$(C_STANDARD) -O0 -ggdb3 -DDEBUG_MODE
# -fsanitize=undefined
CFLAGS_RELEASE		+= -std=$(C_STANDARD) -$(GCC_OPTIMIZATION) 

# include bootloader-fat12
include $(bootloader_src_dir)/fat12/Makefile.include

# include libk
include $(libk_src_dir)/Makefile.include

# include kernel
include $(kernel_src_dir)/Makefile.include

# include libc
include $(libc_src_dir)/Makefile.include

ifeq ($(IMAGE_FORMAT),FLOPPY)
include ./Makefile.floppy
endif

$(include_dir)/%.h: $(src_dir)/%.h
	$(progress) "LN" $@
	$(MKDIR) -p $(dir $@)
	ln -sf $(CWD)/$< $@

$(log_dir):
	$(progress) "MKDIR" $@
	$(MKDIR) -p $@

# copy files from data directory to 
$(data_files): $(output_image)
	$(progress) "INSTALL" $@ 
	$(info $$var is $@)
	$(MCOPY) -oi $< $(data_dir)/$@ ::$@

$(apps_dirs): $(libc_bin)
	$(MAKE) install -C $@ VERBOSE=$(VERBOSE) app_src_dir=$@ output_image=$(output_image)  build_dir=$(build_dir) cross_dir=$(cross_dir)

build: ## build
build: $(output_image) $(header_files) bootloader kernel $(data_files) $(apps_dirs)
.PHONY: build

run-qemu: ## run qemu
run-qemu: $(log_dir) build
	$(progress) QEMU
	$(QEMU) $(QEMU_OPTIONS) \
	-drive file=$(output_image),format=raw,if=floppy
.PHONY: run-qemu

run-debug: ## run qemu with debugger
run-debug: $(log_dir) build
	$(QEMU) $(QEMU_OPTIONS) \
	-drive file=$(output_image),format=raw,if=floppy \
	-s -S
.PHONY: run-debug

vscode: ## install vscode configuration
vscode: bootloader kernel $(apps_dirs)
	$(progress) "MKDIR" ".vscode"
	$(MKDIR) -p .vscode
	
	$(progress) "SED" "launch.json"
	$(CP) -f ".vscode.launch" ".vscode/launch.json"
	$(SED) -i "s!\[PROGRAM]!$(kernel_bin)!g; \
			   s!\[GDB]!$(GDB)!g; \
			   s!\[DEBUG_TARGETS]!$(shell find $(dist_dir) -iname '*.elf' -printf '\"%p\",')!g; \
			   s!\[DEFUALT_DEBUG_TARGET]!\"$(kernel_elf)\"!g;" .vscode/launch.json

	$(progress) "SED" "tasks.json"
	$(CP) -f .vscode.tasks .vscode/tasks.json
	$(SED) -i "s!\[ENV_VARS]!CROSS=$(CROSS) BUILD_DIR=$(BUILD_DIR) LOG_DIR=$(LOG_DIR) VERBOSE=$(VERBOSE)!g" .vscode/tasks.json
.PHONY: vscode

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
	$(progress) "CLEAN" $(include_dir)
	rm -rf $(include_dir)
.PHONY: clean-include

clean-apps: ## clean userspace apps
clean-apps: 
	$(progress) "CLEAN" APPS
	rm -rf $(build_dir)/obj/app
.PHONY: clean-apps

clean: ## clean project
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
