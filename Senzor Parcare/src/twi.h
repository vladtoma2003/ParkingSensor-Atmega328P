#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <stdio.h>
#include "usart.h"

#define F_SCL 100000UL
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16) / (2 * PRESCALER))

/**
 * \brief Initialize TWI by setting the bitrate
*/
void twi_init(void);

/**
 * \brief Transmit START condition
*/
void twi_start(void);

/**
 * \brief Write a byte of data
*/
void twi_write(uint8_t data);

/**
 * \brief Read a byte of data with ACK enabled
*/
void twi_read_ack(uint8_t *data);

/**
 * \brief Read a byte of data without ACK
*/
void twi_read_nack(uint8_t *data);

/**
 * \brief Transmit STOP condition
*/
void twi_stop(void);

/**
 * \brief Search for slave devices and print them to serial
*/
void twi_discover(void);

/**
 * \brief Write a command to a slave device(LCD)
*/
void twi_write_cmd(uint8_t addr);

/**
 * \brief Write data to a slave device(LCD)
*/
void twi_write_data(uint8_t data);
