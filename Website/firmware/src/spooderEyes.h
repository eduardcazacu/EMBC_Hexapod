/* ************************************************************************** */
/** Descriptive File Name

 * Created by Eduard Cazacu
 * 
  @Description
        Handles the incomming scan data from the hexapod received over bluetooth
 * and the requests from the webserver for that new data.   
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define NUMBER_OF_BLOCKS 15
    
    //an array for storing the data:
    int blocks[NUMBER_OF_BLOCKS]; //the latest blocks
    int newBlocks[NUMBER_OF_BLOCKS];  //keep track of which blocks are new and haven't been
                        //sent to the website
    
    
    /*
     *Description:  Adds a new block to the stack 
     *Parameters:   blockId = Which block was received (0 to 14)
     *              blockValue = How close is the block? (0 to 255)
     
     */
    void setNewBlock(char blockID, char blockValue);
    
    /*
     *Description:  Returns all the new values through an array in the
     *              parameter list.
     * Parameters:  newData =  an array of the new data.
     *              length = the length of the data array.
     
     */
    void getNewBlocks(char newData[], int length);
    
    
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
