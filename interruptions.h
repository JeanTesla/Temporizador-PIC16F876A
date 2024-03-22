#include <xc.h>
#include "uart_interface.h"
#define _XTAL_FREQ 4000000

void config_global_interruptions();
void config_uart_interruption();
void config_timer_interruption();