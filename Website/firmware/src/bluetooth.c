/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Description
 this file contains the definitions of the functions neccessary to communicate 
 * between the web server and the spider as well as to retrieve the results 
 * received from the spdier 
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* TODO:  Include other files here if needed. */
#include "bluetooth.h"
#include "uartjb.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

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
