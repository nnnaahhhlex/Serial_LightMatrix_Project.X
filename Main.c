/*
 * File:   Main.c
 * Author: X1
 *
 * Created on January 29, 2022, 10:33 PM
 */

#include "config.h"
#define _XTAL_FREQ 8000000 // Oscillator Speed

void main(void) {
    
    TRISC = 0x00; // Setting all PORTC pins to outputs
    OSCCON = 0b01110111; // Setting the internal oscillator clock to the factory frequency of 8 MHz
    
    while(1){
        
       PORTC = 0x00;
       __delay_ms(500);
       PORTC = 0x01;
       __delay_ms(1000);
        
    }
       
    
}

