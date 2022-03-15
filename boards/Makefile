#BOARD  = avnet_microboard_lx9
#BOARD  = xilinx_ac701_a200
#BOARD  = qmtech_sdram_lx16
#BOARD = lattice_brevia2_xp2
#BOARD = papilio_duo_logicstart

default: all

ifdef BOARD

include $(BOARD)/darksocv.mk

else

all:
	@echo no board selected to build, done.

clean:
	@echo no board selected to clean, done.

endif
