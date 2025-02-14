#include <io.h>
#include <stdio.h>

void irq_handler(void) __attribute__ ((interrupt ("machine")));

void irq_handler(void)
{
    if(io.irq == IRQ_TIMR)
    {
        if(!utimers--)
        {
            io.led++;
            utimers=999999;
        }
        io.irq = IRQ_TIMR;
    }

    return;
}

volatile int heapcheck = 0xdeadbeef;

int main(void)
{
    printf("board: %s (id=%d)\n",board_name(io.board_id),io.board_id);
    printf("build: %s for %s\n",BUILD,ARCH);

    for(int i=0;i!=threads;i++)
    {
      printf("core0/thread%d: darkriscv@%d.%dMHz rv32%s%s%s\n",
          i,
          io.board_cm,                        // board clock MHz
          io.board_ck,                        // board clock kHz
          check4rv32i()?"i":"e",              // architecture
          threads>1?"+MT":"",                 // MT  support
          mac(1000,16,16)==1256?"+MAC":"");   // MAC support
    }

    threads = 0; // prepare for the next restart


    printf("uart0: 115200 bps (div=%d)\n",io.uart.baud);
    printf("timr0: frequency=%dHz (io.timer=%d)\n",(io.board_cm*1000000u+io.board_ck*10000u)/(io.timer+1),io.timer);

    set_mtvec(irq_handler);

    unsigned mtvec = get_mtvec();

    if(mtvec)
    {
        printf("mtvec: handler@%d, enabling interrupts...\n",mtvec);
        set_mie(1);
        printf("mtvec: interrupts enabled!\n");
    }
    else
        printf("mtvec: not found (polling only)\n");

    io.irq = IRQ_TIMR; // clear interrupts

    printf("\n");

    printf("Welcome to DarkRISCV!\n");

    // main loop

    while(1)
    {
        char  buffer[64];

        printf("> ");
        memset(buffer,0,sizeof(buffer));

        if(mtvec==0)
        {
            while(1)
            {
                if(io.irq&IRQ_TIMR)
                {
                    if(!utimers--)
                    {
                        io.led++;
                        utimers=999999;
                    }
                    io.irq = IRQ_TIMR;
                }

                if(io.uart.stat&2)
                {
                    break;
                }
            }
        }

        gets(buffer,sizeof(buffer));

        char *argv[8];
        int   argc;

        for(argc=0;argc<8 && (argv[argc]=strtok(argc==0?buffer:NULL," "));argc++)
            //printf("argv[%d] = [%s]\n",argc,argv[argc]);
            ;

        if(argv[0])
        {
          if(!strcmp(argv[0],"clear"))
          {
              printf("\33[H\33[2J");
          }
          else
          if(!strcmp(argv[0],"reboot"))
          {
              int i;

              printf("core0: reboot in 3 seconds");

              for(i=0;i!=3;i++)
              {
                  usleep(1000000);
                  putchar('.');
              }

              printf("done.\n");

              return 0;
          }
          else
          if(!strcmp(argv[0],"dump"))
          {
              char *p=(char *)(kmem+(argv[1]?xtoi(argv[1]):0));

              int i,j;

              for(i=0;i!=16;i++)
              {
                  printf("%x: ",(unsigned) p);

                  for(j=0;j!=16;j++) printf("%x ",p[j]);
                  for(j=0;j!=16;j++) putchar((p[j]>=32&&p[j]<127)?p[j]:'.');

                  putchar('\n');
                  p+=16;
              }
          }
          else
          if(0) // if(!strncmp(argv[0],"rd",2)||!strncmp(argv[0],"wr",2))
          {
              int kp = 2,
                  i = 1,j,k,w,
                  vp = 1;

              if(argv[0][kp]=='m')
              {
                  i=xtoi(argv[vp++]);
                  kp++;
              }

              printf("%x: ",k=xtoi(argv[vp++]));

              for(j=0;i--;j++)
              {
                  if(argv[0][0]=='r')
                  {
                      if(argv[0][kp]=='b') printf("%x ",j[(char  *)k]);
                      if(argv[0][kp]=='w') printf("%x ",j[(short *)k]);
                      if(argv[0][kp]=='l') printf("%x ",j[(int   *)k]);
                  }
                  else
                  {
                      w = xtoi(argv[vp++]);
                      if(argv[0][kp]=='b') printf("%x ",j[(char  *)k]=w);
                      if(argv[0][kp]=='w') printf("%x ",j[(short *)k]=w);
                      if(argv[0][kp]=='l') printf("%x ",j[(int   *)k]=w);
                  }
              }
              printf("\n");
          }
          else
          if(!strcmp(argv[0],"led"))
          {
              if(argv[1]) io.led = xtoi(argv[1]);

              printf("led = %x\n",io.led);
          }
          else
          if(!strcmp(argv[0],"timer"))
          {
              if(argv[1]) io.timer = atoi(argv[1]);

              printf("timer = %d\n",io.timer);
          }
          else
          if(!strcmp(argv[0],"gpio"))
          {
              if(argv[1]) io.gpio = xtoi(argv[1]);

              printf("gpio = %x\n",io.gpio);
          }
          else
          if(!strcmp(argv[0],"mul"))
          {
              int x = atoi(argv[1]);
              int y = atoi(argv[2]);

              printf("mul = %d\n",x*y);
          }
          else
          if(!strcmp(argv[0],"div"))
          {
              int x = atoi(argv[1]);
              int y = atoi(argv[2]);

              printf("div = %d, mod = %d\n",x/y,x%y);
          }
          else
          if(!strcmp(argv[0],"mac"))
          {
              int acc = atoi(argv[1]);
              int x = atoi(argv[2]);
              int y = atoi(argv[3]);

              printf("mac = %d\n",mac(acc,x,y));
          }
          else
          if(!strcmp(argv[0],"srai"))
          {
              int acc = xtoi(argv[1]);
              printf("srai %x >> 1 = %x\n",acc,acc>>1);
          }
          else
          if(argv[0][0])
          {
              printf("command: [%s] not found.\n"
                     "valid commands: clear, dump [hex], led [hex], timer [dec], gpio [hex]\n"
                     "                mul [dec] [dec], div [dec] [dec], mac [dec] [dec] [dec]\n"
                     "                reboot, wr[m][bwl] [hex] [hex] [[hex] when m],\n"
                     "                rd[m][bwl] [hex] [[hex] when m]\n",
                     argv[0]);
          }

          if(heapcheck!=0xdeadbeef)
          {
              printf("out of memory detected, a reboot is recommended...\n");
          }
       }
    }
}
