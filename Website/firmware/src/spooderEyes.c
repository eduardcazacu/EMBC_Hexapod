#include "spooderEyes.h"

void setNewBlock(char blockID, char blockValue) {
    blocks[(int)blockID] = blockValue;
    newBlocks[(int)blockID] = 1;
}

void testPattern(){
    int i;
    for(i=0;i<NUMBER_OF_BLOCKS;i++){
        setNewBlock(i,testArr[i]);
    }
    
    int temp = testArr[0];
    for(i=0;i<NUMBER_OF_BLOCKS-1;i++){
        testArr[i] = testArr[i+1];
    }
    testArr[NUMBER_OF_BLOCKS-1] = temp;
}

void getNewBlocks(char newData[], int length) {
    //clear the array:
    int i;
    for (i = 0; i < length; i++) {
        newData[i] = 0;
    }

    int dataIndex = 0;

    for (i = 0; i < NUMBER_OF_BLOCKS; i++) {
        if (newBlocks[i] == 1) {
            //check if there is enough space to send one more block
            if (dataIndex + 1 < length) {

                //this is a new block. add it to the array.
                newData[dataIndex] = i+1; //send the id of the block 
                                          //+1 because ascii 0 is null
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