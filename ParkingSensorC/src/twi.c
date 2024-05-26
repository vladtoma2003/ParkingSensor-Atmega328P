#include "twi.h"

void twi_init(void) {
    // SDA on A4 and SCL on A5
    DDRC |= _BV(PC5); // Set SCL as output

    PORTC |= _BV(PC5); // Set SCL high

    TWCR = 0; // reset TWI control register

    TWBR = TWBR_VAL; // Set bitrate
    TWSR &= ~(_BV(TWPS0) | _BV(TWPS1)); // Set prescaler to 1
}

void twi_start(void) {
    // Send START condition
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)));
}

void twi_write(uint8_t data) {
    // Write data to TWI
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)));
}

void twi_read_ack(uint8_t *data) {
    // Read data from TWI with ACK
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
    while (!(TWCR & _BV(TWINT)));
    *data = TWDR;
}

void twi_read_nack(uint8_t *data) {
    // Read data from TWI without ACK
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)));
    *data = TWDR;
}

void twi_stop(void) {
    // Send STOP condition
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

void twi_discover(void) {
    // Search for slave devices
    for (uint8_t i = 0; i < 128; i++) {
        twi_start();
        twi_write(i << 1);
        if (TWSR == 0x18) {
            // Slave found
            printf("Slave found at address 0x%02X\n", i);
        }
        twi_stop();
    }
}

void twi_write_cmd(uint8_t addr) {
    TWDR = addr; // Write command to TWI
    TWCR = _BV(TWINT) | _BV(TWEN); // Clear interrupt flag and enable TWI
    while (!(TWCR & _BV(TWINT))); // Wait for TWI to finish
    while(TW_STATUS != TW_MT_SLA_ACK); // Wait for ACK
}

void twi_write_data(uint8_t data) {
    TWDR = data; // Write data to TWI
    TWCR = _BV(TWINT) | _BV(TWEN); // Clear interrupt flag and enable TWI
    while (!(TWCR & _BV(TWINT))); // Wait for TWI to finish
    while(TW_STATUS != TW_MT_DATA_ACK); // Wait for ACK
}
