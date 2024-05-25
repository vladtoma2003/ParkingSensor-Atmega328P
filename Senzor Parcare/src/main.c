#include <Arduino.h>
#include <avr/io.h>

#include "lcd_twi.h"
#include "twi.h"
#include "ultrasonic.h"
#include "buzzer.h"
#include "intteruption.h"
#include "usart.h"

#define CALC_USART_UBRR(BAUD) (F_CPU / 16 / (BAUD) - 1)
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/*
* \brief Convert a float to a string
*/
char *float_to_string(float num) {
    // only 2 decimal places
    static char str[10];
    sprintf(str, "%.2f", num);
    return str;
}

int main()
{
    // Write "Hello, World!" to lcd

    // Initialize LCD
    lcd_init();
    twi_init();

    // Initialize interrupt
    init_intteruption();
    init_timer2();

    // Initialize sensors ultrasonic
    init_sensor();

    // initialize LED
    init_LED();

    // Initialize buzzer
    init_buzzer();
    stop_buzzer();

    // Usart serial
    USART0_init(CALC_USART_UBRR(9600));
    USART0_use_stdio();

    uint16_t distance= 0;
    uint16_t distance1 = 0;
    uint16_t distance2 = 0;

    uint16_t old_distance1 = 0;
    uint16_t old_distance2 = 0;

    while (1) {
        ultrasonic_trigger();

        distance = ultrasonic_calculate_distance();
        if(turn == 0) {
            old_distance1 = distance1;
            distance1 = distance;
        } else {
            old_distance2 = distance2;
            distance2 = distance;
        }
        if(distance <= 50) { // display
            lcd_set_cursor(turn, 0);
            lcd_write("Distance: \0");
            lcd_write_number(distance);
            lcd_write("cm      \0");
            if(turn)  ping_buzzer(100);
        } else {
            lcd_clear_line(turn);
        }

        set_color(MIN(distance1, distance2));

        // if(active == 0) {
        //     if(old_distance1 != distance1 && old_distance2 != distance2) {
        //         active = 1;
        //     }
        // }

        turn = ++turn % 2;
        _delay_ms(100);
    }

    return 0;
}
