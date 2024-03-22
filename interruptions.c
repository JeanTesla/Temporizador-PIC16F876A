#include "interruptions.h"

void config_global_interruptions() {  
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    config_uart_interruption();
    config_timer_interruption();
}

void config_uart_interruption() {
    PIE1bits.TXIE = 0;
    PIE1bits.RCIE = 1;
    TXIF = 1;
    RCIF = 1;
}

void config_timer_interruption() {
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    OPTION_REG = 0b00000001;
    TMR0 = 6; // corrensponde a um overflow a cada 1ms
}
