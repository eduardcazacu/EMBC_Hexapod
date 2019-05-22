/* ************************************************************************** */
/** Sequencer

 * Authors:
 * Robertas Konarskis
 * Michal Mikolajczyk

  @Description
  This file is responsible for sequencing the servo.
 */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "sequencer.h"
#include "ssc32.h"

#define TMR_TIME 0.02
#if HEXAPOD_HW_VERSION == HEXAPOD_HW_A
	#define servoFrontOffset 20
#elif HEXAPOD_HW_VERSION == HEXAPOD_HW_B
	#define servoFrontOffset 40
#endif
#define servoBackOffset -40
#define servoheightOffset 20
#if HEXAPOD_HW_VERSION == HEXAPOD_HW_A
	#define servoUp 60
#elif HEXAPOD_HW_VERSION == HEXAPOD_HW_B
	#define servoUp 50
#endif
#define servoBottom 0
#if HEXAPOD_HW_VERSION == HEXAPOD_HW_A
	#define servoUpStep 30
	#define frontStep 40
#elif HEXAPOD_HW_VERSION == HEXAPOD_HW_B
	#define servoUpStep 20
	#define frontStep 15
#endif


#define servoTop 60
#define legLeanFrontOffset 0

#define SEQUENCE_TIMEOUT 2000000

const int groupBottom[] = { 0, 4, 8,  16, 20, 24 };
const int groupMiddle[] = { 1, 5, 9,  17, 21, 25 };
const int groupTop[]    = { 2, 6, 10, 18, 22, 26 };
const int lblfrmU[] = { 1, 9, 21 };
const int lmrbrfU[] = { 5, 17, 25 };
const int lblfrmF[] = { 0, 8, 20 };
const int lmrbrfF[] = { 4, 16, 24 };
const int s_channels[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 };
int sequence = 0;

char walkForward = 0;
char walkLeft = 0;
char walkRight = 0;
char walkBack = 0;
char wake = 0;

void sequencer_sleep(){
    if (wake) {
        sequencer_stop();
        wake = 0;
        
        sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
        sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);   
        
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < SEQUENCE_TIMEOUT) {}
        
        int i;
        for (i = 0; i < 20; i++) {
            sequencer_writeMultiple(groupMiddle, sizeof(groupMiddle), servoUp + i);
            
            _CP0_SET_COUNT(0);
            while(_CP0_GET_COUNT() < (SEQUENCE_TIMEOUT / 2)) {}
        }
    }
}
void sequencer_wake(){
    if (!wake) {
        wake = 1;

        sequencer_writeMultiple(groupMiddle, sizeof(groupMiddle), 80);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < SEQUENCE_TIMEOUT) {}
        
        sequencer_writeMultiple(groupBottom, sizeof(groupBottom), servoBottom + legLeanFrontOffset);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < SEQUENCE_TIMEOUT) {}
        
        sequencer_writeMultiple(groupTop, sizeof(groupTop), servoTop);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < SEQUENCE_TIMEOUT) {}
        
        sequencer_writeMultiple(groupMiddle, sizeof(groupMiddle), servoUp);
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < SEQUENCE_TIMEOUT) {}
    }
}

void sequencer_walkForward() {
    if (wake && !walkForward) {
        sequencer_stop();
        walkForward = 1;
    }
}

void sequencer_walkLeft() {
    if (wake && !walkLeft) {
        sequencer_stop();
        walkLeft = 1;
    }
}

void sequencer_walkRight() {
    if (wake && !walkRight) {
        sequencer_stop();
        walkRight = 1;
    }
}

void sequencer_walkBack() {
    if (wake && !walkBack) {
        sequencer_stop();
        walkBack = 1;
    }
}
    
void sequencer_stop(void){
    walkForward = 0;
    walkLeft = 0;
    walkRight = 0;
    walkBack = 0;
    sequence = 0;
}

void sequencer_init(int freq) {
#if HEXAPOD_HW_VERSION == HEXAPOD_HW_A
    ssc32_init(180, 1100, 1900);
#elif HEXAPOD_HW_VERSION == HEXAPOD_HW_B
    ssc32_init(180, 500, 2500);
#endif
    sequencer_stop();
    
    sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp );
    sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp );
    
    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT() < SEQUENCE_TIMEOUT) {}
    
    int i;
    for (i = 0; i < 20; i++) {
        sequencer_writeMultiple(groupMiddle, sizeof(groupMiddle), servoUp + i*2);
        
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT() < (SEQUENCE_TIMEOUT / 2)) {}
    }
    
    TRISA = 0x0200;
    
    PR5 = (int)(((float)(TMR_TIME * freq * 5) / 256) + 0.5);
    TMR5 = 0;
    T5CONbits.TCKPS = 7;
    T5CONbits.TGATE = 0;
    T5CONbits.TCS = 0;
    IPC5bits.T5IP = 7;
    T5CONbits.ON = 1;
    IPC5bits.T5IS = 3;
    IFS0bits.T5IF = 0;
    IEC0bits.T5IE = 1;
    
    macro_enable_interrupts();
}

void sequencer_moveHeadHorizontal(int angle) {
    if (angle > 45 || angle < -45) {
        return;
    }
    
    ssc32_write(13, -angle);
}
void sequencer_moveHeadVertical(int angle) {
    if (angle > 45 || angle < -45) {
        return;
    }
    
    ssc32_write(12, -angle);
}

void sequencer_write(int channel, int angle) {
    int angle1 = angle;
    
    if (channel == 8 || channel == 24) {
        angle = angle1 + servoBackOffset;
    }
    
    if (channel == 0 || channel == 16) {
        angle = angle1 + servoFrontOffset;
    }
       
    if (channel < 15) {
        ssc32_write(channel, angle);
    } else {
        ssc32_write(channel, -angle);
    }
}

void sequencer_writeMultiple(int s_channels[], int size, int angle) {
    int i;
    for (i = 0; i < size/sizeof(int); i++) {
        sequencer_write(s_channels[i], angle);
    }
}

void sequencer_writeAll(int angle) {
    sequencer_writeMultiple(s_channels, sizeof(s_channels), angle);
}

void __ISR(_TIMER_5_VECTOR, ipl7auto) Timer5ISR(void) {
    if (wake) {
        if (walkForward) {
            switch (sequence) {
                case 0: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);
                        
                case 1: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp + servoUpStep);
                        break;
                        
                case 2: sequencer_write(0, legLeanFrontOffset + frontStep);sequencer_write(8, legLeanFrontOffset + frontStep); sequencer_write(20, legLeanFrontOffset + frontStep);
                        sequencer_write(4, legLeanFrontOffset - frontStep);sequencer_write(16, legLeanFrontOffset - frontStep); sequencer_write(24, legLeanFrontOffset - frontStep);
                        break;
                        
                case 3: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        break;
                        
                case 4: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp + servoUpStep);
                        break;
                
                case 5: sequencer_write(4, legLeanFrontOffset + frontStep); sequencer_write(16, legLeanFrontOffset + frontStep); sequencer_write(24, legLeanFrontOffset + frontStep);
                        sequencer_write(0, legLeanFrontOffset - frontStep); sequencer_write(8, legLeanFrontOffset - frontStep);sequencer_write(20, legLeanFrontOffset - frontStep);
                        break;
                        
                case 6: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);  sequence = 0;
                        break;

                default: break; 
            }

            sequence++;
            LATA = sequence;
        }
        
        if (walkBack) {
            switch (sequence) {
                case 0: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);
                        
                case 1: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp + servoUpStep);
                        break;
                
                case 2: sequencer_write(0, legLeanFrontOffset - frontStep);sequencer_write(8, legLeanFrontOffset - frontStep); sequencer_write(20, legLeanFrontOffset - frontStep);
                        sequencer_write(4, legLeanFrontOffset + frontStep);sequencer_write(16, legLeanFrontOffset + frontStep); sequencer_write(24, legLeanFrontOffset + frontStep);
                        break;
                        
                case 3: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        break;
                
                case 4: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp + servoUpStep);
                        break;
                
                case 5: sequencer_write(4, legLeanFrontOffset - frontStep); sequencer_write(16, legLeanFrontOffset - frontStep); sequencer_write(24, legLeanFrontOffset - frontStep);
                        sequencer_write(0, legLeanFrontOffset + frontStep); sequencer_write(8, legLeanFrontOffset + frontStep);sequencer_write(20, legLeanFrontOffset + frontStep);
                        break;
                        
                case 6: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);  sequence = 0;
                        break;
                
                default: break;            
            }
            
            sequence++;
            LATA = sequence;
        }
        
        if (walkLeft){
            switch (sequence) {
                case 0: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);
                        
                case 1: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp + servoUpStep);
                        break;
                
                case 2: sequencer_write(0, legLeanFrontOffset + frontStep);sequencer_write(8, legLeanFrontOffset + frontStep); sequencer_write(20, legLeanFrontOffset - frontStep);
                        sequencer_write(4, legLeanFrontOffset - frontStep);sequencer_write(16, legLeanFrontOffset - frontStep); sequencer_write(24, legLeanFrontOffset + frontStep);
                        break; 
                        
                case 3: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        break;
                
                case 4: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp + servoUpStep);
                        break;
                
                case 5: sequencer_write(4, legLeanFrontOffset + frontStep); sequencer_write(16, legLeanFrontOffset + frontStep); sequencer_write(24, legLeanFrontOffset - frontStep);
                        sequencer_write(0, legLeanFrontOffset - frontStep); sequencer_write(8, legLeanFrontOffset - frontStep);sequencer_write(20, legLeanFrontOffset + frontStep);
                        break;
                        
                case 6: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);
                        sequence = 0;
                        break;
                
                default: break;
            }
            
            sequence++;
            LATA = sequence;
        }
        
        if (walkRight){
            switch (sequence) {
                case 0: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);
                        
                case 1: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp + servoUpStep);
                        break;
                
                case 2: sequencer_write(0, legLeanFrontOffset - frontStep);sequencer_write(8, legLeanFrontOffset - frontStep); sequencer_write(20, legLeanFrontOffset + frontStep);
                        sequencer_write(4, legLeanFrontOffset + frontStep);sequencer_write(16, legLeanFrontOffset + frontStep); sequencer_write(24, legLeanFrontOffset - frontStep);
                        break; 
                        
                case 3: sequencer_writeMultiple(lblfrmU, sizeof(lblfrmU), servoUp);
                        break;
                
                case 4: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp + servoUpStep);
                        break;
                
                case 5: sequencer_write(4, legLeanFrontOffset - frontStep); sequencer_write(16, legLeanFrontOffset - frontStep); sequencer_write(24, legLeanFrontOffset + frontStep);      
                        sequencer_write(0, legLeanFrontOffset + frontStep); sequencer_write(8, legLeanFrontOffset + frontStep);sequencer_write(20, legLeanFrontOffset - frontStep);
                        break;
                        
                case 6: sequencer_writeMultiple(lmrbrfU, sizeof(lmrbrfU), servoUp);  
                        sequence = 0;
                        break;
                
                default: break;            
            }
            
            sequence++;
            LATA = sequence;
        }
    }
    
    IFS0bits.T5IF = 0;       // clear interrupt flag
}
