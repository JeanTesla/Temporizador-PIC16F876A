/*
 * File:   newmain.c
 * Author: Tesla
 *
 * Created on 4 de Março de 2024, 13:52
 */

#include <xc.h>
#include <PIC16F876A.h>
#include "interruptions.h"
#include "uart_interface.h"
#include "common_anode_display.h"
#include <stdbool.h>

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000 

#define BTN_UP PORTAbits.RA2
#define BTN_DOWN PORTAbits.RA1
#define BTN_CONFIG_MODE PORTAbits.RA0
#define BUZZER PORTAbits.RA3
#define RELAY PORTCbits.RC4

unsigned long tick_timer_count = 1;

unsigned short int button_timer_count = 1,
        display_value = 0,
        config_mode_value = 1,
        decrement_wait_time_multiplier = 1,
        last_decrement_wait_time_multiplier = 1;

bool is_enabled_bip = false;
bool is_active_count = false;

void __interrupt() my_interruptions();
void beep();
void trigger_relay();
void change_multiplier();
void counters_reset();

void main(void) {

    PORTA = 0b00000000;
    TRISA = 0b00000111;
    PORTB = 0b00000000;
    TRISB = 0b00000000;
    PORTC = 0b00000000;
    TRISC = 0b10000000;
    ADCON1 = 7;
    CMCON = 7;
    T1CONbits.T1OSCEN = 0;

    config_global_interruptions();
    uart_init();

    while (1) {

        if (display_value > 0) {
            is_enabled_bip = true;
            trigger_relay();
            disp_print(display_value);
            is_active_count = true;
        } else {
            if (is_active_count) {
                RELAY = 0;
                beep();
                is_active_count = false;
                counters_reset();
            }
            disp_show_end();
        }

        while (BTN_CONFIG_MODE) {
            if (BTN_UP && (config_mode_value < 3)) {
                config_mode_value++;
                __delay_ms(250);
            }
            if (BTN_DOWN && (config_mode_value > 1)) {
                config_mode_value--;
                __delay_ms(250);
            }

            switch (config_mode_value) {
                case 1:
                    // 1 x 1000ms = 1 sec
                    decrement_wait_time_multiplier = 1;
                    change_multiplier();
                    break;
                case 2:
                    // 60 x 1000ms = 60 sec = 1 min
                    decrement_wait_time_multiplier = 60;
                    change_multiplier();
                    break;
                case 3:
                    // 3600 x 1000ms = 60 min = 1 hora
                    decrement_wait_time_multiplier = 3600;
                    change_multiplier();
                    break;
            }

            disp_config_mode(config_mode_value);
        }
    }
    return;
}

void __interrupt() my_interruptions() {
    if (RCIF == 1) {
        if (RCSTAbits.OERR || RCSTAbits.FERR) {
            CREN = 0;
            __nop();
            CREN = 1;
        }
        char byte = RCREG;
        if (byte == 'A') {
            // --- //
        }
        RCIF = 0;
    }

    if (T0IF) {

        /* INICIO A
         * 
         * Bloco de interrupção do Timer0
         * para decremento do valor do display.
         * Acontece a cada 
         * 1000ms x decrement_wait_time_multiplier
         */
        if (tick_timer_count ==
                (1000 * decrement_wait_time_multiplier)
                ) {
            //Se não tiverem sendo pressionados
            if (!BTN_UP && !BTN_DOWN) {
                if (display_value > 0) {
                    --display_value;
                }
            }
            TMR0 = 6;
            tick_timer_count = 1;
        }
        tick_timer_count++;
        /* FIM A*/

        /* INICIO B
         * 
         * Bloco de interrupção do Timer0
         * para manipular tempo de resposta
         * em milisegundos dos botões de Up e Down
         */
        if (button_timer_count == 200) {
            if (!BTN_CONFIG_MODE) {
                if (BTN_UP && display_value != 9999)
                    ++display_value;
                if (BTN_DOWN && display_value != 0)
                    --display_value;
            }
            button_timer_count = 1;
        }
        button_timer_count++;
        /* FIM B*/

        T0IF = 0;
    }
}

void beep() {

    if (!is_enabled_bip) return;

    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;
    __delay_ms(100);
    BUZZER = 1;
    __delay_ms(100);
    BUZZER = 0;

    is_enabled_bip = false;
}

void trigger_relay() {
    if (RELAY) return;
    RELAY = 1;
}

void change_multiplier() {
    if (last_decrement_wait_time_multiplier ==
            decrement_wait_time_multiplier) return;

    RELAY = 0;
    beep();
    counters_reset();

    last_decrement_wait_time_multiplier =
            decrement_wait_time_multiplier;
}

void counters_reset(){
    tick_timer_count = 1;
    display_value = 0;
    TMR0 = 6;
}