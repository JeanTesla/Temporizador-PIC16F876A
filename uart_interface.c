#include "uart_interface.h"

void uart_init() {
    TXSTA = 0b00100010; //BRGH=0, TXEN = 1, Asynchronous Mode, 8-bit mode
    RCSTA = 0b10010010; //Serial Port enabled,8-bit reception
    SPBRG = 12; //corresponde a um baud rate de 4800 para 4MHz   
    TRISC6 = 0; //Output (TX)
    TRISC7 = 1; //Input (RX)   
    TXSTAbits.TRMT = 1;
}

void uart_write_byte(unsigned char byte) {
    PORTBbits.RB3 = 1;
    while (!TXIF);
    TXREG = byte;
    PORTBbits.RB3 = 0;
}

unsigned char uart_read_byte() {
    RCIF = 0;
    while (!RCIF);
    return RCREG;
}

void uart_write_string(unsigned char *s) {
    while (*s) {
        uart_write_byte(*s++);
    }
}
