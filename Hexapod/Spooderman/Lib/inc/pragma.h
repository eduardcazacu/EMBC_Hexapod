/* 
 * File:   pragma.h
 * Author: snipe2nite
 *
 * Created on July 4, 2017, 1:49 PM
 */

#ifndef PRAGMA_H
#define	PRAGMA_H

#ifdef	__cplusplus
extern "C" {
#endif

#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      

/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */

// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =      PRI
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_16
#pragma config FPLLODIV =	DIV_8

#ifdef	__cplusplus
}
#endif

#endif	/* PRAGMA_H */
