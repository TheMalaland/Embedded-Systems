/*
 * File:   lcd.c
 * Author: RenatoSoriano
 * Created on May 17, 2016, 8:53 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "lcd.h"
#include "config.h"

#define _XTAL_FREQ 20000000

/*
void lcd_iniciar(){
    //Poner en LOW el puerto de datos
    //LCD = 0b00000000; 
    D4 = 0;
    D5 = 0;
    D6 = 0;
    D7 = 0;
    //configurar puertos de datos como salida
    //LCD_TRIS = 0b00000000; 
    TRIS_D4 = 0;
    TRIS_D5 = 0;
    TRIS_D6 = 0;
    TRIS_D7 = 0;
    
    TRIS_RW = 0;
    TRIS_RS = 0;
    TRIS_E = 0;
    //poner en bajo todas las señales de control
    RW = 0;
    RS = 0; 
    E = 0;
    //Esperar al menos 15 ms para que se energice el LCD
    wait_in_ms(15);
    //Configurar 4bit,1-line,5x8
    lcd_comando(0b00100000); 
    //Esperar 5 us  1 ms
    wait_in_ms(1);
    //Configurar 4bit,1-line,5x8
    lcd_comando(0b00100000); 
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    //Configurar 4bit,1-line,5x8
    lcd_comando(0b00100000); 
    //Esperar a que el LCD se desocupe
    while(lcd_Busy()){}   
    //Configurar 4bit,2-line,5x8;
    lcd_comando(0b00101000);  
    //Esperar a que el LCD se desocupe
    while(lcd_Busy()){}   
    //Display, Cursor, Parpadeo desactivado
    lcd_comando(0b00001000);
    //Esperar a que el LCD se desocupe
    while(lcd_Busy()){}
    //Display, Cursor, Parpadeo activado
    lcd_comando(0b00001111);            ///CHECAAAAAAAARRRRR!!!! BIEN
    //Esperar a que el LCD se desocupe
    while(lcd_Busy()){}
    //Limpiar pantalla
    lcd_comando(0b00000001);  
    //Esperar a que el LCD se desocupe
    while(lcd_Busy()){}
    //Enviar al cursor al inicio
    lcd_comando(0b00000010);  
    //Esperar a que el LCD se desocupe
    while(lcd_Busy()){}
}
void lcd_comando(unsigned char comando){
    //Poner en el puerto de datos el valor del comando
    //LCD = comando; 
    D4 = comando;
    D5 = comando;
    D6 = comando;
    D7 = comando;
    //Configurar puertos de datos como salida
    //LCD_TRIS = 0b00000000; 
    TRIS_D4 = 0;
    TRIS_D5 = 0;
    TRIS_D6 = 0;
    TRIS_D7 = 0;
    //RW = ESCRIBIR
    RW = 0; 
    //RS = COMANDO
    RS = 0; 
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    // EN = Habilitado
    E = 1;  
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    // EN = Deshabilitado
    E = 0;  
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    //Puerto de datos como entrada 
    //LCD_TRIS = 0b11111111;   
    TRIS_D4 = 1;
    TRIS_D5 = 1;
    TRIS_D6 = 1;
    TRIS_D7 = 1;
    
}
void lcd_dato(unsigned char dato){
    //Poner en el puerto de datos el valor del dato
    //LCD = dato; 
    D4 = dato;
    D5 = dato;
    D6 = dato;
    D7 = dato;
    //Configurar puertos de datos como salida
    //LCD_TRIS = 0b00000000; 
    TRIS_D4 = 0;
    TRIS_D5 = 0;
    TRIS_D6 = 0;
    TRIS_D7 = 0;
    //RW = ESCRIBIR
    RW = 0; 
    //RS = dato
    RS = 1; 
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    // EN = Habilitado
    E = 1;  
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    // EN = Deshabilitado
    E = 0;  
    //RS = COMANDO
    RS = 0; 
    //Puerto de datos como entrada 
    //LCD_TRIS = 0b11111111;  
    TRIS_D4 = 1;
    TRIS_D5 = 1;
    TRIS_D6 = 1;
    TRIS_D7 = 1;
    
}
unsigned int lcd_Busy(){
    //RW = LEER
    RW = 1;   
    //RS = COMANDO
    RS = 0; 
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    // EN = Habilitado
    E = 1; 
    //Esperar 2 us ~ 1 ms
    wait_in_ms(1);
    //Checar la bandera de "ocupado"
        if (D7 == 1)
            {
              //EN = Deshabilitado
              E = 0; 
              //RW = ESCRIBIR
              RW = 0; 
              return 1;
            }
        else 
             {
                //EN = Deshabilitado
                E = 0;
                //RW = ESCRIBIR
                RW = 0;
                return 0;
             }
    
}
void lcd_texto(unsigned char T[32]){
    
    unsigned int i=0;
    
    while(T[i] != 0 && i<32)
    {
        lcd_dato(T[i]);
        i++;
        //Esperar 2 us ~ 1 ms
        wait_in_ms(1);
    }
    return;
}
void lcd_ubicarXY(char linea,char columna){  //Función para ubicar el cursor en cualquier ubicación del LCD
    lcd_comando(0b00000010);        //Enviar al cursor al inicio                           
    if (linea==1){
        for(char i=1; i<columna; i++){
            lcd_comando(0b00010100);
        }
    }
    if(linea==2){
        for (char i=1;i<(columna+40);i++){
            lcd_comando(0b00010100);
        }
    }
}
void lcd_limpiar(){     //Función para limpiar la pantalla LCD
    lcd_comando(0b00000001);
    wait_in_ms(2);
}
void lcd_origen(){        //Función par enviar el cursor al inicio de la pantalla
    lcd_comando(0b00000010);        //Cursor inicio pantalla
    lcd_comando(0b00001100);    // 1, Display ON (1), Cursor OFF (0), blink cursor OFF (0).
    //lcd_comando(0b00001111);        // 1, Display ON (1), Cursor ON (1), blink cursor ON (1).
    wait_in_ms(4);
}

//*******BETA**************
void lcd_movimiento_derecha()
{
	lcd_comando(0b00000001);
	lcd_comando(0b00001100);
}
void lcd_movimiento_izquierda()
{
	lcd_comando(0b00000001);
	lcd_comando(0b00001000);
}
*/

/***************************************+*/


void lcd_iniciar(){
    TRIS_RS=0;
    TRIS_RW=0;
    TRIS_E=0;
    TRIS_D4=0;
    TRIS_D5=0;
    TRIS_D6=0;
    TRIS_D7=0;
    
    RS=0;
    RW=0;
    E=0;
    D4=0;
    D5=0;
    D6=0;
    D7=0;
    
    wait_in_ms(20);
    PORTD=0X03<<4;                           //Esto también deberá modificarce,
    lcd_pulso();                             //dependiendo del puerto que se use para
    wait_in_ms(4);                           //el envio de datos u ordenes.
    
    PORTD=0X03<<4;
    lcd_pulso();
    wait_in_ms(4);
    
    PORTD=0X03<<4;
    lcd_pulso();
    wait_in_ms(1);
    
    PORTD=0X02<<4;                          //a 4 bits
    lcd_pulso();
    
    lcd_orden(0x28);                        //a 4 bits,2 lineas, 5×7 puntos
    wait_in_ms(8);
    
    lcd_orden(0x0c);                        //display encendido,no muestra cursor,sin parpadeo
    wait_in_ms(8);
    
    lcd_orden(0x01);                        //Limpia pantalla;
    wait_in_ms(2);
    
    lcd_orden(0x06);                        //incremento a la derecha, sin desplazamiento de display
    wait_in_ms(8);
}

void lcd_pulso(){                           //función para habilitar el LCD
    E=1;
    wait_in_ms(2);
    E=0;
    wait_in_ms(2);
}

void lcd_orden(unsigned int orden){       // ¡ES COMANDO!   Función para enviar ordenes al LCD   
    RS=0;
    //Se modificará  si se usa un puerto diferente al PORTD
    PORTD=(((((PORTD>>4&0x0f)|0x0f)&(orden>>4&0x0f))<<4)|(PORTD&0x0f));
    lcd_pulso();
    PORTD=(((((PORTD>>4&0x0f)|0x0f)&(orden&0x0f))<<4)|(PORTD&0x0f));
    lcd_pulso();
}

void lcd_caracter(unsigned int caracter){   //Función para enviar caracteres al LCD
    RS=1;
    //Se modificará  si se usa un puerto diferente al PORTD
    PORTD=(((((PORTD>>4&0x0f)|0x0f)&(caracter>>4&0x0f))<<4)|(PORTD&0x0f));
    lcd_pulso();
    PORTD=(((((PORTD>>4&0x0f)|0x0f)&(caracter&0x0f))<<4)|(PORTD&0x0f));
    lcd_pulso();
    RS=0;
}

void lcd_texto(char* cadena){  //Función para enviar cadenas de caracteres al LCD
    while(*cadena != 0x00){
        lcd_caracter(*cadena);
        cadena++;
    }
}

void lcd_limpiar(){     //Función para limpiar la pantalla LCD
    lcd_orden(0x01);
    wait_in_ms(2);
}

void lcd_origen(){        //Función par enviar el cursor al inicio de la pantalla
    lcd_orden(0x02);
    wait_in_ms(4);
}

void lcd_ubicarXY(char linea,char columna){  //Función para ubicar el cursor 
    lcd_origen();                            //en cualquier ubicación del LCD
    if (linea==1){
        for(char i=1; i<columna; i++){
            lcd_orden(0x14);
        }
    }
    if(linea==2){
        for (char i=1;i<(columna+40);i++){
            lcd_orden(0x14);
        }
    }
}

void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}

void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	E  = 1;             // => E = 1
    wait_in_ms(4);
    E  = 0;             // => E = 0
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}

 
/***************************************+*/
