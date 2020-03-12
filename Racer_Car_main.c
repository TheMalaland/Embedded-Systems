/*
 * File:   Racer_Car_main.c
 * Author: rodri
 *
 * Created on 17 de abril de 2018, 08:04 AM
 */


#include <xc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Global_Definitions.h"
#include "lcd.h"
#include "config.h"
#include "USART_Header_File.h"
#include <stdbool.h>
#include <pic18f4550.h>


#define _XTAL_FREQ 8000000     //4MHz#define Trigger_Pulse LATD0
#define Trigger_Pulse LATD0
#define Trigger_Pulse2 LATB2 
#define Trigger_Pulse3 LATB3

unsigned Ultrasonic_select;


/*======Variables Globales=======*/

//==============Sensor QTR8A============

unsigned int an0;
unsigned int an1;
unsigned int an2;
unsigned int an3;
unsigned int an4;
unsigned int an5;
unsigned int an6;
unsigned int an7;

unsigned int an0max=0;
unsigned int an1max=0;
unsigned int an2max=0;
unsigned int an3max=0;
unsigned int an4max=0;
unsigned int an5max=0;
unsigned int an6max=0;
unsigned int an7max=0;

unsigned int an0min=100;
unsigned int an1min=100;
unsigned int an2min=100;
unsigned int an3min=100;
unsigned int an4min=100;
unsigned int an5min=100;
unsigned int an6min=100;
unsigned int an7min=100;

//conversión de 0 -1000
long van0;
long van1;
long van2;
long van3;
long van4;
long van5;
long van6;
long van7;

//resultado posición
int vposicion;


/*contantes PID*/
int Velmax=18; //set on 25

float KP=0.011; 
float KD=0.019;
float Ki=0.0012;
/*
 *
linea recta 7 de mayo 2018
float KP=0.005; //
float KD=0.00;
float Ki=0.00;
 * 
linea recta 7 de mayo 2018
float KP=0.09; //
float KD=0.012;
float Ki=0.05;
 * 
 * float KP=0.009; 
float KD=0.012;
float Ki=0.0012;
 * 
 *  * float KP=0.009; 
float KD=0.012;
float Ki=0.0010;
 * 
 * 
 * float KP=0.012; 
float KD=0.017;
float Ki=0.0010;
 * 
 * MUCHISIMAS VUELTAS
 * float KP=0.009; 
float KD=0.017;
float Ki=0.0010;
 * 
 * 
 * MEJOR EN CURVAS PERO NO EN RECTAS
 * float KP=0.009; 
float KD=0.019;
float Ki=0.0010;
 * 
 * 
 * float KP=0.011; 
float KD=0.019;
float Ki=0.0012;
 * 
 * 
 * 
 * 
 * 
 */
 
/*variables para PID*/
int proporcional=0;
int integral =0;
int derivativo=0;

/*errores para el integral*/

int error1=0;
int error2=0;
int error3=0;
int error4=0;
int error5=0;
int error6=0;

int diferencial=0; //deferencia aplicada a los motores
int last_prop;     //ultimo valor proporcional aplicado para calcular la derivada del error
 

int target=2500;  //set point
/*========*/

float dist_1=0;
float dist_2=0;
float dist_3=0;


float D1=0;
float D2=0;
float D3=0;


unsigned char volt_string_an0[25]; 
unsigned char volt_string_an1[25]; 
unsigned char volt_string_an2[25]; 
unsigned char volt_string_an3[25]; 
unsigned char volt_string_an4[25]; 
unsigned char volt_string_an5[25];
unsigned char volt_string_an6[25];
unsigned char volt_string_an7[25];
unsigned char posicion[10];
unsigned char cproporcional[10];
unsigned char cdiferencial[10];
unsigned char sensor[10];

/*rutina de obstaculo*/

int esquivar=0;
void Trigger_Pulse2_10us();

//=============================

void interrupt echo()
{
  if(RBIF == 1)                 //Makes sure that it is PORTB On-Change Interrupt
  {
    RBIF = 0;                     //Clear PORTB On-Change Interrupt flag
    RBIE = 0;                   //Disable On-Change Interrupt
    if(RB4 == 1 && Ultrasonic_select ==0)
    {                  //If ECHO is HIGH
      TMR3ON = 1;     
    }//Start Timer
    if(RB4 == 0 && Ultrasonic_select ==0)                  //If ECHO is LOW
    {
      TMR3ON = 0;                    //Stop Timer
      dist_1 = (TMR3L | (TMR3H<<8))/58.82;  //Calculate Distance
        
    }
    if(RB5 == 1 && Ultrasonic_select ==1)
    {                  //If ECHO is HIGH
      TMR3ON = 1;     
    }//Start Timer
    if(RB5 == 0 && Ultrasonic_select ==1)                  //If ECHO is LOW
    {
      TMR3ON = 0;                    //Stop Timer
      dist_2 = (TMR3L | (TMR3H<<8))/58.82;  //Calculate Distance
    }
    
     if(RB6 == 1 && Ultrasonic_select ==2)
    {                  //If ECHO is HIGH
      TMR3ON = 1;     
    }//Start Timer
    if(RB6 == 0 && Ultrasonic_select ==2)                  //If ECHO is LOW
    {
      TMR3ON = 0;                    //Stop Timer
      dist_3 = (TMR3L | (TMR3H<<8))/58.82;  //Calculate Distance
        
    }
    
  }
  RBIE = 1;                     //Enable PORTB On-Change Interrupt
}

int calibrate(){
    for (int i=0; i<1000;i++){
        an0 = ADC_Read(0);
        // Se lee y almacena el valor ADC
        an1 = ADC_Read(1);
   
        an2 = ADC_Read(2);
    
        an3 = ADC_Read(3);  // Se lee y almacena el valor AD
    
        an4 = ADC_Read(4);
    
        an5 = ADC_Read(5); 
        
//        an6 =  ADC_Read(6); 
//        
//        an7 =  ADC_Read(7); 
        
        if(an0>an0max){
            an0max=an0;
        }
        if(an0<an0min){
            an0min=an0;
        }
         if(an1>an1max){
            an1max=an1;
        }
        if(an1<an1min){
            an1min=an1;
        }
        
         if(an2>an2max){
            an2max=an2;
        }
        if(an2<an2min){
            an2min=an2;
        }
         if(an3>an3max){
            an3max=an3;
        }
        if(an3<an3min){
            an3min=an3;
        }
         if(an4>an4max){
            an4max=an4;
        }
        if(an4<an4min){
            an4min=an4;
        }
         if(an5>an5max){
            an5max=an5;
        }
        if(an5<an5min){
            an5min=an5;
        }
        i=i++;   
        __delay_ms(3);
    }
        LED1 = _ON;
        wait_in_ms(1000);
        LED1 =_OFF;
       
}




void main(void) {
    OSCCON= 0b11110011;  //Oscilador interno a 8Mhz, no watchdog timer
    configPIC();
    all_ok();
    PWM_Init();             /* Initialize PWM */
    lcd_iniciar();      //Se inicializa el LCD
    lcd_limpiar();
    USART_Init(9600);
    wait_in_ms(5);
    //Period = setPeriodTo(1000);/* 10KHz PWM frequency */
    LEDON = _ON;
    ADCON0bits.ADON = 1; //  A/D Encendido.
    __delay_ms(2);   
    /* Note that period step size will gradually increase with PWM frequency */
    GIE = 1;                      //Global Interrupt Enable
    RBIF = 0;                     //Clear PORTB On-Change Interrupt Flag
    RBIE = 1;                     //Enable PORTB On-Change Interrupt
    T3CON = 0x10;      
    TRISB = 0b01110000; 
    calibrate();
    while(1){
//    sprintf(volt_string_an0,"%d  ",an0min); 
//     USART_SendString(volt_string_an0);
//    sprintf(volt_string_an0,"%d  ",an0max);
//     USART_SendString(volt_string_an0);
//    
//      sprintf(volt_string_an0,"%d  ",an1min); 
//     USART_SendString(volt_string_an0);
//    sprintf(volt_string_an0,"%d  ",an1max);
//     USART_SendString(volt_string_an0);
//     
//       sprintf(volt_string_an0,"%d  ",an2min); 
//     USART_SendString(volt_string_an0);
//    sprintf(volt_string_an0,"%d  ",an2max);
//     USART_SendString(volt_string_an0);
//    
//       sprintf(volt_string_an0,"%d  ",an3min); 
//     USART_SendString(volt_string_an0);
//    sprintf(volt_string_an0,"%d  ",an3max);
//     USART_SendString(volt_string_an0);
//    
//    
//       sprintf(volt_string_an0,"%d  ",an4min); 
//     USART_SendString(volt_string_an0);
//    sprintf(volt_string_an0,"%d  ",an4max);
//     USART_SendString(volt_string_an0);
//     
//    sprintf(volt_string_an0,"%d  ",an5min); 
//    USART_SendString(volt_string_an0);
//    sprintf(volt_string_an0,"%d  \n",an5max);
//    USART_SendString(volt_string_an0);
//    
//           
       /* ================================================MAIN======================================================*/
    TMR3=0;
    //dist=0;
    Trigger_Pulse_10us();
    //dist_1 = dist_1 + 1; //Error Correction Constant
     USART_SendString("Sensor 1: ");
    sprintf(sensor,"%2.2f cm  ",dist_1);
    USART_SendString(sensor);
    
    TMR3=0;
    
    
    Trigger_Pulse2_10us();
    //dist_2 = dist_2 + 1; //Error Correction Constant
     USART_SendString("Sensor 2: ");
    sprintf(sensor,"%2.2f  cm  ",dist_2);
    USART_SendString(sensor);
    
    TMR3=0;
    
    Trigger_Pulse3_10us();
    //dist_3 = dist_3 + 1; //Error Correction Constant
    USART_SendString("Sensor 3: ");
    sprintf(sensor,"%2.2f cm  \n ",dist_3);
    USART_SendString(sensor);
    
    
   // if(dist>=15){      
        /*lectura de sensor QTRA-8*/
        an0 = ADC_Read(0);  // Se lee y almacena el valor ADC
   
        an1 = ADC_Read(1);
   
        an2 = ADC_Read(2);
    
        an3 = ADC_Read(3);  // Se lee y almacena el valor AD
    
        an4 = ADC_Read(4);
    
        an5 = ADC_Read(5); 
        
        /*TRANSFORMA VALORES A ESCALA 0-1000*/
        van0=map(an0, an0min,an0max,0,1000);
        van1=map(an1, an1min,an1max,0,1000);
        van2=map(an2, an2min,an2max,0,1000);
        van3=map(an3, an3min,an3max,0,1000);
        van4=map(an4, an4min,an4max,0,1000);
        van5=map(an5, an5min,an5max,0,1000);

        /*Si es menor a un valor imprime blanco*/
//       if(van0<=41 && van1<=41 && van2<=41 && van3<=41 && van4<=41 && van5<=41){
//        USART_SendString("BLANCO \n");
//           Motor(0,0);
//       }
//               
        /*caso contrario, imprime los valores del sensor transformados de 0-1000|*/
        if(van0>41 || van1>41 || van2>41 || van3>41 || van4>41 || van5>41){
         
        vposicion=act_position(van0,van1,van2, van3,van4, van5); 
         
         /*valor del propocional, obtiene diferencial de error*/
         proporcional=((int)vposicion)-2500;
         
         
         /*EMPIEZA DERIVATIVO E INTEGRAL PID*/
         
         derivativo=proporcional-last_prop;
         
         integral=error1+error2+error3+error4+error5+error6;
         
         last_prop=proporcional;
         
        error6=error5;
        error5=error4;  
        error4=error3;  
        error3=error2;
        error2=error1;
        error1=proporcional;
        
        diferencial = ( proporcional * KP ) + ( derivativo * KD )+ (integral*Ki) ;
         
        
        if ( diferencial > Velmax ) diferencial = Velmax; 
        else if ( diferencial < -Velmax ) diferencial = -Velmax;
        
        ( diferencial < 0 ) ? 
    Motor(Velmax+diferencial, Velmax) : Motor(Velmax, Velmax-diferencial);

//         if(proporcional <= -target){
//             enable(_ON);
//             freno(true,false,18);
//         }
//         else if( proporcional >= target)
//         {
//             enable(_ON);
//             freno(false,true,18);
//         
//         }
    /*variables*/
        
//         
//    sprintf(volt_string_an0,"%d  ",an0); 
//    sprintf(volt_string_an1,"%d  ",an1);
//    sprintf(volt_string_an2,"%d  ",an2); 
//    sprintf(volt_string_an3,"%d  ",an3);
//    sprintf(volt_string_an4,"%d  ",an4); 
//    sprintf(volt_string_an5,"%d  \n",an5);
//    
//    //USART_SendString(sensor);
//    
//    //USART_SendString(sensor);
//    
//    USART_SendString(volt_string_an0);
//    USART_SendString(volt_string_an1);
//    USART_SendString(volt_string_an2);
//    USART_SendString(volt_string_an3);
//    USART_SendString(volt_string_an4);
//    USART_SendString(volt_string_an5);
//    
//    
//    
//    USART_SendString(posicion);
//    USART_SendString(cproporcional);
//    USART_SendString(cdiferencial);
//    
    }
    //vuelta sentido horario    
    if(dist_1<=20){
        Motor(0,0);
        D1=dist_1;
        //LED1=_ON;
        __delay_ms(2000); //paro de motores
        while(dist_2>20){
           TMR3=0;
           Trigger_Pulse2_10us();
           sprintf(sensor,"%2.2f cm  \n",dist_2);
           USART_SendString(sensor);
           Motor(0,15);//motor izquierdo arranca
        }
        Motor(0,0);
        __delay_ms(1000);
        D2=dist_2;
        while( dist_2<=D2+2){
           TMR3=0;
           Trigger_Pulse2_10us();
           sprintf(sensor,"%2.2f cm  \n",dist_2);
           USART_SendString(sensor);
           Motor(10,10);
        }
        Motor(10,10);
        __delay_ms(100);
        Motor(0,0);
        __delay_ms(1000);
        Motor(15,0);  //derecha
        __delay_ms(900);
        Motor(0,0);
        __delay_ms(1000);
        D2=dist_2;
        while(dist_2>10){
           TMR3=0;
           Trigger_Pulse2_10us();
           sprintf(sensor,"%2.2f cm  \n",dist_2);
           USART_SendString(sensor);
           Motor(10,10);
        }
        Motor(0,0);
        __delay_ms(1000);
        D2=dist_2;
        while(D2+15>dist_2){
            TMR3=0;
           Trigger_Pulse2_10us();
           sprintf(sensor,"%2.2f cm  \n",dist_2);
           USART_SendString(sensor);
            Motor(10,10);
        }
        Motor(10,10);
        __delay_ms(100);
        Motor(0,0);
        __delay_ms(1000);
        
         do{
        an0 = ADC_Read(0);  // Se lee y almacena el valor ADC
   
        an1 = ADC_Read(1);
   
        an2 = ADC_Read(2);
    
        an3 = ADC_Read(3);  // Se lee y almacena el valor AD
    
        an4 = ADC_Read(4);
    
        an5 = ADC_Read(5); 
        
        ///TRANSFORMA VALORES A ESCALA 0-1000/
        van0=map(an0, an0min,an0max,0,1000);
        van1=map(an1, an1min,an1max,0,1000);
        van2=map(an2, an2min,an2max,0,1000);
        van3=map(an3, an3min,an3max,0,1000);
        van4=map(an4, an4min,an4max,0,1000);
        van5=map(an5, an5min,an5max,0,1000);
        Motor(13,10);
        }while(van1<=an1min+4 && van2<=an2min && van3<=an3min+4 && van4<=an4min && van5<=an5min && van6<= an6min && van0<=an0min);
        
        
//          Motor(15,0);  //derecha
//        __delay_ms(900);
//        Motor(0,0);
//        __delay_ms(1000);
//        D2=dist_2;
//        while( dist_2<=D2+2){
//           TMR3=0;
//           Trigger_Pulse2_10us();
//           sprintf(sensor,"%2.2f cm  \n",dist_2);
//           USART_SendString(sensor);
//           Motor(10,10);
//        }
//        Motor(10,10);
//        __delay_ms(100);
//        Motor(0,0);
//        __delay_ms(1000);

        
//         do{
//        an0 = ADC_Read(0);  // Se lee y almacena el valor ADC
//   
//        an1 = ADC_Read(1);
//   
//        an2 = ADC_Read(2);
//    
//        an3 = ADC_Read(3);  // Se lee y almacena el valor AD
//    
//        an4 = ADC_Read(4);
//    
//        an5 = ADC_Read(5); 
//        
//        ///TRANSFORMA VALORES A ESCALA 0-1000/
//        van0=map(an0, an0min,an0max,0,1000);
//        van1=map(an1, an1min,an1max,0,1000);
//        van2=map(an2, an2min,an2max,0,1000);
//        van3=map(an3, an3min,an3max,0,1000);
//        van4=map(an4, an4min,an4max,0,1000);
//        van5=map(an5, an5min,an5max,0,1000);
//        Motor(11,9);
//        }while(van1<=an1min+4 && van2<=an2min && van3<=an3min+4 && van4<=an4min && van5<=an5min && van6<= an6min && van0<=an0min);
//        
        
        
        
        
//        LED1=_ON;
//        __delay_ms(1000);
//         LED1=_OFF;
//         __delay_ms(2000);
//        Motor(10,10);
//        __delay_ms(700);
//        Motor(15,0);
//        __delay_ms(900);
//          Motor(10,10);
//        __delay_ms(900);
//          Motor(15,0);
//        __delay_ms(800);
//        LED1=_ON;
//        __delay_ms(100);
//         LED1=_OFF;
//        do{
//        an0 = ADC_Read(0);  // Se lee y almacena el valor ADC
//   
//        an1 = ADC_Read(1);
//   
//        an2 = ADC_Read(2);
//    
//        an3 = ADC_Read(3);  // Se lee y almacena el valor AD
//    
//        an4 = ADC_Read(4);
//    
//        an5 = ADC_Read(5); 
//        
//        ///TRANSFORMA VALORES A ESCALA 0-1000/
//        van0=map(an0, an0min,an0max,0,1000);
//        van1=map(an1, an1min,an1max,0,1000);
//        van2=map(an2, an2min,an2max,0,1000);
//        van3=map(an3, an3min,an3max,0,1000);
//        van4=map(an4, an4min,an4max,0,1000);
//        van5=map(an5, an5min,an5max,0,1000);
//        Motor(9,9);
//        }while(van1<=an1min+4 && van2<=an2min && van3<=an3min+4 && van4<=an4min && van5<=an5min && van6<= an6min && van0<=an0min);
        Motor(0,0);
        __delay_ms(100);
//        motorder_avanzar(15);
//        __delay_ms(1000);
        LED1=_ON;
        __delay_ms(500);
         LED1=_OFF;
    }
        
}
}


void Trigger_Pulse_10us()
{
     Ultrasonic_select =0;
    Trigger_Pulse = 1;
    __delay_us(10);
    Trigger_Pulse = 0;
}
void Trigger_Pulse2_10us()
{
     Ultrasonic_select =1;
    Trigger_Pulse2 = 1;
    __delay_us(10);
    Trigger_Pulse2 = 0;
}
void Trigger_Pulse3_10us()
{
    Ultrasonic_select =2;
    Trigger_Pulse3 = 1;
    __delay_us(10);
    Trigger_Pulse3 = 0;
}

