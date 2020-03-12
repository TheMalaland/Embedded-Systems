

#ifndef GLOBAL_DEFINITIONS_H
#define	GLOBAL_DEFINITIONS_H

//THIS ARE THE CONFIGURATION FOR THE PROGRAM
//WE USUALLY CAN USE EITHER FOR A S

//===BASIC===
#define _ON 1
#define _OFF 0

#define _OUTPUT 0
#define _INPUT 1
//============

//=============================PARTICULAR===
//Motores
#define LED1 PORTDbits.RD1
#define SW1 PORTAbits.RA1
#define LED_TRIS TRISAbits.RA0
#define SW1_TRIS TRISAbits.RA1

//Sensor QTR-8A
#define LEDON_TRIS TRISAbits.TRISA4
#define LEDON PORTAbits.RA4

/*RACE CAR DEFINITION*/

#endif	/* GLOBALDEFINITIONS_H */

