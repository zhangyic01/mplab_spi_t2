/*
 * File:   main.c
 * Author: YiChao
 *
 * Created on July 9, 2017, 4:30 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "pic16F18345_config.h"
#include "spi.h"

void main(void) {
    
    //setup spi registers
    init_spi(SPI_MASTER,SPI_POSEDGE,SPI_M_FOSC_DIV4,SPI_CLK_LOW_IN_IDLE, 1);
    
    
    TRISB = 0xFF; //PORTB as input
    TRISA = 0x00; //PORTA as output
    PORTA = 0x00; //All LEDs OFF
    TRISC7 = 0;
    RC7 = 1;
    
    
    while(1)
    {
        RC7 = 0; //Slave Select
        __delay_ms(1);

        write_spi_data(PORTB);
        PORTB = read_spi_data();

        __delay_ms(1);
        RC7 = 1; //Slave Deselect

        __delay_ms(100);
    }
    
    return;
    
}
