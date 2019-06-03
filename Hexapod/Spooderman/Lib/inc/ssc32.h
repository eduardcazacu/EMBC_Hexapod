#ifndef _SSC32_H
#define _SSC32_H

/**
 * Platform: Basys MX3
 * Authors: Robert Konarskis, Michal Mikolajczyk
 * 
 * The purpose of this library is to provide an abstraction layer
 * for controlling the SSC32 servo controller using UART commands.
 * 
 * Note: the middle point is always angle 0. Example of range: -45 .. 45 degrees
 */

#define HEXAPOD_NUMBER  1   // valid value is a number between 1 and 7

#define HEXAPOD_HW_A    1
#define HEXAPOD_HW_B    2

#if HEXAPOD_NUMBER > 0 && HEXAPOD_NUMBER <= 3
	#define HEXAPOD_HW_VERSION     HEXAPOD_HW_A
#elif HEXAPOD_NUMBER >= 4 && HEXAPOD_NUMBER <= 7
	#define HEXAPOD_HW_VERSION     HEXAPOD_HW_B
#endif

#define UART_BAUDRATE      9600  // default uart baudrate
#define DEFAULT_RESOLUTION 90    // servo resolution in degrees
#if HEXAPOD_HW_VERSION == HEXAPOD_HW_A
	#define DEFAULT_MIN_WIDTH  1100   // servo minimum pulse width in us
	#define DEFAULT_MAX_WIDTH  1900  // servo maximum pulse width in us
#elif HEXAPOD_HW_VERSION == HEXAPOD_HW_B
	#define DEFAULT_MIN_WIDTH  1000   // servo minimum pulse width in us
	#define DEFAULT_MAX_WIDTH  2000  // servo maximum pulse width in us
#endif
#define STATUS_FREE 0
#define STATUS_BUSY 1

/**
 * External functions
 */

/**
 * Initialize SSC32 module.
 * If baud rate needs to be customized, edit @UART_BAUDRATE definition.
 * 
 * @param p_resolution - integer
 *        servo resolution in degrees (max angle - min angle)
 * 
 * @param p_minWidth - integer
 *        PWM width in us for minimum angle
 * 
 * @param p_maxWidth - integer
 *        PWM width in us for maximum angle
 *        
 */
void ssc32_init(int p_resolution, int p_minWidth, int p_maxWidth);

/**
 * Set a specific servo to a defined angle
 * 
 * @param channel - integer
 *        channel number (0 - 31)
 * 
 * @param angle - integer
 *        angle in degrees
 * 
 */
void ssc32_write(int channel, int angle);

/**
 * Set an array of servos to a defined angle
 * 
 * @param channels - int[]
 *        an array of channels (0 - 31)
 * 
 * @param size - integer
 *        size of the given array
 * 
 * @param angle - integer
 *        angle in degrees
 * 
 * Example usage: 
 *      int channels[] = { 4, 18, 19, 20, 21, 22 };
 *      ssc32_writeMultiple(channels, sizeof(channels), 23);
 */
void ssc32_writeMultiple(int channels[], int size, int angle);

/**
 * Set all servos to a defined angle
 * 
 * @param angle - integer
 *        angle in degrees
 */
void ssc32_writeAll(int angle);

/**
 * Request SSC32 status.
 * Register @StatusCallback before calling this.
 * Callback will be fired shortly after with STATUS_FREE or STATUS_BUSY result.
 */
void ssc32_requestStatus();

/**
 * SSC32 Status callback. 
 * Contains current SSC32 status (free of busy).
 */
typedef void (*StatusCallback)(int status);
void ssc32_registerStatusCallback(StatusCallback ptr_statusCallback);

#endif /* _SSC32_H */
