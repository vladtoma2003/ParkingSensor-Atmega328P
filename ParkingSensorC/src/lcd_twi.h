#include "twi.h"
#include "util/delay.h"

#define LCD_ADDR 0x27
#define WRITE 0x00
#define READ 0x01

unsigned char lcd = 0x00;

// inspiration: https://github.com/gpg/wk-misc/blob/master/ebus/i2c-lcd.c

/*
 * \brief Write in module
 */
void module_write(unsigned char data) {
    twi_start();
    twi_write_cmd((LCD_ADDR << 1) | WRITE);
    twi_write_data(data);
    twi_stop();

}

/*
 * \brief Write 4-bit data to LCD
 */
void lcd_4bit_write(unsigned char data) {
    unsigned char temp = 0x00;

    // Write high nibble
    lcd &= 0x0F; // Clear data
    temp = (data & 0xF0); // Get high nibble
    lcd |= temp; // Set high nibble
    lcd |= 0x04; // Set enable
    module_write(lcd); // Write data
    _delay_us(1); // Delay
    lcd &= ~(0x04); // Clear enable
    module_write(lcd); // Write data
    _delay_us(5); // Delay

    // Write low nibble
    temp = ((data & 0x0F) << 4); // Get low nibble
    lcd &= 0x0F; // Clear data
    lcd |= temp; // Set low nibble
    lcd |= 0x04; // Set enable
    module_write(lcd); // Write data
    _delay_us(1); // Delay
    lcd &= ~(0x04); // Clear enable
    module_write(lcd); // Write data
    _delay_us(5); // Delay
}

/*
 * \brief Write command to LCD
 */
void lcd_cmd(unsigned char cmd) {
    lcd = 0x08; // Clear RS
    lcd &= ~(0x01); // Clear RW
    module_write(lcd); // Write data
    lcd_4bit_write(cmd); // Write command
}

/*
 * \brief Initialize LCD
 */
void lcd_init(void) {
    lcd = 0x04; // Set backlight on
    module_write(lcd);
    _delay_us(50);

    lcd_cmd(0x03); // Function set
    lcd_cmd(0x03); // Function set
    lcd_cmd(0x03); // Function set
    lcd_cmd(0x02); // Function set
    lcd_cmd(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcd_cmd(0x0F); // Display on, cursor on, blink on
    lcd_cmd(0x01); // Clear display
    lcd_cmd(0x06); // Entry mode, increment
    lcd_cmd(0x80); // Set cursor to 0, 0
}

/*
 * \brief Write character to LCD
 */
void lcd_clear(void) {
    lcd_cmd(0x01); // Clear display
}

/*
 * \brief Write character to LCD
 */
void lcd_set_cursor(unsigned char row, unsigned char col) {
    unsigned char address = 0x00;

    if (row == 0) {
        address = 0x00;
    } else if (row == 1) {
        address = 0x40;
    } else if (row == 2) {
        address = 0x14;
    } else if (row == 3) {
        address = 0x54;
    }

    address += col;
    lcd_cmd(0x80 | address);
}

/*
 * \brief Write character to LCD
 */
void lcd_write_cmd(unsigned char addr) {
    TWDR = addr;
    TWCR = _BV(TWINT) | _BV(TWEN); // Clear interrupt flag and enable TWI
    while (!(TWCR & _BV(TWINT))); // Wait for TWI to finish
    while(TW_STATUS != TW_MT_DATA_ACK); // Wait for ACK
}

/*
 * \brief Write character to LCD
 */
void lcd_write_data(unsigned char data) {
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN); // Clear interrupt flag and enable TWI
    while (!(TWCR & _BV(TWINT))); // Wait for TWI to finish
    while(TW_STATUS != TW_MT_DATA_ACK); // Wait for ACK
}

void lcd_dwrite(unsigned char data) {
    lcd |= 0x09; // Backlight on
    module_write(lcd); // Write data
    lcd_4bit_write(data); // Write data
}

/*
 * \brief Write character to LCD
 */
void lcd_write(unsigned char *data) {
    while(*data != '\0') lcd_dwrite(*data++);
}

char *number_to_string(uint8_t number) {
    static char str[10];
    sprintf(str, "%d", number);
    return str;

}

void lcd_write_number(uint8_t number) {
    lcd_write(number_to_string(number));
}

lcd_clear_line(uint8_t row) {
    lcd_set_cursor(row, 0);
    lcd_write("                ");
}
