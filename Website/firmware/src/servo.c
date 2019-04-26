#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "servo.h"

#define TMR_TIME    0.02 // 20 ms

#define SERVO1      FUTABA_S3003    // valid value: FUTABA_S3003, HITEC_HS225BB
#define SERVO2      HITEC_HS225BB   // valid value: FUTABA_S3003, HITEC_HS225BB

#define FUTABA_S3003    1
#define HITEC_HS225BB   2

#define FUTABA_S3003_FRQ_MIN    532     // in us
#define FUTABA_S3003_FRQ_MAX   2188     // in us
#define FUTABA_S3003_FRQ_RANGE (FUTABA_S3003_FRQ_MAX - FUTABA_S3003_FRQ_MIN)

#define HITEC_HS225BB_FRQ_MIN    680    // in us
#define HITEC_HS225BB_FRQ_MAX   2540    // in us
#define HITEC_HS225BB_FRQ_RANGE (HITEC_HS225BB_FRQ_MAX - HITEC_HS225BB_FRQ_MIN)

unsigned int sPR3 = 0;
static int servoAngle = 0;

/* ------------------------------------------------------------ */
/***	SRV_Init
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function initializes the hardware involved in the SRV module: 
**      The Servo 1 PWM and Servo 2 PWM pins are configured as digital out and mapped to OC5 and OC4. 
**      The OC5 and OC4 module of PIC32 are configured with a period of 20 ms given by Timer2.
**          
*/
void servo_init(int freq) {
    //servo_configurePins();
    servo_configureOCs(freq);
    // set angle of both servos to 0 degree:
    servo_setAngle(0,1);
    servo_setAngle(0,2);
}

int servo_getAngle()
{
    return servoAngle;
}

void servo_setAngle(int angle, int servo) {
    int width;
    
    // check for boundary value
    if (angle < -90) angle = -90;
    else if (angle > 90) angle = 90;

    if (servo == 1) {
        servoAngle = angle;
        width = servo_angleToWidth1(angle);
        servo_setPulseMicroseconds1(width);
    } else if (servo == 2) {
        width = servo_angleToWidth2(angle);
        servo_setPulseMicroseconds2(width);
    }
}

int servo_angleToWidth1(int angle) {
#if SERVO1 == FUTABA_S3003
    // flip the value for Futaba S3003:
    angle = -angle;
#endif
    // normalize the angle to value between 0-180
    angle += 90;
    
#if SERVO1 == FUTABA_S3003
    return (int) (((FUTABA_S3003_FRQ_RANGE) / (double) 180) * angle + FUTABA_S3003_FRQ_MIN);
#elif SERVO1 == HITEC_HS225BB
    return (int) (((HITEC_HS225BB_FRQ_RANGE) / (double) 180) * angle + HITEC_HS225BB_FRQ_MIN);
#endif

}

int servo_angleToWidth2(int angle) {
#if SERVO2 == FUTABA_S3003
    // flip the value for Futaba S3003:
    angle = -angle;
#endif
    // normalize the angle to value between 0-180
    angle += 90;
    
#if SERVO2 == FUTABA_S3003
    return (int) (((FUTABA_S3003_FRQ_RANGE) / (double) 180) * angle + FUTABA_S3003_FRQ_MIN);
#elif SERVO2 == HITEC_HS225BB
    return (int) (((HITEC_HS225BB_FRQ_RANGE) / (double) 180) * angle + HITEC_HS225BB_FRQ_MIN);
#endif
}

/* ------------------------------------------------------------ */
/***	SRV_ConfigureOCs
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the output compares and timer involved in the SRV module.
**      The OC5 and OC4 module of PIC32 are configured with a period of 20 ms given by Timer2.
**      This is a low-level function called by SRV_Init(), so user should avoid calling it directly.
**          
*/
void servo_configureOCs(int freq) {
    T3CONbits.TCKPS = 7;                // 1:256 prescale value
    T3CONbits.TGATE = 0;                // not gated input (the default)
    T3CONbits.TCS = 0;                  // PCBLK input (the default)
    T3CONbits.ON = 1;                   // turn on Timer2
    
    sPR3 = (int)(((float)(TMR_TIME * freq) / 128) + 1250); // period of 20 ms
    PR3 = sPR3;

    // Configure Output Compare Module 2
    OC5CONbits.ON = 0;       // Turn off OC5 while doing setup.
    OC5CONbits.OCM = 6;      // PWM mode on OC5; Fault pin is disabled
    OC5CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC5CONbits.ON = 1;       // Start the OC5 module

    // Configure Output Compare Module 3
    OC4CONbits.ON = 0;       // Turn off OC4 while doing setup.
    OC4CONbits.OCM = 6;      // PWM mode on OC4; Fault pin is disabled
    OC4CONbits.OCTSEL = 1;   // Timer3 is the clock source for this Output Compare module
    OC4CONbits.ON = 1;       // Start the OC4 module
}

/* ------------------------------------------------------------ */
/***	SRV_ConfigurePins
**
**	Parameters:
**		
**
**	Return Value:
**		
**
**	Description:
**		This function configures the digital pins involved in the SRV module:
**      The servo 1 and servo 2 pins are defined as digital out and mapped to OC5 and OC4. 
**      The function uses pin related definitions from config.h file.
**      This is a low-level function called by SRV_Init(), so user should avoid calling it directly.
**          
*/
void servo_configurePins() {
    tris_SRV_S0PWM = 0;
    tris_SRV_S1PWM = 0;
    
    // disable analog (set pins as digital))
    ansel_SRV_S0PWM = 0;
    
    // remap pins to OCs
    rp_SRV_S0PWM = 0x0B; // 1011 = OC5
    rp_SRV_S1PWM = 0x0B; // 1011 = OC4    
}

/* ------------------------------------------------------------ */
/***	SRV_SetPulseMicroseconds1
**
**	Parameters:
**		unsigned short usVal - the pulse width in microseconds
**                              
**	Return Value:
**		
**
**	Description:
**		This function configures the output compare 5 (corresponding to servo 1) 
**      according to the specified pulse width.
**          
*/
void servo_setPulseMicroseconds1(unsigned short usVal) {
    float fVal;
    
    // check for boundary value:
#if SERVO1 == FUTABA_S3003
    if(usVal < FUTABA_S3003_FRQ_MIN) usVal = FUTABA_S3003_FRQ_MIN;
    else if (usVal > FUTABA_S3003_FRQ_MAX) usVal = FUTABA_S3003_FRQ_MAX;
#elif SERVO1 == HITEC_HS225BB
    if(usVal < HITEC_HS225BB_FRQ_MIN) usVal = HITEC_HS225BB_FRQ_MIN;
    else if (usVal > HITEC_HS225BB_FRQ_MAX) usVal = HITEC_HS225BB_FRQ_MAX;
#endif
    
    fVal = ((float)(sPR3 + 1) * (float)usVal)/ (TMR_TIME * 1000000);
    OC5RS = (unsigned short)fVal;

}

/* ------------------------------------------------------------ */
/***	SRV_SetPulseMicroseconds2
**
**	Parameters:
**		unsigned short usVal - the pulse width in microseconds
**                              
**	Return Value:
**		
**
**	Description:
**		This function configures the output compare 4 (corresponding to servo 2) 
**      according to the specified pulse width.
**          
*/
void servo_setPulseMicroseconds2(unsigned short usVal) {
    float fVal;
    
    // check for boundary value:
#if SERVO2 == FUTABA_S3003
    if(usVal < FUTABA_S3003_FRQ_MIN) usVal = FUTABA_S3003_FRQ_MIN;
    else if (usVal > FUTABA_S3003_FRQ_MAX) usVal = FUTABA_S3003_FRQ_MAX;
#elif SERVO2 == HITEC_HS225BB
    if(usVal < HITEC_HS225BB_FRQ_MIN) usVal = HITEC_HS225BB_FRQ_MIN;
    else if (usVal > HITEC_HS225BB_FRQ_MAX) usVal = HITEC_HS225BB_FRQ_MAX;
#endif
    fVal = ((float)(sPR3 + 1) * (float)usVal)/ (TMR_TIME * 1000000);
    OC4RS = (unsigned short)fVal;

}

/* ------------------------------------------------------------ */
/***	SRV_Close
**
**	Parameters:
** 
**
**	Return Value:
**      
**
**	Description:
**		This functions releases the hardware involved in the SRV library: 
**      it turns off the OC5, OC4 and Timer 2 modules.
**      
**          
*/
void servo_close() {
    OC5CONbits.ON = 0;       // Stop the OC2 module
    OC4CONbits.ON = 0;       // Stop the OC3 module   
    T2CONbits.ON = 0;        // turn off Timer2
}
/* *****************************************************************************
 End of File
 */
