#include "sequencer.h"
#include "pragma.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "ssc32.h"

#define shortWait 1000
#define scanTime 5000

enum WState{fwd,bwk,lft,rgt,stp};

void fullScan();
void neutralScan();

void main() {
    //__builtin_enable_interrupts();
    TRISACLR = 0xF;
    LATA = 0x00;
    
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
    DelayAprox10Us(10000);
    //sequencer_sleep();
    //DelayAprox10Us(5000);    //calling sleep caused some odd things in the legs while scanning
     
    
    int var = 1;
    fullScan(var);
    DelayAprox10Us(10000);
        
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

void fullScan(int var) {
   
    if(var == 1) {    
    //move to top left
    sequencer_moveHeadHorizontal(-45);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(-45);
    DelayAprox10Us(scanTime);
    
    //move to middle left
    sequencer_moveHeadHorizontal(-45);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(scanTime);
    
    //move to bottom left
    sequencer_moveHeadHorizontal(-45);
    sequencer_moveHeadVertical(45);
    DelayAprox10Us(scanTime);
   
    //move to bottom center
    sequencer_moveHeadHorizontal(0);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(45);
    DelayAprox10Us(scanTime);
    
    //move to middle center
    sequencer_moveHeadHorizontal(0);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(scanTime);
    
    //move to top center
    sequencer_moveHeadHorizontal(0);
    sequencer_moveHeadVertical(-45);
    DelayAprox10Us(scanTime);
    
    //move to top right
    sequencer_moveHeadHorizontal(45);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(-45);
    DelayAprox10Us(scanTime);
    
    //move to middle right
    sequencer_moveHeadHorizontal(45);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(scanTime);
    
    //move to bottom right
    sequencer_moveHeadHorizontal(45);
    sequencer_moveHeadVertical(45);
    DelayAprox10Us(scanTime);
    }
    
}

void neutralScan() {
    //move to middle center
    sequencer_moveHeadHorizontal(0);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(shortWait);
}
