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

//custom scan functions
void fullScan();
void neutralScan();

//srf functions
unsigned char srf_startRanging();
unsigned char srf_getDistance(short int *distance);
void srf_init(int freq);

//srf variables
char waitdone = 0;
char comm1[] = {0x00, 0x51};
char comm2[] = {0x02};
char a;
short int dst_arr;
short int distance[15] = {};

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
    sequencer_wake();
    DelayAprox10Us(10000);
     
    //int freq = 400000;
    srf_init(400000);
    
    fullScan();
    DelayAprox10Us(10000);
    
    neutralScan();
            
    while(1)    //While only when connected to BT, to be implemented (Watchdog?)
    {     
       neutralScan();
       if (distance[8] > 50 ) {
           sequencer_walkBack();
           DelayAprox10Us(10000);
           sequencer_stop();
           
       }
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


void fullScan() {
   
    //move to top left
    sequencer_moveHeadHorizontal(-45);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(-45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[0] = dst_arr;
    }
    else {
        distance[0] = 0;
    }
    
    
    //square 2
    sequencer_moveHeadHorizontal(-45);
    sequencer_moveHeadVertical(0);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[1] = dst_arr;
    }
    else {
        distance[1] = 0;
    }
    
    //square 3
    sequencer_moveHeadHorizontal(-45);
    sequencer_moveHeadVertical(45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[2] = dst_arr;
    }
    else {
        distance[2] = 0;
    }
   
    //square 4
    sequencer_moveHeadHorizontal(-23);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[3] = dst_arr;
    }
    else {
        distance[3] = 0;
    }
    
    //square 5
    sequencer_moveHeadHorizontal(-23);
    sequencer_moveHeadVertical(0);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[4] = dst_arr;
    }
    else {
        distance[4] = 0;
    }
    
    //square 6
    sequencer_moveHeadHorizontal(-23);
    sequencer_moveHeadVertical(-45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[5] = dst_arr;
    }
    else {
        distance[5] = 0;
    }
    
    //square 7
    sequencer_moveHeadHorizontal(0);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(-45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[6] = dst_arr;
    }
    else {
        distance[6] = 0;
    }
    
    //square 8
    sequencer_moveHeadHorizontal(0);
    sequencer_moveHeadVertical(0);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[7] = dst_arr;
    }
    else {
        distance[7] = 0;
    }
    
    //square 9
    sequencer_moveHeadHorizontal(0);
    sequencer_moveHeadVertical(45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[8] = dst_arr;
    }
    else {
        distance[8] = 0;
    }
    
    //square 10
    sequencer_moveHeadHorizontal(23);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[9] = dst_arr;
    }
    else {
        distance[9] = 0;
    }
    
    //square 11
    sequencer_moveHeadHorizontal(23);
    sequencer_moveHeadVertical(0);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[10] = dst_arr;
    }
    else {
        distance[10] = 0;
    }
    
    //square 12
    sequencer_moveHeadHorizontal(23);
    sequencer_moveHeadVertical(-45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[11] = dst_arr;
    }
    else {
        distance[11] = 0;
    }
    
    //square 13
    sequencer_moveHeadHorizontal(-45);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(-45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[12] = dst_arr;
    }
    else {
        distance[12] = 0;
    }
    
    //square 14
    sequencer_moveHeadHorizontal(0);
    sequencer_moveHeadVertical(0);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[13] = dst_arr;
    }
    else {
        distance[13] = 0;
    }
    
    //move to bottom right
    sequencer_moveHeadHorizontal(45);
    sequencer_moveHeadVertical(45);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[14] = dst_arr;
    }
    else {
        distance[14] = 0;
    }
}

void neutralScan() {
    //move to middle center
    int i;
    for(i =0; i<15; i++) {
        distance[i] = 0;
    }
    sequencer_moveHeadHorizontal(0);
    DelayAprox10Us(shortWait);
    sequencer_moveHeadVertical(0);
    DelayAprox10Us(shortWait);
    srf_startRanging();
    DelayAprox10Us(scanTime);
    if(srf_getDistance(&dst_arr)) {
        distance[8] = dst_arr;
    }
    else {
        distance[8] = 0;
    }
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