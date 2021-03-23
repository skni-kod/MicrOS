THREADS_COUNT := 1
WORKSPACE_DIR := ${CURDIR}

.PHONY: all
all:
	# Delete old ELF and BIN files
	find build -iname "*.elf" -type f -delete
	find build -iname "*.bin" -type f -delete

	find environment -iname "*.elf" -type f -delete
	find library -iname "*.elf" -type f -delete
	find os -iname "*.elf" -type f -delete

	$(MAKE) -C os/bootloader

	$(MAKE) -C library

	for d in $(sort $(dir $(wildcard ./environment/*/))) ; do \
  		$(MAKE) -C $${d}; \
  	done

	$(MAKE) -C os/kernel


.PHONY: clean
clean:
	# Delete old ELF and BIN files
	find build -iname "*.elf" -type f -delete
	find build -iname "*.bin" -type f -delete

	find environment -iname "*.elf" -type f -delete
	find library -iname "*.elf" -type f -delete
	find os -iname "*.elf" -type f -delete

	$(MAKE) -C os/bootloader clean

	$(MAKE) -C library clean

	for d in $(sort $(dir $(wildcard ./environment/*/))) ; do \
      	$(MAKE) -C $${d} clean; \
    done

	$(MAKE) -C os/kernel clean