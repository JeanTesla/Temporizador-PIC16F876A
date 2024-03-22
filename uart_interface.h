#include <xc.h>

void uart_init();
void uart_write_byte(unsigned char byte);
unsigned char uart_read_byte();
void uart_write_string(unsigned char *s);
void __interrupt() my_uart_interruption();