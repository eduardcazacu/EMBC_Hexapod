#ifndef _SEQUENCER_H
#define _SEQUENCER_H

/**
 * Platform: Basys MX3
 * Authors: Robert Konarskis, Michal Mikolajczyk
 * 
 * The purpose of this library is to provide basic sequencing for the
 * SSC32 module on a Hexapod. It supports sleep/wake and walk in
 * different directions
 * 
 * Note: this library uses Timer 5.
 */

/**
 * External functions
 */

/**
 * Initialize sequencer
 * 
 * @param freq - integer
 *        Peripheral bus clock frequency
 */
void sequencer_init(int freq);

/**
 * @Synchronous
 * 
 * Put hexapod to sleep
 */
void sequencer_sleep();

/**
 * @Synchronous
 * 
 * Make hexapod stand up
 */
void sequencer_wake();

/**
 * Stop movement of hexapod
 */
void sequencer_stop();

/**
 * Start walking forward
 */
void sequencer_walkForward();

/**
 * Start turning left
 */
void sequencer_walkLeft();

/**
 * Start turning right
 */
void sequencer_walkRight();

/**
 * Start walking back
 */
void sequencer_walkBack();

/**
 * Turn hexapod head horizontally
 * 
 * @param angle - integer
 *        angle to which the head turns in degrees
 *        range: -45 to 45
 */
void sequencer_moveHeadHorizontal(int angle);

/**
 * Turn hexapod head vertically
 * 
 * @param angle - integer
 *        angle to which the head turns in degrees
 *        range: -45 to 45
 */
void sequencer_moveHeadVertical(int angle);

/**
 * Internal functions
 */
void sequencer_write(int channel, int angle);
void sequencer_writeMultiple(int channels[], int size, int angle);
void sequencer_writeAll(int angle);

#endif /* _SEQUENCER_H */