/*
 * File:   Main.c
 * Author: X1
 *
 * Created on June 17, 2022, 6:43 PM
 */

//RC0 = RCLK
//RC1 = SRCLR
//RC2 = SRCLK
//RC3 = SER

#include "config.h"
#include "pic.h"

#define LATCH_OUTPUT PORTCbits.RC4 //Pin6
#define CLEAR_REGISTER PORTCbits.RC5 //Pin 5
#define SHIFT_CLOCK PORTCbits.RC6 //Pin 8
#define SERIAL_DATA PORTBbits.RB4 //Pin 13

void input_data(unsigned char num);
void latch_output(void);
void serial_clock_pulse(void);
void clear_shift_register(void);
void input_high(void);
void input_low(void);
void initiate(void);
void red_arrow(void);
void animation_red_arrow(void);
void green_arrow(void);
void animation_green_arrow(void);

unsigned char timer_count = 0;

void interrupt ISR() {


    if (PIR1bits.TMR1IF == 1) {
        TMR1 = 63035; //10 ms overflow time
        timer_count++;

        PIR1bits.TMR1IF = 0;
    }

}

#define _XTAL_FREQ 8000000 // Oscillator Speed 8 MHZ

void main(void) {
    initiate();

    while (1) {
        //while (timer_count < 20) {

        animation_green_arrow();

        __delay_ms(1000);

        animation_red_arrow();

        __delay_ms(1000);

        //        while (timer_count < 200) {
        //            green_arrow();
        //        }

        //}
        timer_count = 0;
        //        __delay_ms(100);
        //        while (timer_count < 200) {
        //
        //            red_arrow();
        //
        //
        //        }


        timer_count = 0;
    }
}

void initiate(void) {

    TRISC = 0b00001111; // Setting all PORTC pins to inputs besides pin RC4,RC5,RC6,RC7
    TRISB = 0b01100000; // PortB has 4 pins. We make pin RB7 (TX) an output and RB5 (RX) an input. Set RB4 as an output
    OSCCON = 0b01110111; // Setting the internal oscillator clock to the factory frequency of 8 MHz
    SHIFT_CLOCK = 0;

    // Timer1 set-up

    T1CONbits.TMR1ON = 1; // Enable Timer1 (Clock speed = Fosc/4 = 2 MHz) tmr1
    T1CONbits.T1CKPS1 = 1; //Prescaler = 8
    T1CONbits.T1CKPS0 = 1;
    TMR1 = 63035;

    //Time delay = ( 65535 - TMR1) * Prescalar * Machine cycle
    //0.0500 s = ( 65535 - TMR1) * 8 * (1/2e6 s) ----> TMR1 = 53035 for 50 ms overflow

    //Setting up serial COM registers
    TXSTA = 0b00100010;
    //bit7-CSRC =0
    //bit6-TX9  =0
    //bit5-TXEN =1
    //bit4-SYNC =0
    //bit3-SENDB=0
    //bit2-BRGH =0 
    //bit1-TRMT =1 a read bit
    //bit0-TX9D =0

    RCSTA = 0b10110000;

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

    SPBRG = 12; //((_XTAL_FREQ/64)/Baud_rate) - 1      
    
    
    //    //Interrupt Control
    INTCON = 0b11000000; // Turned on the global and peripheral interrupt bits

    //bit7-GIE   =1 
    //bit6-PEIE  =1
    //bit5-T0IE  =0
    //bit4-INTE  =0
    //bit3-RABIE =0
    //bit2-T0IF  =0
    //bit1-INTF  =0
    //bit0-RABIF =0 

    PIE1 = 0b00110001; //Turned on the EUSART interrupt bits

    //bit7--     =0 
    //bit6-ADIE  =0
    //bit5-RCIE  =1
    //bit4-TXIE  =1
    //bit3-SSPIE =0
    //bit2-CCP1IE=0
    //bit1-TMR2IE=0
    //bit0-TMR1IE=1 Turn on TIMER1 OVERFLOW interrupt (Check TMR1IF (must be cleared in software))

}

void clear_shift_register(void) {
    CLEAR_REGISTER = 1;
    __delay_ms(5);
    CLEAR_REGISTER = 0; //AND --> Creating a low to clear register
    __delay_ms(5);
    CLEAR_REGISTER = 1; //SRCLR = RC1 --> Creating positive edge (HIGH) OR
    __delay_ms(5);
}

void input_data(unsigned char num) {

    //either high or low
    //0-255 input

    //0b00000001 = 1

    if ((num & 0x01) == 0x01) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();

    //0b00000010 = 2

    if ((num & 0x02) == 0x02) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();

    //0b00000100 = 4

    if ((num & 0x04) == 0x04) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();

    //0b00001000 = 8

    if ((num & 0x08) == 0x08) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();

    //0b00010000 = 16

    if ((num & 0x10) == 0x10) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();

    //0b00100000 = 32

    if ((num & 0x20) == 0x20) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();

    //0b01000000 = 64

    if ((num & 0x40) == 0x40) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();


    //0b10000000 = 128


    if ((num & 0x80) == 0x80) {

        //make data pin high and pulse the clock
        input_high();

    } else input_low();


    //latch_output();
}

void input_high(void) {

    SERIAL_DATA = 1; //SER = RC3 Creating a positive pulse
    serial_clock_pulse();
    //SERIAL_DATA = 0; //SER = RC3

}

void input_low(void) {

    SERIAL_DATA = 0; //SER = RC3 Creating a positive pulse
    serial_clock_pulse();
    //SERIAL_DATA = 0; //SER = RC3

}

void serial_clock_pulse(void) {
    SHIFT_CLOCK = 0;
    __delay_us(5);
    SHIFT_CLOCK = 1; //SRCLK = RC2 --> Creating positive edge (HIGH) OR
    __delay_us(5);
    SHIFT_CLOCK = 0; //AND
    __delay_us(5);
}

void latch_output(void) {
    LATCH_OUTPUT = 0;
    __delay_us(5);
    LATCH_OUTPUT = 1; //RCLK = RCO --> Creating positive edge (HIGH) OR
    //__delay_ms(500);
    __delay_us(5);
    LATCH_OUTPUT = 0; //AND
    __delay_us(5);
}

void green_arrow(void) {
    for (int i = 0; i < 8; i++) {
        unsigned char row = 0b00000001;
        unsigned char col = 0;

        if (i != 0) {
            row = (row << i);
        }

        input_data(row); // loading the cathode register-

        if (i == 7) {

            col = 24; //00011000
        } else if (i == 0 || i == 1 || i == 2 || i == 3 || i == 6) {

            col = 60; //00111100
        } else if (i == 5) {

            col = 126; //01111110
        } else {

            col = 255; //11111111
        }
        input_data(col);
        input_data(0);

        latch_output();
        //__delay_ms(250);
    }
    input_data(0b10000000);
    input_data(0);
    input_data(0);
    latch_output();

}

void red_arrow(void) {
    for (int i = 0; i < 8; i++) {
        unsigned char row = 0b10000000;
        unsigned char col = 0;

        if (i != 0) {
            row = (row >> i);
        }

        input_data(row); // loading the cathode register


        if (i == 7) {

            col = 24; //00011000
        } else if (i == 0 || i == 1 || i == 2 || i == 3 || i == 6) {

            col = 60; //00111100
        } else if (i == 5) {

            col = 126; //01111110
        } else {

            col = 255; //11111111
        }

        input_data(0);
        input_data(col);
        latch_output();
        //__delay_ms(250);
    }
    input_data(0b00000001);
    input_data(0);
    input_data(0);
    latch_output();


}

void animation_red_arrow(void) {
    unsigned char row = 0b10000000;
    unsigned char col = 0;
    unsigned char ani_timer = 2;
    //cycle 1
    while (timer_count < ani_timer) {
        row = 0b10000000;
        input_data(row);
        input_data(0);
        input_data(24);
        latch_output();
    }
    timer_count = 0;
    //cycle 2
    while (timer_count < ani_timer) {
        for (int i = 0; i < 2; i++) {
            row = 0b10000000;


            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 1) {

                col = 24; //00011000
            }

            if (i == 0) {

                col = 60; //00111100

            }
            input_data(0);
            input_data(col);
            latch_output();

        }

    }
    timer_count = 0;

    //cycle 3
    while (timer_count < ani_timer) {
        for (int i = 0; i < 3; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 2) {

                col = 24; //00011000
            }

            if (i == 1) {

                col = 60; //00111100

            }

            if (i == 0) {

                col = 126; //01111110

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 4
    while (timer_count < 4) {
        for (int i = 0; i < 4; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 3) {

                col = 24; //00011000
            }

            if (i == 2) {

                col = 60; //00111100

            }

            if (i == 1) {

                col = 126; //01111110

            }
            if (i == 0) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 5
    while (timer_count < 4) {
        for (int i = 0; i < 5; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 4) {

                col = 24; //00011000
            }

            if (i == 3 || i == 0) {

                col = 60; //00111100

            }

            if (i == 2) {

                col = 126; //01111110

            }
            if (i == 1) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 6
    while (timer_count < 4) {
        for (int i = 0; i < 6; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 5) {

                col = 24; //00011000
            }

            if (i == 4 || i == 0 || i == 1) {

                col = 60; //00111100

            }

            if (i == 3) {

                col = 126; //01111110

            }
            if (i == 2) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 7
    while (timer_count < 5) {
        for (int i = 0; i < 7; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 6) {

                col = 24; //00011000
            }

            if (i == 5 || i == 0 || i == 1 || i == 2) {

                col = 60; //00111100

            }

            if (i == 4) {

                col = 126; //01111110

            }
            if (i == 3) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 8
    while (timer_count < 5) {
        for (int i = 0; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 7) {

                col = 24; //00011000
            }

            if (i == 6 || i == 0 || i == 1 || i == 2 || i == 3) {

                col = 60; //00111100

            }

            if (i == 5) {

                col = 126; //01111110

            }
            if (i == 4) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //    if(ani_select == 0)
    //    {
    //        return 0;
    //    }

    while (timer_count < 200) {

        red_arrow();


    }

    //cycle 9
    while (timer_count < 5) {
        for (int i = 1; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 7 || i == 1 || i == 2 || i == 3 || i == 4) {

                col = 60; //00111100

            }

            if (i == 6) {

                col = 126; //01111110

            }
            if (i == 5) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 10
    while (timer_count < 4) {
        for (int i = 2; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 2 || i == 3 || i == 4 || i == 5) {

                col = 60; //00111100

            }

            if (i == 7) {

                col = 126; //01111110

            }
            if (i == 6) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 11
    while (timer_count < 4) {
        for (int i = 3; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 3 || i == 4 || i == 5 || i == 6) {

                col = 60; //00111100

            }

            if (i == 7) {

                col = 255; //11111111

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 12
    while (timer_count < ani_timer) {
        for (int i = 4; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 4 || i == 5 || i == 6 || i == 7) {

                col = 60; //00111100

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 13
    while (timer_count < ani_timer) {
        for (int i = 5; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 5 || i == 6 || i == 7) {

                col = 60; //00111100

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 14
    while (timer_count < ani_timer) {
        for (int i = 6; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 6 || i == 7) {

                col = 60; //00111100

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

    //cycle 15
    while (timer_count < ani_timer) {
        for (int i = 7; i < 8; i++) {
            row = 0b10000000;

            if (i != 0) {
                row = (row >> i);
            }

            input_data(row);

            if (i == 7) {

                col = 0;

            }
            input_data(0);
            input_data(col);
            latch_output();
        }
    }
    timer_count = 0;

}

void animation_green_arrow(void) {
    unsigned char row = 0b10000000;
    unsigned char col = 0;
    unsigned char ani_timer = 2;
    //cycle 1
    while (timer_count < ani_timer) {
        row = 0b00000001;
        input_data(row);
        input_data(24);
        input_data(0);
        latch_output();
    }
    timer_count = 0;
    //cycle 2
    while (timer_count < ani_timer) {
        for (int i = 0; i < 2; i++) {
            row = 0b00000001;


            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 1) {

                col = 24; //00011000
            }

            if (i == 0) {

                col = 60; //00111100

            }
            input_data(col);
            input_data(0);
            latch_output();

        }

    }
    timer_count = 0;

    //cycle 3
    while (timer_count < ani_timer) {
        for (int i = 0; i < 3; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 2) {

                col = 24; //00011000
            }

            if (i == 1) {

                col = 60; //00111100

            }

            if (i == 0) {

                col = 126; //01111110

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 4
    while (timer_count < 4) {
        for (int i = 0; i < 4; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 3) {

                col = 24; //00011000
            }

            if (i == 2) {

                col = 60; //00111100

            }

            if (i == 1) {

                col = 126; //01111110

            }
            if (i == 0) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 5
    while (timer_count < 4) {
        for (int i = 0; i < 5; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 4) {

                col = 24; //00011000
            }

            if (i == 3 || i == 0) {

                col = 60; //00111100

            }

            if (i == 2) {

                col = 126; //01111110

            }
            if (i == 1) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 6
    while (timer_count < 4) {
        for (int i = 0; i < 6; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 5) {

                col = 24; //00011000
            }

            if (i == 4 || i == 0 || i == 1) {

                col = 60; //00111100

            }

            if (i == 3) {

                col = 126; //01111110

            }
            if (i == 2) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 7
    while (timer_count < 5) {
        for (int i = 0; i < 7; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 6) {

                col = 24; //00011000
            }

            if (i == 5 || i == 0 || i == 1 || i == 2) {

                col = 60; //00111100

            }

            if (i == 4) {

                col = 126; //01111110

            }
            if (i == 3) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 8
    while (timer_count < 5) {
        for (int i = 0; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 7) {

                col = 24; //00011000
            }

            if (i == 6 || i == 0 || i == 1 || i == 2 || i == 3) {

                col = 60; //00111100

            }

            if (i == 5) {

                col = 126; //01111110

            }
            if (i == 4) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //    if(ani_select == 0)
    //    {
    //        return 0;
    //    }

    while (timer_count < 200) {

        green_arrow();


    }

    //cycle 9
    while (timer_count < 5) {
        for (int i = 1; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 7 || i == 1 || i == 2 || i == 3 || i == 4) {

                col = 60; //00111100

            }

            if (i == 6) {

                col = 126; //01111110

            }
            if (i == 5) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 10
    while (timer_count < 4) {
        for (int i = 2; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 2 || i == 3 || i == 4 || i == 5) {

                col = 60; //00111100

            }

            if (i == 7) {

                col = 126; //01111110

            }
            if (i == 6) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 11
    while (timer_count < 4) {
        for (int i = 3; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 3 || i == 4 || i == 5 || i == 6) {

                col = 60; //00111100

            }

            if (i == 7) {

                col = 255; //11111111

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 12
    while (timer_count < ani_timer) {
        for (int i = 4; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 4 || i == 5 || i == 6 || i == 7) {

                col = 60; //00111100

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 13
    while (timer_count < ani_timer) {
        for (int i = 5; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 5 || i == 6 || i == 7) {

                col = 60; //00111100

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 14
    while (timer_count < ani_timer) {
        for (int i = 6; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 6 || i == 7) {

                col = 60; //00111100

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

    //cycle 15
    while (timer_count < ani_timer) {
        for (int i = 7; i < 8; i++) {
            row = 0b00000001;

            if (i != 0) {
                row = (row << i);
            }

            input_data(row);

            if (i == 7) {

                col = 0;

            }
            input_data(col);
            input_data(0);

            latch_output();
        }
    }
    timer_count = 0;

}