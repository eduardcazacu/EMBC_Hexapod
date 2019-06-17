/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Fontys

  @File Name
    bluetooth.c

  @Description
 this file contains the definitions of the functions neccessary to communicate 
 * between the web server and the spider as well as to retrieve the results 
 * received from the spdier 
 */
/* ************************************************************************** */

#include "bluetooth.h"
#include "uartjb.h"

//holds the received results from scans 
//since it's a global variable it's initialized to zeros
unsigned char scan_result_arr[16];


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

//bluetooth init

void bluetooth_init() {
    UARTJB_Init(115200);
}

/*  TX functions
 */

//send error flag to spider
void send_internal_server_fault(){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_INTERNAL_SERVER_FAULT;
    msg[3] = 0;
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void send_toggle_auto_scan(int state){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_TOGGLE_AUTO_SCANNING;
    if (state==0||state==1){
        msg[3]=state;
    } else {
        return; 
    }
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void send_one_shot_scan(){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_ONE_SHOT_SCAN;
    msg[3] = 0;
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void send_toggle_move_forward(int state){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_MOVE_FORWARD;
    if (state==0||state==1){
        msg[3]=state;
    } else {
        return; 
    }
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void send_toggle_turn_left(int state){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_TURN_LEFT;
    if (state==0||state==1){
        msg[3]=state;
    } else {
        return; 
    }
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void send_toggle_turn_right(int state){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_TURN_RIGHT;
    if (state==0||state==1){
        msg[3]=state;
    } else {
        return; 
    }
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void send_toggle_move_backward(int state){
    char msg[4];
    msg[1] = COMMAND_PREFIX; 
    msg[2] = COMMAND_MOVE_BACKWARD;
    if (state==0||state==1){
        msg[3]=state;
    } else {
        return; 
    }
    msg[4] = (msg[1] + msg[2] + msg[3])%255;
    UARTJB_PutString(msg);
}

void bluetooth_decode_command(unsigned char* cmd){
    if(cmd[0]==0x5F){ //if first character is an underscore continue
        if((cmd[0]+cmd[1]+cmd[2])%255!=cmd[3]){
            return;
        }
        switch(cmd[1]){ //see what the command is 
            case SCAN_RESULT_ERROR: //scan error
                set_rect_result(1,INDEX_ERROR); //set the error result to 1 
                break;
              
            case SCAN_RESULT_RECT1:
                set_rect_result(cmd[2],INDEX_RECT1);
                break;
                
           case SCAN_RESULT_RECT2:
                set_rect_result(cmd[2],INDEX_RECT2);
                break;
            
            case SCAN_RESULT_RECT3:
                set_rect_result(cmd[2],INDEX_RECT3);
                break;
                
            case SCAN_RESULT_RECT4:
                set_rect_result(cmd[2],INDEX_RECT4);
                break;
                
            case SCAN_RESULT_RECT5:
                set_rect_result(cmd[2],INDEX_RECT5);
                break;
                
            case SCAN_RESULT_RECT6:
                set_rect_result(cmd[2],INDEX_RECT6);
                break;
                
            case SCAN_RESULT_RECT7:
                set_rect_result(cmd[2],INDEX_RECT7);
                break;
                
            case SCAN_RESULT_RECT8:
                set_rect_result(cmd[2],INDEX_RECT8);
                break;
                
            case SCAN_RESULT_RECT9:
                set_rect_result(cmd[2],INDEX_RECT9);
                break;
                
            case SCAN_RESULT_RECT10:
                set_rect_result(cmd[2],INDEX_RECT10);
                break;
                
            case SCAN_RESULT_RECT11:
                set_rect_result(cmd[2],INDEX_RECT11);
                break;
                
            case SCAN_RESULT_RECT12:
                set_rect_result(cmd[2],INDEX_RECT12);
                break;
                
            case SCAN_RESULT_RECT13:
                set_rect_result(cmd[2],INDEX_RECT13);
                break;
                
            case SCAN_RESULT_RECT14:
                set_rect_result(cmd[2],INDEX_RECT14);
                break;
                
            case SCAN_RESULT_RECT15:
                set_rect_result(cmd[2],INDEX_RECT15);
                break;
        }
    }
}

//get rectangle scan result
//to be used when the site polls for scan results
unsigned char get_rect_result(int index){
    if (scan_result_arr[0]){
        return 0;
    }
    return scan_result_arr[index];
}


void set_rect_result(unsigned char result, int index){
    scan_result_arr[index] = result;
}

/* *****************************************************************************
 End of File
 */
