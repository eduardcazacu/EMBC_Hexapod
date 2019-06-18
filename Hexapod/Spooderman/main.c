#include "sequencer.h"
#include "pragma.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

enum WState{fwd,bwk,lft,rgt,stp};


#define baud 115200
int freq = 40000000;

void main() {
    //__builtin_enable_interrupts();
    
    enum WState WalkState;
    WalkState = stp;
    macro_enable_interrupts();
    UART_Init(baud);
    UART_InitPoll(baud);
    sequencer_init(freq);
    DelayAprox10Us(1000); //Wait 100ms for initialization
    sequencer_wake();
    DelayAprox10Us(1000); //Wait 100ms for Spooder to stand up
    
    
    
    
    while(1)    //While only when connected to BT, to be implemented (Watchdog?)
    {
        //HERE: command decoder. Split walk commands from scan commands?
        switch(WalkState)
        {
        case stp:
            sequencer_stop();
        case fwd:
            sequencer_walkForward();
        case bwk:
            sequencer_walkBack();
        case lft:
            sequencer_walkLeft();
        case rgt:
            sequencer_walkRight();
        default:
            sequencer_stop();
        }
        
        //HERE: Scan switch case?
    }
}