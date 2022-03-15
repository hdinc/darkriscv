SHELL := /bin/bash

ifndef HOST_CC
    HOST_CC = gcc
    HOST_CFLAGS = -Wall -Wno-incompatible-library-redeclaration -I./include -Os
endif

ifndef CROSS

    ARCH = rv32e
    #ARCH = rv32i

    #TODO: better way to switch little/big-endian:
    #ENDIAN = _le # gcc for little-endian
    #ENDIAN = _be # gcc for big-endian

    CROSS = riscv32-embedded$(ENDIAN)-elf
    #CROSS = riscv-elf
    #CROSS = riscv32-unknown-elf
    #CROSS = riscv32-embedded-elf

    CCPATH = /usr/local/share/gcc-$(CROSS)/bin/
    #CCPATH = /usr/local/share/toolchain-$(CROSS)/bin
    #HARVARD = 1
endif

ifndef BUILD
    BUILD = $(shell date -R)
endif

    CC = $(CCPATH)/$(CROSS)-gcc
    AS = $(CCPATH)/$(CROSS)-as
    LD = $(CCPATH)/$(CROSS)-ld
    OC = $(CCPATH)/$(CROSS)-objcopy
    OD = $(CCPATH)/$(CROSS)-objdump
   CPP = $(CCPATH)/$(CROSS)-cpp

CCFLAGS = -Wall -fcommon -ffreestanding -I./include -O2 -march=$(ARCH) -mabi=ilp32e -D__RISCV__ -DBUILD="\"$(BUILD)\"" -DARCH="\"$(ARCH)\""
ASFLAGS = -march=$(ARCH)
LDFLAGS = -T$(PROJ).ld -Map=$(PROJ).map -m elf32lriscv # -Ttext=0
CPFLAGS = -P

CCFLAGS += -mcmodel=medany -mexplicit-relocs # relocable clode

ifdef HARVARD
	CPFLAGS +=  -DHARVARD=1
endif

OCFLAGS = -O binary # --reverse-bytes=4 # workaround for darkriscv big-endian blockrams
ODFLAGS = -d

PROJ = darksocv
OBJS = main.o stdio.o io.o banner.o boot.o
ASMS = main.s stdio.s io.s banner.s
SRCS = main.c stdio.c io.c banner.c boot.s

#include app/Makefile.in

DEPS = $(SRCS) $(ASMS) $(OBJS) $(PROJ).ld $(PROJ).ld.src

ifdef HARVARD
	TARGETS = $(PROJ).rom.mem $(PROJ).ram.mem
else
	TARGETS = $(PROJ).mem
endif

.PHONY: all

all: $(TARGETS) $(DEPS)
	@echo sources ok.

clean:
	-rm $(ASMS) $(OBJS) $(PROJ).{bin,lst,map,ram,rom,x86,text,data,bin,ld,o,mem}

%.o: %.s Makefile
	$(AS) $(ASFLAGS) -c $< -o $@

%.s: %.c Makefile
	$(CC) $(CCFLAGS) -S $< -o $@

$(PROJ).ld: $(PROJ).ld.src
	$(CPP) $(CPFLAGS) $(PROJ).ld.src $(PROJ).ld

$(PROJ).o: $(OBJS) $(PROJ).ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@
	$(OD) $(ODFLAGS) $@ > $(PROJ).lst

$(PROJ).rom.mem: $(PROJ).o
	$(OC) $(OCFLAGS) $< $(PROJ).text --only-section .text*
	hexdump -ve '1/4 "%08x\n"' $(PROJ).text > $@
	#xxd -p -c 4 -g 4 $< > $@
	rm $(PROJ).text
	wc -l $@
	@echo rom ok.

$(PROJ).ram.mem: $(PROJ).o
	$(OC) $(OCFLAGS) $< $(PROJ).data --only-section .*data*
	hexdump -ve '1/4 "%08x\n"' $(PROJ).data > $@
	#xxd -p -c 4 -g 4 $< > $@
	rm $(PROJ).data
	wc -l $@
	@echo ram ok.

$(PROJ).mem: $(PROJ).o
	$(OC) $(OCFLAGS) $< $(PROJ).bin
	hexdump -ve '1/4 "%08x\n"' $(PROJ).bin > $@
	#xxd -p -c 4 -g 4 $< > $@
	rm $(PROJ).bin
	wc -l $@
	@echo mem ok.

$(PROJ).x86: $(SRCS)
	$(HOST_CC) $(HOST_CFLAGS) $(SRCS) -o $(PROJ).x86 -DBUILD="\"$(BUILD)\"" -DARCH="\"x86\""
	@echo x86 ok.
