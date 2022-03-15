# board LatticeXP2 Brevia 2
BOARD  = lattice_brevia2_xp2
DEVICE = LFXP2-5E-6TN144C
DIAMOND_PATH=/usr/local/diamond/3.11_x64
IMPL = impl1
TMP = ../tmp


# Expected by Lattice Diamond
export TEMP=../tmp
export LSC_INI_PATH=""
export LSC_DIAMOND=true
export TCL_LIBRARY=$(DIAMOND_PATH)/tcltk/lib/tcl8.5
export FOUNDRY=$(DIAMOND_PATH)/ispFPGA
export PATH:=$(FOUNDRY)/bin/lin64:${PATH}


RTL = ../rtl
SRC = ../src
BIT = $(TMP)/darksocv.bit

RTLS = $(RTL)/darksocv.v $(RTL)/darkriscv.v $(RTL)/darkuart.v $(RTL)/config.vh

ifdef HARVARD
	BOOT = $(SRC)/darksocv.rom.mem $(SRC)/darksocv.ram.mem
else
	BOOT = $(SRC)/darksocv.mem
endif

default: build

$(BIT): $(BOOT) $(RTLS)
	echo PATH: $$PATH
	cd $(BOARD) && $(DIAMOND_PATH)/bin/lin64/diamondc darksocv.tcl 2>&1 | tee darksocv_build.log
	cp $(BOARD)/$(IMPL)/darksocv_impl1.jed $(BIT)

clean:
	-rm -v $(TMP)/*
	rm -rf $(BOARD)/$(IMPL)
