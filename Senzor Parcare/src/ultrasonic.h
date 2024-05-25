#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define TRIG PB1
#define ECHO1 PB2
#define ECHO2 PB3

volatile turn = 0;

/*
 * \brief Initialize ultrasonic sensor
 */
void init_sensor(void) {
    // Set trigger as output
    DDRB |= _BV(TRIG);

    // Set echo as input
    DDRB &= ~(_BV(ECHO1) | _BV(ECHO2));

    // Set INT0 as falling edge
    EICRA |= _BV(ISC01);
    EICRA &= ~_BV(ISC00);

    // Enable INT0
    EIMSK |= _BV(INT0);

    // Set prescaler to 8
    TCCR1B |= _BV(CS11);
}

/*
 * \brief Send trigger signal
 */
void ultrasonic_trigger(void) {
    // Send trigger signal
    PORTB |= _BV(TRIG);
    _delay_us(10);
    PORTB &= ~_BV(TRIG); 
}

/*
 * \brief Calculate distance
 */
uint16_t ultrasonic_calculate_distance(void) {
    if(turn == 0) {
        while(!(PINB & _BV(ECHO1)));
        TCNT1 = 0;

        TCCR1B |= _BV(CS11);

        while(PINB & _BV(ECHO1));
    } else {
        while(!(PINB & _BV(ECHO2)));
        TCNT1 = 0;

        TCCR1B |= _BV(CS11);

        while(PINB & _BV(ECHO2));
    }
        TCCR1B &= ~_BV(CS11);
        return TCNT1 / 58;
}
