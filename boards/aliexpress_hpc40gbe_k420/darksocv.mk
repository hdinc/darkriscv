# The general concept is based in the article:
# 
# 	https://www.fpgarelated.com/showarticle/786.php
#
# However, since the ISE GUI itself creates a "darksocv.cmd_log" file with
# all commands executed and the respective options, it is possible change
# some options in the ISE and check the file in order to understand how
# enable/disable the different options.
# 

# board Aliexpress HPC 40GbE K420
BOARD  = aliexpress_hpc40gbe_k420
DEVICE = xc7k420t-ffg901-2

ISE = ../boards/$(BOARD)
RTL = ../rtl
SRC = ../src
TMP = ../tmp

XST = $(ISE)/darksocv.xst
SYR = $(TMP)/darksocv.syr
UCF = $(ISE)/darksocv.ucf
IMP = $(ISE)/darksocv.imp
NGC = $(TMP)/darksocv.ngc
NGD = $(TMP)/darksocv.ngd
PCF = $(TMP)/darksocv.pcf
NCD = $(TMP)/darksocv.ncd
TWX = $(TMP)/darksocv.twx
TWR = $(TMP)/darksocv.twr
BIT = $(TMP)/darksocv.bit
MAP = $(TMP)/darksocv_map.ncd
UT  = $(ISE)/darksocv.ut

PRJS = $(ISE)/darksocv.prj
RTLS = $(RTL)/darksocv.v $(RTL)/darkriscv.v $(RTL)/darkuart.v $(RTL)/config.vh

ifdef HARVARD
	BOOT = $(SRC)/darksocv.rom.mem $(SRC)/darksocv.ram.mem
else
	BOOT = $(SRC)/darksocv.mem
endif

IMP  = $(ISE)/darksocv.imp

default: all

$(NGC): $(PRJS) $(BOOT) $(RTLS)
	cd $(TMP) && xst -intstyle ise -ifn $(XST) -ofn $(SYR)

$(NGD): $(NGC) $(UCF) $(BOOT) $(RTLS)
	cd $(TMP) && ngdbuild -intstyle ise -dd _ngo -nt timestamp -uc $(UCF) -p $(DEVICE) $(NGC) $(NGD)

$(PCF): $(NGD) $(BOOT) $(UCF) $(RTLS)
	cd $(TMP) && map -intstyle ise -p $(DEVICE) -w -logic_opt on  -ol high -t 1 -xt 0 -register_duplication on  -r 4 -global_opt off -mt 2 -detail -ir off -ignore_keep_hierarchy -pr off -lc auto -power off -o $(MAP) $(NGD) $(PCF)

$(NCD): $(PCF) $(BOOT) $(UCF) $(RTLS)
	cd $(TMP) && par -w -intstyle ise -ol high -mt 2 $(MAP) $(NCD) $(PCF)
	cd $(TMP) && trce -intstyle ise -v 3 -s 2 -n 3 -fastpaths -xml $(TWX) $(NCD) -o $(TWR) $(PCF)

$(BIT): $(UT) $(NCD) $(BOOT) $(UCF) $(RTLS)
	cd $(TMP) && bitgen -intstyle ise -f $(UT) $(NCD)

all: $(BIT) $(BOOT) $(UCF) $(RTLS)

install: $(BIT) $(BOOT) $(UCF) $(RTLS)
	cd $(TMP) && impact -batch $(IMP)

clean:
	-rm -v $(TMP)/*
