/*
 * File:   Main.c
 * Author: X1
 *
 * Created on January 29, 2022, 10:33 PM
 */

#include "config.h"
#include "pic.h"
#define _XTAL_FREQ 8000000 // Oscillator Speed 8 MHZ
#define BIT(a) (1 << (a))

char UART_Buffer = 'w';
char *pointer = &UART_Buffer;
void UART_send_char(char bt);
char UART_get_char(void);
void flash_red(void);
void initiate(void);
void UART_send_string(char* st_pt);
//void interrupt ISR(void);


void main(void) {
    
    RCSTAbits.SPEN = 0;
    initiate();
    __delay_ms(1500);
    
    PORTC = 0x00; // Turning RC0 off (0b0000 0000)
    UART_send_string("Start Value: ");
    UART_send_char(UART_Buffer);
    UART_send_char('\r');
    
    UART_send_string("Initialized");
    UART_send_char('\r');
    
    __delay_ms(250);
    RCSTAbits.SPEN = 1;
    __delay_ms(250);
    
    while(1){
        
       //PORTC = 0x00; // Turning RC0 off (0b0000 0000)


       UART_send_string("Begin loop");
       UART_send_char('\r');
       UART_send_string("Get char: ");
       UART_send_char('\r');
       
       __delay_ms(1000);
       
       
       UART_Buffer = UART_get_char();
       
       UART_send_string("Buffer Value: ");
       UART_send_char(UART_Buffer);
       UART_send_char('\r');
       
       if(UART_Buffer == 'a'){
       PORTC = 0x01;
       //UART_send_char('B');
       __delay_ms(2000);
       PORTC = 0x00;
       UART_send_char('a');
       __delay_ms(1000);
       }
       
       else if(UART_Buffer == 'b'){
       PORTC = 0x02;
       //UART_send_char('B');
       __delay_ms(2000);
       PORTC = 0x00;
       UART_send_char('b');
       __delay_ms(1000);
       }
       
       else if(UART_Buffer == 'c'){
       PORTC = 0x03;
       //UART_send_char('B');
       __delay_ms(2000);
       PORTC = 0x00;
       UART_send_char('c');
       __delay_ms(1000);
       }
       else{
       flash_red(); 
       }
//       if(UART_Buffer == 0 ){
//           for(int i=0;i<10;i++){
//           PORTC = 0x03;
//           __delay_ms(250);
//           PORTC = 0x00;
//          __delay_ms(250);
//           }
//         UART_send_string("Buffer Value = 0 ");
//         __delay_ms(1000);
//        }
       
//    flash_red(); 
       
}
}


void UART_send_char(char bt)  
{
    while(!PIR1bits.TXIF);  // hold the program till TX buffer is free
    TXREG = bt; //Load the transmitter buffer with the received value
}

void UART_send_string(char* st_pt)
{
    while(*st_pt) //if there is a char
        UART_send_char(*st_pt++); //process it as a byte data
}


char UART_get_char(void)   
{
    char garbage;
        if(RCSTAbits.OERR) // check for overrun Error 
        {
            UART_send_string("Overrun Error");
            UART_send_char('\r');

            RCSTAbits.CREN = 0; //If error -> Reset 
            RCSTAbits.CREN = 1; //If error -> Reset 
        }
   
   while(!PIR1bits.RCIF){
        UART_send_string("In while loop");
        UART_send_char('\r');
        
        if(RCSTAbits.FERR) // check for framing error 
        {
            UART_send_string("Framing Error");
            UART_send_char('\r');
            RCSTAbits.SREN = 0;
            __delay_ms(2000);
            RCSTAbits.SREN = 1;

             UART_send_string("Reset");
             UART_send_char('\r');
             UART_send_char('\r');
        }
        
        if(BAUDCTLbits.RCIDL == 1){
        UART_send_string("Idling");
        UART_send_char('\r');
        }
        
  }  // hold the program till RX buffer is free
    
    UART_send_string("Reading");
    UART_send_char('\r');
    __delay_ms(10);
    
    return RCREG; //receive the value and send it to main function
}


void flash_red(void){
    PORTC = 0x01; // Turning RC0 on (0b0000 0001)
    __delay_ms(500);
    PORTC = 0x00; // Turning RC0 off (0b0000 0000)
    __delay_ms(500);
}


void initiate(void){
       
    TRISC =  0b11111100; // Setting all PORTC pins to inputs besides pin RC0,RC1
    TRISB =  0b01110000; // PortB has 4 pins. We make pin RB7 (TX) an output and RB5 (RX) an input
    OSCCON = 0b01110111; // Setting the internal oscillator clock to the factory frequency of 8 MHz
        
    //Setting up serial COM registers
    TXSTA =   0b00100110;
         //bit7-CSRC =0
         //bit6-TX9  =0
         //bit5-TXEN =1
         //bit4-SYNC =0
         //bit3-SENDB=0
         //bit2-BRGH =1 // JUST CHANGED from 0 to 1 for 19231 BAUD
         //bit1-TRMT =1 a read bit
         //bit0-TX9D =0
    
    RCSTA =   0b10110000;
    
         //bit7-SPEN =1
         //bit6-RX9  =0
         //bit5-SREN =1
         //bit4-CREN =1
         //bit3-ADDEN=0
         //bit2-FERR =0
         //bit1-OERR =0
         //bit0-RX9D =0 
    
    BAUDCTL = 0b00000000;
    
         //bit7-ABDOVF=0 
         //bit6-RCIDL =0
         //bit5--     =0
         //bit4-SCKP  =0
         //bit3-BRG16 =0
         //bit2--     =0
         //bit1-WUE   =0
         //bit0-ABDEN =0 
    
    SPBRG = 25; //((_XTAL_FREQ/64)/Baud_rate) - 1      CHANGED FROM 12
    
    //Interrupt Control
    INTCON = 0b11000000; // Turned on the global and peripheral interrupt bits
    
         //bit7-GIE   =1 
         //bit6-PEIE  =1
         //bit5-T0IE  =0
         //bit4-INTE  =0
         //bit3-RABIE =0
         //bit2-T0IF  =0
         //bit1-INTF  =0
         //bit0-RABIF =0 
    
    PIE1 =   0b00110000; //Turned on the EUSART interrupt bits

         //bit7--     =0 
         //bit6-ADIE  =0
         //bit5-RCIE  =1
         //bit4-TXIE  =1
         //bit3-SSPIE =0
         //bit2-CCP1IE=0
         //bit1-TMR2IE=0
         //bit0-TMR1IE=0   
    
}





//void __interrupt() ISR(void){
//    
//    UART_send_string("Entered Interrupt");
//    UART_send_char('\r');
//    
//    if(PIR1bits.RCIF == 1){ //Checking to see if the bit has been flagged
//        *pointer = RCREG;
//        UART_send_string("Read Receiver");
//        UART_send_char('\r');
//        
//    }
//    }


/*
 I've been trying to solve this issue for a while. Sending data works well but receiving does not function. As we can see in the screenshot of the Tera Term, when I run the MC I initialize the buffer value to a 'w'. When I go to read the RX pin, we notice the value changed to a null value ' '. When I do the second pass to read the RX pin, we notice the value remains null ' ' again. During the second loop, I get stuck in the while loop waiting for RCIF bit to become true. We remain in this loop for the remaining of the time. While in the loop, a framing error occurs. To clear the error, the datasheet says to clear the SPEN bit which I do but nothing appears to change.



If you have questions, let me know and I will do my best to answer. If you can see an issue, I will appreciate if you share your findings. Thank you


 
 */