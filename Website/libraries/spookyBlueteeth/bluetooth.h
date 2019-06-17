/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Fontys

  @File Name
 bluetooth.h

  @Description
 This file groups the declarations of the functions that implement the functions
 * which connect send data to the hexapod as well as the functions which get
 * data sent from the hexapod to the boad.
 */
/* ************************************************************************** */

#ifndef _BLUETOOTH_H   /* Guard against multiple inclusion */
#define _BLUETOOTH_H




/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    //used as parameters
#define SET_STATE_OFF 0
#define SET_STATE_ON 1 

    //tx commands from server to spider
#define COMMAND_INTERNAL_SERVER_FAULT 0x00
#define COMMAND_TOGGLE_AUTO_SCANNING 0x01
#define COMMAND_ONE_SHOT_SCAN 0x02
#define COMMAND_MOVE_FORWARD 0x03
#define COMMAND_TURN_LEFT 0x04
#define COMMAND_TURN_RIGHT 0x05
#define COMMAND_MOVE_BACKWARD 0x06
    
    //rx commands from spider to serevr 
#define SCAN_RESULT_ERROR  0xF0
#define SCAN_RESULT_RECT1  0xF1
#define SCAN_RESULT_RECT2  0xF2    
#define SCAN_RESULT_RECT3  0xF3
#define SCAN_RESULT_RECT4  0xF4
#define SCAN_RESULT_RECT5  0xF5
#define SCAN_RESULT_RECT6  0xF6
#define SCAN_RESULT_RECT7  0xF7
#define SCAN_RESULT_RECT8  0xF8
#define SCAN_RESULT_RECT9  0xF9
#define SCAN_RESULT_RECT10 0xFA
#define SCAN_RESULT_RECT11 0xFB
#define SCAN_RESULT_RECT12 0xFC
#define SCAN_RESULT_RECT13 0xFD    
#define SCAN_RESULT_RECT14 0xFE
#define SCAN_RESULT_RECT15 0xFF

#define INDEX_ERROR 0 
#define INDEX_RECT1 1
#define INDEX_RECT2 2
#define INDEX_RECT3 3
#define INDEX_RECT4 4
#define INDEX_RECT5 5
#define INDEX_RECT6 6
#define INDEX_RECT7 7
#define INDEX_RECT8 8
#define INDEX_RECT9 9
#define INDEX_RECT10 10
#define INDEX_RECT11 11
#define INDEX_RECT12 12
#define INDEX_RECT13 13
#define INDEX_RECT14 14
#define INDEX_RECT15 15

#define COMMAND_PREFIX 0x5F 

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    
    //init function
    void bluetooth_init();
    
    // *****************************************************************************
    // TX functions
    // *****************************************************************************
    
    //send error flag to spider 
    void send_internal_server_fault();    
    
    //commands the spider to turn on/off auto scann
    // state = 0 - turn off
    // state = 1 - turn on
    void send_toggle_auto_scan(int state);

    //commands the spider to scan every direction once
    void send_one_shot_scan();
    
    //command toggles forward movement
    //state = 0 - off
    //state = 1 - on
    void send_toggle_move_forward(int state);
    
    //commands toggles turning left
    //state = 0 - off
    //state = 1 - on
    void send_toggle_turn_left(int state);
    
    //commands toggles turning right
    //state = 0 - off
    //state = 1 - on
    void send_toggle_turn_right(int state);
    
    //commands toggles backward movement
    //state = 0 - off
    //state = 1 - on
    void send_toggle_move_backward(int state);
    
    
    // *****************************************************************************
    // retrieve scan results
    // *****************************************************************************
    
    //gets the result of latest scan at given index
    //index - number corresponding to the rectangle from 1 to 15 
    
    
    void bluetooth_decode_command(char* cmd);
    
    
    //returns number in cm 
    //returns 0 if rectangle has never been scanned or wrong index 
    unsigned char get_rect_result(int index);
    
    
    //set the scan result at the index of the rectangle
    //to be used in the ISR only
    void set_rect_result(unsigned char result, int index);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _BLUETOOTH_H */

/* *****************************************************************************
 End of File
 */
