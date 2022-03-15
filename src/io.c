#include <io.h>

#ifdef __RISCV__

volatile struct DARKIO io;

#else

volatile struct DARKIO io =
{
    4, 100, 0, 0,   // ctrl = { board id, fMHz, fkHz }
    { 0, 0, 0 },    // uart = { stat, fifo, baud }
    0,              // led
    0,              // gpio
    1000000         // timer
};

unsigned char kmem[8192] = "darksocv x86 payload test";

#endif

volatile int threads = 0; // number of threads
volatile int utimers = 0; // number of microseconds

// board database

char *board_name(int id)
{
    return id==0  ? "simulation only" :
           id==1  ? "avnet microboard lx9":
           id==2  ? "xilinx ac701 a200" :
           id==3  ? "qmtech sdram lx16" :
           id==4  ? "qmtech spartan7 s15" :
           id==5  ? "lattice brevia2 lxp2" :
           id==6  ? "piswords rs485 lx9" :
           id==7  ? "digilent spartan3 s200" :
           id==8  ? "aliexpress hpc/40gbe k420" :
           id==9  ? "qmtech artix7 a35" :
           id==10 ? "aliexpress hpc/40gbe ku040" :
           id==11 ? "papilio duo logicstart" :
           id==12 ? "qmtech kintex-7 k325" :
                    "unknown";
}
