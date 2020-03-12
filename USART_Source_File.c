
/* PIC18F4550 USART Source File 
 * http://www.electronicwings.com
 */

#include "USART_Header_File.h"
#define F_CPU 8000000/64
#define Baud_value 9600

/*****************************USART Initialization*******************************/
void USART_Init(long baud_rate)
{
    float temp;
    TRISC6=0;		/* Make Tx pin as output*/
    TRISC7=1;  		/* Make Rx pin as input*/

    /* Baud rate=9600, SPBRG = (F_CPU /(64*9600))-1*/
    temp= (( (float) (F_CPU) / (float) baud_rate ) - 1);     
    SPBRG = (int)temp;	

    TXSTA = 0x20;  	/* Enable Transmit(TX) */ 
    RCSTA = 0x90;  	/* Enable Receive(RX) & Serial */
}
/******************TRANSMIT FUNCTION*****************************************/ 
void USART_TransmitChar(char out)
{        
        while(TXIF==0);            /*wait for transmit interrupt flag*/
        TXREG=out;                 /*wait for transmit interrupt flag to set which indicates TXREG is ready
                                    for another transmission*/    
}
/*******************RECEIVE FUNCTION*****************************************/
char USART_ReceiveChar()
{

    while(RCIF==0);                 /*wait for receive interrupt flag*/
    return(RCREG);                  /*receive data is stored in RCREG register and return to main program */
}

void USART_SendString(const char *out)
{
   while(*out!='\0')
   {            
        USART_TransmitChar(*out);
        out++;
   }
}
