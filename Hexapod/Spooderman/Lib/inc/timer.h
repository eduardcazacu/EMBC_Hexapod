#ifndef _TIMER_H
#define _TIMER_H

/**
 * This library provides basic timer functionality for Timers 1-5.
 * Allows initializing a timer and an interrupt for that timer (optional).
 */

/**
 * External functions
 */

/**
 * Initialize a timer
 * 
 * @param freq - integer
 *        Peripheral bus clock frequency.
 * 
 * @param channel - integer
 *        Timer selection (1-5).
 * 
 * @param periodMs - integer
 *        Timer period in ms. 
 *        Keep in mind that the upper limit depends on clock frequency.
 * 
 * @param interruptEnabled - boolean
 *        Initialize an interrupt for this timer.
 *        
 * @param interruptPriority - integer
 *        Interrupt priority (1-7).
 * 
 * @return success
 */
int timer_init(int freq, int channel, int periodMs, int interruptEnabled, int interruptPriority);

/**
 * Attach an interrupt to a timer
 * 
 * @param channel - integer
 *        Timer selection (1-5).
 * 
 * @param priority - integer
 *        Interrupt priority (1-7).
 */
void timer_attachInterrupt(int channel, int priority);

/**
 * Detach an interrupt from a timer
 * 
 * @param channel - integer
 *        Timer selection (1-5).
 */
void timer_detachInterrupt(int channel);

/**
 * Disable a timer
 * 
 * @param channel - integer
 *        Timer selection (1-5).
 * 
 * @return success
 */
int timer_stop(int channel);

/**
 * Reset timer counter
 * 
 * @param channel - integer
 *        Timer selection (1-5).
 */
void timer_reset(int channel);

/**
 * Internal functions
 */
int getPrescaler(int channel, int periodMs);
int getPrescalerCode(int channel, int prescaler);
int getPeriod(int channel, int periodMs, int prescaler);
void setupTimer(int channel, int period, int prescalerCode);
void setupInterrupt(int channel, int priority);

#endif /* _TIMER_H */