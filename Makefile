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
	build_dir		= $(CWD)/build
else
	build_dir		= ${BUILD_DIR}
endif
data_dir			= $(CWD)/data
external_dir		= $(CWD)/external
ifndef LOG_DIR
	log_dir			= $(CWD)/log
else
	log_dir			= ${LOG_DIR}
endif
src_dir				= $(CWD)/src
tests_dir			= $(CWD)/tests
tools_dir			= $(CWD)/tools

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
#header_files 		= $(addprefix $(src_dir)/, $(patsubst $(src_dir)/%,%,$(src_header_files)))
header_files 		= $(src_header_files)
src_data_files 		+= $(shell find $(data_dir)/ -mindepth 1 -type f)
data_files 			= $(patsubst $(data_dir)/%,%,$(src_data_files))
apps_dirs 			= $(shell find $(apps_src_dir)/ -maxdepth 1 -mindepth 1 -type d )
log_file 			= $(log_dir)/qemu.log
output_image_floppy	= $(build_dir)/floppy.img

CFLAGS_DEBUG		+= -std=$(C_STANDARD) -O0 -ggdb3 -DDEBUG_MODE
# -fsanitize=undefined
CFLAGS_RELEASE		+= -std=$(C_STANDARD) -$(GCC_OPTIMIZATION) 

# include bootloader-fat12
include $(bootloader_src_dir)/fat12/Makefile.include

# include bootloader-pxe
include $(bootloader_src_dir)/pxe/Makefile.include

# include libk
include $(libk_src_dir)/Makefile.include

# include kernel
include $(kernel_src_dir)/Makefile.include

# include libc
include $(libc_src_dir)/Makefile.include

# output image format:
ifeq ($(IMAGE_FORMAT),FLOPPY)
include ./Makefile.floppy
endif

ifeq ($(QEMU_NET), 1)
	ifeq ($(QEMU_NET_LOG),1)
		QEMU_OPTIONS		+= -object filter-dump,id=f1,netdev=net1,file=$(log_dir)/traffic.pcap
	endif

	ifneq ($(QEMU_TAP),1)
		QEMU_OPTIONS		+= -netdev user,id=net1,ipv6=off,dhcpstart=10.0.2.20,hostfwd=tcp::5555-:22
	else
		QEMU_OPTIONS		+= -netdev tap,id=net1,ifname=tap0,script=no,downscript=no
	endif

	QEMU_OPTIONS			+= -net nic,model=$(QEMU_NET_DEVICE),netdev=net1,macaddr=00:11:22:33:44:55
endif

$(log_dir):
	$(progress) "MKDIR" $@
	$(MKDIR) -p $@

# copy files from data directory to 
$(data_files):
	$(progress) "CP" $@ 
	$(MKDIR) -p $(output_dir)/$(dir $@)
	$(CP) -f $(data_dir)/$@ $(output_dir)/$@

$(apps_dirs): $(libc_bin)
	$(MAKE) install -C $@ VERBOSE=$(VERBOSE) app_src_dir=$@ build_dir=$(build_dir) cross_dir=$(cross_dir)

build: ## build
build: kernel $(data_files) $(apps_dirs) 
.PHONY: build

floppy-image: ## generate floppy image
floppy-image: $(output_image_floppy) bootloader-fat12
.PHONY: floppy-image

run-qemu: ## run qemu
run-qemu: $(log_dir) build
	$(progress) QEMU
	$(QEMU) $(QEMU_OPTIONS) \
	-drive file=$(output_image_floppy),format=raw,if=floppy
.PHONY: run-qemu

run-debug: ## run qemu with debugger
run-debug: $(log_dir) build
	$(QEMU) $(QEMU_OPTIONS) \
	-drive file=$(output_image_floppy),format=raw,if=floppy \
	-s -S
.PHONY: run-debug

run-pxe: ## run pxe
run-pxe:
	make bootloaderpxe && G_MESSAGES_DEBUG=all qemu-system-i386 -netdev user,id=net0,tftp=/mnt/ramdisk/build,bootfile=/pxe.0 -boot n -net nic,model=rtl8139,netdev=net0,macaddr=00:11:22:33:44:55
.PHONY: run-pxe

run-pxe-debug: ## run pxe-debug
run-pxe-debug:
	make bootloaderpxe && make build && G_MESSAGES_DEBUG=all qemu-system-i386 -s -S \
	-boot n \
	-netdev user,id=net0,hostfwd=tcp::5555-:22,tftp=/mnt/ramdisk/build,bootfile=/pxe.0 \
	-device rtl8139,netdev=net0,bootindex=1,romfile="$(CWD)/rtl8139.rom"
.PHONY: run-pxe-debug


vscode: ## install vscode configuration
vscode: kernel $(apps_dirs)
	$(progress) "MKDIR" ".vscode"
	$(MKDIR) -p .vscode
	
	$(progress) "SED" "launch.json"
	$(CP) -f ".vscode.launch" ".vscode/launch.json"
	$(SED) -i "s!\[PROGRAM]!$(kernel_bin)!g; \
			   s!\[GDB]!$(GDB)!g; \
			   s!\[DEBUG_TARGETS]!$(shell find $(dist_dir) -iname '*.elf' -printf '\\"%p\\",')!g; \
			   s!\[DEFUALT_DEBUG_TARGET]!\"$(kernel_elf)\"!g;" .vscode/launch.json

	$(progress) "SED" "tasks.json"
	$(CP) -f .vscode.tasks .vscode/tasks.json
	$(SED) -i "s!\[ENV_VARS]!CROSS=$(cross_dir) BUILD_DIR=$(build_dir) LOG_DIR=$(log_dir) VERBOSE=$(VERBOSE)!g" .vscode/tasks.json
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
	$(progress) "CLEAN" $(build_dir)/obj/app
	rm -rf $(build_dir)/obj/app
.PHONY: clean-apps

clean-image: ## clean image
clean-image: 
	$(progress) "CLEAN" $(output_image_floppy)
	rm -rf $(output_image_floppy)
.PHONY: clean-image


clean: ## clean project
clean: 
	$(progress) "CLEAN" $(build_dir)
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
