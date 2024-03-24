#include "common_anode_display.h"

void disp_print(unsigned int number) {

    unsigned int aux_number = number,
            count = 1,
            number_of_digits = 0;

    if (aux_number == 0) {
        number_of_digits = 1;
    } else {
        while (aux_number > 0) {
            number_of_digits = aux_number % 10;
            aux_number = aux_number / 10;
            number_of_digits = count++;
        }
    }

    unsigned char array_digits[4];
    sprintf(array_digits, "%d", number);

    for (int i = 0; i < number_of_digits; i++) {
        PORTC |= 0b1111;
        disp_set_byte(array_digits[i]);
        PORTC ^= (1 << i);
        __delay_ms(2);
    }
}

void disp_config_mode(short int config_number) {

    char configs[] = {'0', '1', '2', '3'};

    PORTC |= 0b1111;
    disp_set_byte('c');
    PORTC ^= (1 << 0);
    __delay_ms(2);
    PORTC |= 0b1111;
    disp_set_byte(configs[config_number]);
    PORTC ^= (1 << 1);
    __delay_ms(2);
}

void disp_set_byte(unsigned char byte) {
    switch (byte) {
        case '0':
            PORTB = 0x3F;
            break;
        case '1':
            PORTB = 0x06;
            break;
        case '2':
            PORTB = 0x5B;
            break;
        case '3':
            PORTB = 0x4F;
            break;
        case '4':
            PORTB = 0x66;
            break;
        case '5':
            PORTB = 0x6D;
            break;
        case '6':
            PORTB = 0x7D;
            break;
        case '7':
            PORTB = 0x07;
            break;
        case '8':
            PORTB = 0x7F;
            break;
        case '9':
            PORTB = 0x6F;
            break;
        case 'a':
            PORTB = 0x77;
            break;
        case 'b':
            PORTB = 0x7C;
            break;
        case 'c':
            PORTB = 0x39;
            break;
        case 'd':
            PORTB = 0x5E;
            break;
        case 'e':
            PORTB = 0x79;
            break;
        case 'f':
            PORTB = 0x71;
            break;
        case 'n':
            PORTB = 0b1010100;
            break;
        default:
            PORTB = 0xFF;
            break;
    }
}

void disp_show_end() {
    char bytes[] = {'e', 'n', 'd'};

    for (int i = 0; i < 3; i++) {
        PORTC |= 0b1111;
        disp_set_byte(bytes[i]);
        PORTC ^= (1 << i);
        __delay_ms(2);
    }
}
