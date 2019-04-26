/* ************************************************************************** */
/** Descriptive File Name
 * 
  @Description
        This file groups the declarations of the functions that implement
        the SRV library (defined in servo.c).
        Include the file in the project when this library is needed.
        Use #include "servo.h" in the source files where the functions are needed.
 */
/* ************************************************************************** */

#ifndef _SERVO_H    /* Guard against multiple inclusion */
#define _SERVO_H

#include <stdint.h>


void servo_init(int freq);

void servo_setAngle(int angle, int servo);
int servo_getAngle();
int servo_angleToWidth1(int angle);
int servo_angleToWidth2(int angle);

void servo_setPulseMicroseconds1(unsigned short usVal);
void servo_setPulseMicroseconds2(unsigned short usVal);
void servo_close();

// private functions
void servo_configureOCs(int freq);
void servo_configurePins();

#endif /* _SERVO_H */

/* *****************************************************************************
 End of File
 */
