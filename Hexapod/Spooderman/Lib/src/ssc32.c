#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include "ssc32.h"
#include "uart.h"

#define HEXAPOD_NUMBER  1   // valid value is a number between 1 and 7

const int channels[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

int resolution = DEFAULT_RESOLUTION;
int minWidth   = DEFAULT_MIN_WIDTH;
int maxWidth   = DEFAULT_MAX_WIDTH;

int deltaWidth;
int halfResolution;

StatusCallback statusCallback;
unsigned char statusCallbackFlag = 0;
int waitingForStatus = 0;

void onUartData(char rx[]);

void ssc32_init(int p_resolution, int p_minWidth, int p_maxWidth) {
    UART_Init(UART_BAUDRATE);
    
    resolution = p_resolution;
    minWidth = p_minWidth;
    maxWidth = p_maxWidth;
    
    deltaWidth = maxWidth - minWidth;
    halfResolution = (resolution / 2 > 0) ? resolution / 2 : 0;
    
    int i;
    for (i = 0; i < 10000; i++) {}
    
    registerUartCallback(onUartData);
}

void ssc32_registerStatusCallback(StatusCallback ptr_statusCallback) {
    statusCallback = ptr_statusCallback;
    statusCallbackFlag = 1;
}

void ssc32_write(int channel, int angle) {
    execute(channel, angleToWidth(angle));
}

void ssc32_writeMultiple(int channels[], int size, int angle) {
    int i;
    for (i = 0; i < size/sizeof(int); i++) {
        ssc32_write(channels[i], angle);
    }
}

void ssc32_writeAll(int angle) {
    ssc32_writeMultiple(channels, sizeof(channels), angle);
}

void ssc32_requestStatus() {
    waitingForStatus = 1;
    UART_PutString("Q");
}

void onUartData(char rx[]) {
    if (statusCallbackFlag && waitingForStatus) {
        if (rx[0] == '+') {
            statusCallback(STATUS_BUSY);
        } else if (rx[0] == '.') {
            statusCallback(STATUS_FREE);
        }
        
        waitingForStatus = 0;
    }
}

void execute(int channel, int width) {
    char cmd[64];
    sprintf(cmd, "#%d P%d \r\n", channel, width);
    UART_PutString(cmd);
}

int angleToWidth(int angle) {
    return (int) ((deltaWidth / (double) resolution) * (angle + halfResolution) + minWidth);
}