#include <avr/io.h>
#include <util/delay.h>

#define BUZZER PD6

void init_buzzer(void) {
    // Set buzzer as output
    DDRD |= _BV(BUZZER);

    // pullup resistor
    PORTD |= _BV(BUZZER);

    // Set fast PWM mode
    TCCR0A |= _BV(WGM01) | _BV(WGM00) | _BV(COM0A1); // Fast PWM, Clear OC0A on compare match, set OC0A at BOTTOM

    TCCR0B |= _BV(CS01) | _BV(CS00); // Set prescaler to 64

    // Set frequency
    OCR0A = 0;
}

void set_freq(uint16_t freq) {
    // Set frequency
    OCR0A = F_CPU / (2 * 8 * freq) - 1;
}

void start_buzzer(void) {
    // Turn on buzzer
    TCCR0B |= _BV(CS01) | _BV(CS00); // Set prescaler to 64
}

void stop_buzzer(void) {
    // Turn off buzzer
    TCCR0B &= ~_BV(CS01) & ~_BV(CS00); // Set prescaler to 0
}

void ping_buzzer(uint8_t time) {
    start_buzzer();
    _delay_ms(time);
    stop_buzzer();
}

void set_volume(uint8_t volume) {
    // Set volume
    OCR0B = (OCR0A * volume) / 100;
}
