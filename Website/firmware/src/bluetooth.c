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
#include "spooderEyes.h"

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

//unused
//send error flag to spider
void send_internal_server_fault(){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_INTERNAL_SERVER_FAULT;
    msg[2] = 0;
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void send_toggle_auto_scan(int state){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_TOGGLE_AUTO_SCANNING;
    if (state==0||state==1){
        msg[2]=state;
    } else {
        return; 
    }
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void send_one_shot_scan(){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_ONE_SHOT_SCAN;
    msg[2] = 0;
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void send_toggle_move_forward(int state){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_MOVE_FORWARD;
    if (state==0||state==1){
        msg[2]=state;
    } else {
        return; 
    }
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void send_toggle_turn_left(int state){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_TURN_LEFT;
    if (state==0||state==1){
        msg[2]=state;
    } else {
        return; 
    }
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void send_toggle_turn_right(int state){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_TURN_RIGHT;
    if (state==0||state==1){
        msg[2]=state;
    } else {
        return; 
    }
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void send_toggle_move_backward(int state){
    char msg[6];
    msg[0] = COMMAND_PREFIX; 
    msg[1] = COMMAND_MOVE_BACKWARD;
    if (state==0||state==1){
        msg[2]=state;
    } else {
        return; 
    }
    msg[3] = (msg[0] + msg[1] + msg[2])%255;
    msg[4] = 0x0d; ///r/n
    msg[5] = 0x0a; 
    UARTJB_PutString(msg);
}

void bluetooth_decode_command(unsigned char* cmd){
    if(cmd[0]==0x5F){ //if first character is an underscore continue
        if((cmd[0]+cmd[1]+cmd[2])%255!=cmd[3]){
            return;
        }
        switch(cmd[1]){ //see what the command is 
            case SCAN_RESULT_RECT1:
                setNewBlock(INDEX_RECT1, cmd[2]);
                break;
                
           case SCAN_RESULT_RECT2:
                setNewBlock(INDEX_RECT2, cmd[2]);
                break;
            
            case SCAN_RESULT_RECT3:
                setNewBlock(INDEX_RECT3, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT4:
                setNewBlock(INDEX_RECT4, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT5:
                setNewBlock(INDEX_RECT5, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT6:
                setNewBlock(INDEX_RECT6, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT7:
                setNewBlock(INDEX_RECT7, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT8:
                setNewBlock(INDEX_RECT8, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT9:
                setNewBlock(INDEX_RECT9, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT10:
                setNewBlock(INDEX_RECT10, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT11:
                setNewBlock(INDEX_RECT11, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT12:
                setNewBlock(INDEX_RECT12, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT13:
                setNewBlock(INDEX_RECT13, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT14:
                setNewBlock(INDEX_RECT14, cmd[2]);
                break;
                
            case SCAN_RESULT_RECT15:
                setNewBlock(INDEX_RECT15, cmd[2]);
                break;
        }
    }
}
//unused
//get rectangle scan result
//to be used when the site polls for scan results
unsigned char get_rect_result(int index){
    if (scan_result_arr[0]){
        return 0;
    }
    return scan_result_arr[index];
}

//unused
void set_rect_result(unsigned char result, int index){
    scan_result_arr[index] = result;
}

/* *****************************************************************************
 End of File
 */
