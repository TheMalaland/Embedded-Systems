/*
 * File:   lcd.h
 * Author: RenatoSoriano
 * Created on May 17, 2016, 8:53 AM
 */

#include <xc.h>
#include "config.h"


#define RS            PORTDbits.RD1          //Esto se puedee
#define TRIS_RS     TRISDbits.TRISD1        //modificar para utilizar
#define RW            PORTDbits.RD2          //otros pines del PIC
#define TRIS_RW     TRISDbits.TRISD2         //para los bits de control del LCD
#define E             PORTDbits.RD3
#define TRIS_E      TRISDbits.TRISD3

#define D4            PORTDbits.RD4          //Esto se puede modificar si se quiere
#define TRIS_D4     TRISDbits.TRISD4         //que los pines para el envio de datos
#define D5            PORTDbits.RD5          //u ordenes sean a través de otro puerto
#define TRIS_D5     TRISDbits.TRISD5         //pero es este caso siempre tendrá que
#define D6            PORTDbits.RD6          //ser el nibble alto del puerto que se utilice.
#define TRIS_D6     TRISDbits.TRISD6
#define D7            PORTDbits.RD7
#define TRIS_D7     TRISDbits.TRISD7

#define LCD_TRIS TRISD
#define LCD PORTD

/*
#define D4            PORTDbits.RD4          //Esto se puede modificar si se quiere
#define TRIS_D4     TRISDbits.TRISD4         //que los pines para el envio de datos
#define D5            PORTDbits.RD5          //u ordenes sean a través de otro puerto
#define TRIS_D5     TRISDbits.TRISD5         //pero es este caso siempre tendrá que
#define D6            PORTDbits.RD6          //ser el nibble alto del puerto que se utilice.
#define TRIS_D6     TRISDbits.TRISD6
#define D7            PORTDbits.RD7
#define TRIS_D7     TRISDbits.TRISD7
*/


void lcd_iniciar();
void lcd_pulso();
void lcd_orden(unsigned int);
void lcd_caracter(unsigned int );
void lcd_texto(char*);
void lcd_limpiar();
void lcd_origen();
void lcd_ubicarXY(char,char);
void Lcd_Port(char);
void Lcd_Cmd(char);
void Lcd_Shift_Right();
void Lcd_Shift_Left();


