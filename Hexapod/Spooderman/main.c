#include "sequencer.h"
#include "pragma.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "ssc32.h"

enum WState{fwd,bwk,lft,rgt,stp};

void main() {
    //__builtin_enable_interrupts();
    
    enum WState WalkState;
    WalkState = stp;
    macro_enable_interrupts();
    UART_Init(UART_BAUDRATE);
    //UART_InitPoll(UART_BAUDRATE);
    sequencer_init(PB_FRQ);
    DelayAprox10Us(1000); //Wait 100ms for initialization
    sequencer_wake();
    //DelayAprox10Us(1000); //Wait 100ms for Spooder to stand up
    //DelayAprox10Us(10000);
    //sequencer_sleep();
    sequencer_walkForward();
    DelayAprox10Us(20000);
    sequencer_stop();
    
    
    while(1)    //While only when connected to BT, to be implemented (Watchdog?)
    {
        //HERE: command decoder. Split walk commands from scan commands?

//        switch(WalkState)
//        {
//        case stp:
//            //sequencer_wake();
//        case fwd:
//            sequencer_walkForward();
//        case bwk:
//            sequencer_walkBack();
//        case lft:
//            sequencer_walkLeft();
//        case rgt:
//            sequencer_walkRight();
//        default:
//            sequencer_stop();
//        }
        
        //HERE: Scan switch case?
    }
}

//angles between -45 and 45
void moveHead() {
    
    //move to top left
    sequencer_moveHeadHorizontal(-45);
    DelayAprox10Us(1000);
    sequencer_moveHeadVertical(-45);
    DelayAprox10Us(2000);
    
    //move to middle left
    sequencer_moveHeadHorizontal(0);
    DelayAprox10Us(500);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(2000);
    
    //move to bottom left
    sequencer_moveHeadHorizontal(45);
    DelayAprox10Us(500);
    sequencer_moveHeadVertical(45);
    DelayAprox10Us(2000);
    
}