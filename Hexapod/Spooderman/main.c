#include "sequencer.h"
#include "pragma.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "ssc32.h"

//head
#define shortWait 1000
#define scanTime 5000


enum WState{fwd,bwk,lft,rgt,stp};

void fullScan();
void neutralScan();

//srf
char waitdone = 0;
char comm1[] = {0x00, 0x51};
char comm2[] = {0x02};
char a;
short int dst_arr;

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
    sequencer_walkForward();
    DelayAprox10Us(20000);
    sequencer_stop();
    DelayAprox10Us(10000);
     
    int freq = 1;
    srf_init(int freq);
    srf_startRanging();
    srf_getDistance(&dst_arr);
            
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

void neutralScan() {
    //move to middle center
    sequencer_moveHeadHorizontal(0);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(shortWait);
}


void __ISR(_TIMER_3_VECTOR, ipl7auto) Timer3ISR(void) {
    waitdone = 1;
    IFS0bits.T3IF = 0;       // clear interrupt flag
}

void srf_init(int freq) {
     I2C_Init(freq);
     timer_init(40000000,3,270,1,7);
     T3CONbits.ON = 0;
     TMR3 = 0;
}

unsigned char srf_startRanging() {
    a = I2C_Write(0x70,comm1,2,1);
    T3CONbits.ON = 1;
    return 1;
}
 
unsigned char srf_getDistance(short int *distance)  {
    if(waitdone)
    {
       //_CP0_SET_COUNT(0);
       //while(_CP0_GET_COUNT()<20000000);
       a= I2C_Write(0x70,comm2,1,1);
       char viktor[2];
       a= I2C_Read(0x70,viktor,2);
       short int viktorInt = (viktor[0]<<8) + viktor[1];
       *distance = viktorInt;
       waitdone = 0;
       T3CONbits.ON = 0;
       return 1;
    }
    else return 0;
}