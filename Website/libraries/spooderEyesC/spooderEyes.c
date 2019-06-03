#include "spooderEyes.h"

void setNewBlock(char blockID, char blockValue) {
    blocks[blockID] = blockValue;
    newBlocks[blockID] = 1;
}

void getNewBlocks(char newData[], int length) {
    //clear the array:
    for (int i = 0; i < length; i++) {
        newData[i] = 0;
    }

    int dataIndex = 0;

    for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
        if (newBlocks[i] == 1) {
            //check if there is enough space to send one more block
            if (dataIndex + 2 < length) {

                //this is a new block. add it to the array.
                newData[dataIndex] = i; //send the id of the block
                dataIndex++;
                newData[dataIndex] = blocks[i]; //add the value of the block
                dataIndex++;
                
                newBlocks[i] = 0;
            }
            else{
                return;
            }
        }
    }
}