#include "sequencer.h"
#include "pragma.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"



#define baud 115200
int freq = 40000000;

void main() {
    //__builtin_enable_interrupts();
    macro_enable_interrupts();
    UART_Init(baud);
    UART_InitPoll(baud);
    sequencer_init(freq);
    
    sequencer_wake();
    //sequencer_walkForward();
    sequencer_walkBack();
    while(1);
}