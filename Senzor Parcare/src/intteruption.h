#include <avr/io.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/atomic.h>
#include "usart.h"

#define BUTTON PD2

#define RED PD3
#define GREEN PD5

volatile uint8_t button_state = 0;
volatile uint8_t button_debounce_count = 0;

volatile uint8_t active = 1;

/*
 * \brief Initialize button
 */
void init_button(void) {
    // Set button as input
    DDRD &= ~_BV(BUTTON);

    // Enable pull-down resistor
    PORTD &= ~_BV(BUTTON);
}

/*
 * \brief Check if button is pressed
 */
uint8_t is_button_pressed(void) {
    // Check if button is pressed
    return !(PIND & _BV(BUTTON));
}

/*
 * \brief Initialize intteruption
 */
void init_intteruption(void) {
    cli();
    // Set INT0 as falling edge
    EICRA |= _BV(ISC01);
    EICRA &= ~_BV(ISC00);

    // Enable INT0
    EIMSK |= _BV(INT0);
    
    sei();
}

/*
 * \brief Clear interrupt flag
 */
void clear_interrupt_flag(void) {
    // Clear interrupt flag
    EIFR |= _BV(INTF0);
}

/*
 * \brief Initialize LED
 */
void init_LED(void) {
    // Set LED as output
    DDRD |= _BV(RED) | _BV(GREEN);

    // Turn off LED
    PORTD &= ~_BV(RED);
    PORTD &= ~_BV(GREEN);
}

/*
* \brief Initialize timer2
*/
void init_timer2(void) {
    // set CTC mode
    TCCR2A |= _BV(WGM21);

    // compare value to 1ms
    OCR2A = 250;

    // enable interrupt
    TIMSK2 |= _BV(OCIE2A);

    // set prescaler to 64
    TCCR2B |= _BV(CS22) | _BV(CS21); 
}

/*
 * \brief Turn off LED
 */
void turn_off_LED(void) {
    PORTD &= ~_BV(RED); // Turn off red LED
    PORTD &= ~_BV(GREEN); // Turn off green LED
}

/*
 * \brief Sets color of LED based on distance
 */
void set_color(uint16_t distance) {
    if(active) {
        if(distance < 20) {
            PORTD |= _BV(RED); // Turn on red LED
            PORTD &= ~_BV(GREEN); // Turn off green LED
        } else if (distance < 50){
            PORTD &= ~_BV(RED); // Turn off red LED
            PORTD |= _BV(GREEN); // Turn on green LED
        } else {
           turn_off_LED();
        }
    } else {
        turn_off_LED();
    }
}

/*
 * \brief Timer 2 CTC interrupt, used for debouncing button
 */
ISR(TIMER2_COMPA_vect) {
    static uint8_t last_button_state = 0;
    uint8_t current_button_state = (PIND & _BV(BUTTON)) == 0;

    if (button_debounce_count > 0) {
        --button_debounce_count;
        if (button_debounce_count == 0) {
            if (current_button_state != button_state) {
                button_state = current_button_state;
                last_button_state = current_button_state;
            }
        }
    }
}

/*
 * \brief INT0 interrupt, used for button press
 */
ISR(INT0_vect) {
    if(button_debounce_count == 0) {
        active ^= 1;
        button_debounce_count = 100;
    }
}
