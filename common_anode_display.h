#include <xc.h>
#include <stdio.h>
#include "uart_interface.h"

#define _XTAL_FREQ 4000000

void disp_print(unsigned int number);
void disp_config_mode(short int config_number);
void disp_set_byte(unsigned char);

