ifndef ICARUS
    ICARUS = iverilog
endif

RTLS = darksimv.v ../rtl/darksocv.v ../rtl/darkuart.v ../rtl/darkriscv.v
INCS = ../rtl/config.vh
XSIM = darksocv
VCDS = darksocv.vcd

.PRECIOUS: $(VCDS)	# Do not delete the trace file if simulation is aborted
.PHONY: $(VCDS)		# Always run the simulation

ifdef HARVARD
	SRCS = ../src/darksocv.rom.mem ../src/darksocv.ram.mem
else
	SRCS = ../src/darksocv.mem
endif

DEPS = $(SRCS) $(RTLS) $(INCS)

all: $(VCDS)
	@echo simulation ok.

clean:
	-rm $(VCDS) $(XSIM)

$(VCDS): $(XSIM)
	./$(XSIM)


$(XSIM): $(DEPS)
	$(ICARUS) -I ../rtl -o $(XSIM) $(RTLS)
