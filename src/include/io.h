#ifndef __IO__
#define __IO__

extern volatile int threads; // number of threads in the core
extern volatile int utimers; // microsecond timer

struct DARKIO {

    unsigned char board_id; // 00
    unsigned char board_cm; // 01
    unsigned char board_ck; // 02
    unsigned char irq;      // 03

    struct DARKUART {

        unsigned char  stat; // 04
        unsigned char  fifo; // 05
        unsigned short baud; // 06/07

    } uart;

    unsigned short led;     // 08/09
    unsigned short gpio;    // 0a/0b

    unsigned int timer;     // 0c
};

extern volatile struct DARKIO io;

extern char *board_name(int);

#ifdef __RISCV__
#define kmem 0
#else
extern unsigned char kmem[8192];
#endif

#define IRQ_TIMR 0x80
#define IRQ_UART 0x02

#endif
