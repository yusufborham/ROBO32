#ifndef DELAY_H
#define DELAY_H

#define DELAY_MS(d) for(unsigned long i = 0 ; i < (d * 4000) ; i++ ){asm("NOP");}
#define DELAY_US(d) for(unsigned long i = 0 ; i < (d * 4) ; i++ ){asm("NOP");}
#endif
