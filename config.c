#include "config.h"
#include <math.h>
#include "Global_Definitions.h"
#include <stdio.h>
#include <pic18f4550.h>
#include <stdbool.h>


#define MINTHR              8000
#define RESOLUTION          488

#define InternalOsc_8MHz    8000000
#define InternalOsc_4MHz    4000000
#define InternalOsc_2MHz    2000000
#define InternalOsc_1MHz    1000000
#define InternalOsc_500KHz  500000
#define InternalOsc_250KHz  250000
#define InternalOsc_125KHz  125000
#define InternalOsc_31KHz   31000

#define Timer2Prescale_1    1
#define Timer2Prescale_4    4
#define Timer2Prescale_16   16

#define _XTAL_FREQ 8000000     //4MHz
#define Trigger_Pulse LATD0
#define Trigger_Pulse2 LATB2 
#define Trigger_Pulse3 LATC2

unsigned Ultrasonic_select;

int period;




//THIS FUNCTION WAITS X SECONDS  WEEEELLL REALLY THIS ARE NOT SECONDS BUT ms
void wait_in_ms(unsigned int time) //rodrigo chi
{
    unsigned int myxtime = 0; //Declares an initial variable
    while(myxtime < time) // while this so call myxtime variable is minor to the variable the fuction recieves this keeps happening 
    {
        
        //count is count from the formula
        
       /*
        1/Tout = Fout= Fclk/((4*preescaler*(256-TMR0L)*count))
        * this case Fclk is 48MHz prescaler is 4 and count is set on this fuctiion
        
        * 
        *    1/Tout = Fout= 48Mhz/((4*4*(256-131)*24))
        
        * which give us a Fout=1000 or 1ms
        */
        unsigned int count = 0; //here
        TMR0L = 236;  
        T0IF = 0;
        TMR0ON = 1;
        while(count < 25) //
        {
            while(T0IF == 0);
            count++;
            T0IF = 0;
            TMR0L = 236;
        }
        TMR0ON = 0;
        myxtime++; //this 1ms stands as long as my fuction recieves the "time" variable see line 21
    }
    
}

void configPIC()
{
//    //All analog pins need to be input
//    //Output: 0; Input: 1
//    
//    TRISA = 0b00000000;     //Set all pins of port A as output
//    TRISB = 0b00000000;     //Set all pins of port B as output
//    TRISC = 0b00000000;     //Set all pins of port C as output
//    TRISD = 0b00000000;     //Set all pins of port D as output
//    TRISE = 0b00000000;     //Set all pins of part E as output
//    
//    INTCONbits.GIE = 0;     //Turn off ALL interrupts
//    
//    
    T0CON = 0b01000001;     // TIMER 0
                            // Timer 0 enabled; configured as 8-bit timer;
                            // internal clock; increment on low to high;
                            // pre-scaler is assigned; pre-scaler is 1:4
    
//    //Configuración de A/D (sensor QTR-8A)
//    ADCON0bits.ADON = 0;       //A/D convertidor inhabilitado.
//    ADCON1bits.VCFG = 0b00;    //Voltaje de Referencia para conversión, "Desde GND."
//    ADCON1bits.PCFG = 0b1001;  //A/D Selección de Puertos Analogicos. (AN0-AN5) 
//    ADCON2bits.ADFM = 1;       //A/D Result Format Select Justified (1 right, 0 left)
//    ADCON2bits.ADCS = 0b001;   //A/D Conversion Clock Select TADmin 0.7 us(8MHz/8= 1us = TAD)
//    ADCON2bits.ACQT = 0b101;   // A/D Acquisition Time Select (12TAD)    
//    TRISA = 0b11111111;
//    PORTA = 0;
//    
//    LEDON_TRIS = 0; 
//    LEDON = 0;
//     
//    
//
//                            
//   
//    LED_TRIS = OUTPUT;
//    TRISD=0;
//   
    
//Configuración de A/D (sensor QTR-8A)
    ADCON0bits.ADON = 0;       //A/D convertidor inhabilitado.
    ADCON1bits.VCFG = 0b00;    //Voltaje de Referencia para conversión, "Desde GND."
    ADCON1bits.PCFG = 0b0110;  //A/D Selección de Puertos Analogicos. (AN0-AN5) 
    ADCON2bits.ADFM = 1;       //A/D Result Format Select Justified (1 right, 0 left)
    ADCON2bits.ADCS = 0b001;   //A/D Conversion Clock Select TADmin 0.7 us(8MHz/8= 1us = TAD)
    ADCON2bits.ACQT = 0b101;   // A/D Acquisition Time Select (12TAD)    
    TRISA = 0b11111111;
    TRISE = 0b10000111;
    PORTA = 0;
    
    LEDON_TRIS = 0; 
    LEDON = 0;
    
// Generales
    
    TRISB = _OUTPUT;  //Entradas por INT0, INT1, INT2
    TRISC = _OUTPUT;  //TRISC = 0x00 OR 0b00000000
    TRISD = _OUTPUT;
   
    
    PORTB = _INPUT;
    PORTC = 0;
//    PORTE = 0;
    
//**** NO MOVER ***
    
//    T0CONbits.TMR0ON = 1;   //Timer0 On/Off Control
//    T08BIT = 1;             //Timer0 8-Bit/16-Bit Control (1 8-bit, 0 16-bit)
//    T0CONbits.T0CS = 0;     //Timer0 Clock Source Select (1 T0CKI, 0 Internal)
//    T0CONbits.PSA = 0;      //Timer0 Prescaler Assignment
//    T0CONbits.T0PS = 100;   //Timer0 Prescaler Select
//    INTCONbits.TMR0IF = 0;  //TMR0 Overflow Interrupt Flag (1 overflowed, 0 CLEARED)
//   
//    
    //CONFIGURATION BITS
    
// CONFIG1L
#pragma config PLLDIV = 2           // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2       // System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])

// CONFIG1H
#pragma config FOSC = INTOSC_HS          // Oscillator Selection bits (EC oscillator, CLKO function on RA6 (EC))
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)

// CONFIG2L
#pragma config BORV = 3                 // Brown-out Reset Voltage bits (Minimum setting 2.05V)

// CONFIG2H
#pragma config WDT = OFF                // Watchdog Timer Enable bit (WDT enabled)

// CONFIG3H
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config LVP = OFF                // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
}

void all_ok(){
   
        LED1 = _ON;
        wait_in_ms(1000);
        LED1 =_OFF;
        
      
}

void PWM_Init()             /* Initialize PWM */
{
    TRISCbits.TRISC1 = 0;   /* Set CCP2 pin as output for PWM out */
    TRISCbits.TRISC2 =0;    /* Set CCP1 pin also as output */
    CCP1CON = 0x0C;         /* Set PWM mode */
}

int setPeriodTo(unsigned long FPWM)/* Set period */
{
    int clockSelectBits, TimerPrescaleBits;
    int TimerPrescaleValue;
    float period;
    unsigned long FOSC, _resolution = RESOLUTION;

    if (FPWM < MINTHR)    {TimerPrescaleBits = 2; TimerPrescaleValue = Timer2Prescale_16;}
    else                  {TimerPrescaleBits = 0; TimerPrescaleValue = Timer2Prescale_1;}

    if (FPWM > _resolution)               {clockSelectBits = 7; FOSC = InternalOsc_8MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 6; FOSC = InternalOsc_4MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 5; FOSC = InternalOsc_2MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 4; FOSC = InternalOsc_1MHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 3; FOSC = InternalOsc_500KHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 2; FOSC = InternalOsc_250KHz;}
    else if (FPWM > (_resolution >>= 1))  {clockSelectBits = 1; FOSC = InternalOsc_125KHz;}
    else                                  {clockSelectBits = 0; FOSC = InternalOsc_31KHz;}

    period = ((float)FOSC / (4.0 * (float)TimerPrescaleValue * (float)FPWM)) - 1.0;
    period = round(period);

    OSCCON = ((clockSelectBits & 0x07) << 4) | 0x02;
    PR2 = (int)period;
    T2CON = TimerPrescaleBits;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;  /* Turn ON Timer2 */
    return (int)period;
}

void SetDuty1CycleTo(float Duty_cycle, int Period)/* Set Duty cycle for given period */
{
    int PWM10BitValue;
    
    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR1L = (PWM10BitValue >> 2);
    CCP1CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}
void SetDuty2CycleTo(float Duty_cycle, int Period)/* Set Duty cycle for given period */
{
    int PWM10BitValue;
    
    PWM10BitValue = 4.0 * ((float)Period + 1.0) * (Duty_cycle/100.0);
    CCPR2L = (PWM10BitValue >> 2);
    CCP2CON = ((PWM10BitValue & 0x03) << 4) | 0x0C;
}

/*==================Motor control=====================
 ¿seria mejor un control de motores aparte?

 */



void freno (bool left, bool right, int value){
    enable(_ON);
    if(left){
        LATD4= _ON;              /* adelante */
        LATD5 = _ON;
       period = setPeriodTo(1000);/* 10KHz PWM frequency */
       SetDuty1CycleTo(value,period);
    }
    if (right){
        LATD2 = _ON;              /* adelante */
        LATD7 = _ON;    
        period = setPeriodTo(1000);/* 10KHz PWM frequency */
       SetDuty1CycleTo(value,period);
    }

}

void motorder_avanzar(int value){
    period = setPeriodTo(1000);/* 10KHz PWM frequency */ //500hz mejora
     SetDuty1CycleTo(value,period);
    LATD2 = _OFF;              /* Initial Direction */
    LATD7 = _ON;
}
void motorizq_avanzar(int value){
   period = setPeriodTo(1000);/* 10KHz PWM frequency */
    SetDuty2CycleTo(value,period);
    LATD4= _OFF;              /* Initial Direction */
    LATD5 = _ON;
}
void motorder_atras(int value){
    period = setPeriodTo(1000);/* 10KHz PWM frequency */
    SetDuty2CycleTo(value,period);
    LATD2 = _ON;              /* Initial Direction */
    LATD7 = _OFF;

}
void motorizq_atras(int value){
    period = setPeriodTo(1000);/* 10KHz PWM frequency */
    SetDuty2CycleTo(value,period);
    LATD4= _ON;              /* Initial Direction */
    LATD5 = _OFF;
}
void enable(int x){
    if(x==1){
    LATD3=_ON;
    }
    if (x==0){
    LATD3=_OFF;
    }
}
void Motor(int left, int right){
    enable(_ON);
    motorder_avanzar(left);
    motorizq_avanzar(right);
    
}


void reversa(int left, int right){
    enable(_ON); 
    motorder_atras(left);
    motorizq_atras(right);
}
//===================================================================================================






/*===================QTR8A IR SENSOR FUCTIONS=========================================================*/

unsigned int ADC_Read (unsigned int channel){
    
     
    if (channel == 0){
       ADCON0bits.CHS = 0b0000;
    }
    if (channel == 1){
       ADCON0bits.CHS = 0b0001;
    }
    if (channel == 2){
       ADCON0bits.CHS = 0b0010;
    }
    if (channel == 3){
       ADCON0bits.CHS = 0b0011;
    }
    if (channel == 4){
       ADCON0bits.CHS = 0b0100;
    }
    if (channel == 5){
       ADCON0bits.CHS = 0b0101;
    }
    if (channel == 6){
       ADCON0bits.CHS = 0b0110;
    }
    if (channel == 7){
       ADCON0bits.CHS = 0b0111;
    }
    if (channel == 8){
       ADCON0bits.CHS = 0b1000;
    }
    if (channel == 9){
       ADCON0bits.CHS = 0b1001;
    }
    if (channel == 10){
       ADCON0bits.CHS = 0b1010;
    }
    if (channel == 11){
       ADCON0bits.CHS = 0b1011;
    }
    if (channel == 12){
       ADCON0bits.CHS = 0b1100;
    }
    __delay_ms(2);
    ADCON0bits.GO_nDONE = 1;   //Inicia la conversión de la señal analógica almacenada en el condensador
    while(ADCON0bits.GO_nDONE); //Espera que la conversión termine cuando ADCON0bits.GO=0
    return ((ADRESH<<8)+ADRESL); //Se concatenan los bits m·s y menos significativos
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
                                                       
int act_position(long van0, long van1, long van2, long van3, long van4, long van5){
    int pos=0;
    pos= 1000*((0*van0)+(1*van1)+(2*van2)+(3*van3)+(4*van4)+(5*van5))/(van0+van1+van2+van3+van4+van5);
    return(pos);

}


/*Sensor ultrasonico*/
//void Trigger_Pulse_10us()
//{
//     Ultrasonic_select =0;
//    Trigger_Pulse = 1;
//    __delay_us(10);
//    Trigger_Pulse = 0;
//}
//void Trigger_Pulse2_10us()
//{
//     Ultrasonic_select =1;
//    Trigger_Pulse2 = 1;
//    __delay_us(10);
//    Trigger_Pulse2 = 0;
//}
//void Trigger_Pulse3_10us()
//{
//    Ultrasonic_select =2;
//    Trigger_Pulse3 = 1;
//    __delay_us(10);
//    Trigger_Pulse3 = 0;
//}