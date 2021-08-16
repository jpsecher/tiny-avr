/*
  Connections:

  PD0 --- LCD_DB5
  PD1 --- LCD_DB6
  PD2 --- LCD_DB7
  PD3 --- LCD_E
  PD4 --- LCD_RS
  PD5 --- LCD_DB4
*/

// It is important that these four data pins are in this order
#define D_LCD_DB4 PD0
#define D_LCD_DB5 PD1
#define D_LCD_DB6 PD2
#define D_LCD_DB7 PD3

#define D_LCD_RS PD4
#define D_LCD_EN PD5

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void lcd_init(void);
INLINE void lcd_setCursor(uint8_t col, uint8_t row);
INLINE void lcd_write4bits (uint8_t value);
INLINE void lcd_command (uint8_t value);
INLINE void lcd_write (uint8_t value);
INLINE void lcd_clear (void);
INLINE void lcd_display (void);
INLINE void lcd_solid_cursor (void);
INLINE void lcd_blink_cursor();
INLINE void lcd_invisible_cursor();
INLINE void lcd_pulse_enable (void);
INLINE void all_pin_Ds_as_output (void);
INLINE void pin_on_D (uint8_t pin);
INLINE void pin_off_D (uint8_t pin);

void lcd_print (char const *);
void lcd_create_character (uint8_t location, uint8_t const * map);

void lcd_create_heart (void) {
  uint8_t raster[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
  };
  lcd_create_character(0, raster);
}

void lcd_create_oe (void) {
  uint8_t raster[8] = {
    0b00000,
    0b00000,
    0b01110,
    0b10011,
    0b10101,
    0b11001,
    0b01110,
    0b00000
  };
  lcd_create_character(1, raster);
}

void lcd_create_ae (void) {
  uint8_t raster[8] = {
    0b00000,
    0b00000,
    0b11010,
    0b00101,
    0b01110,
    0b10100,
    0b01011,
    0b00000
  };
  lcd_create_character(2, raster);
}

int main (void) {
  all_pin_Ds_as_output();
  lcd_init();
  lcd_clear();
  lcd_create_heart();
  lcd_create_oe();
  lcd_create_ae();
  lcd_setCursor(0, 0);
  lcd_print("  Du er l\2kker");
  lcd_setCursor(0, 1);
  lcd_print("    og s\1d");
  lcd_setCursor(11, 1);
  lcd_write(0);
  lcd_setCursor(15, 1);
  while (1) {
    lcd_solid_cursor();
    _delay_ms(2000);
    lcd_blink_cursor();
    _delay_ms(2000);
    lcd_invisible_cursor();
    _delay_ms(2000);
  }
  return 0;
}

void lcd_print (char const * str) {
  while (*str) {
    lcd_write(*str);
    ++str;
  }
}

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYLEFT 0x02

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_CURSORON 0x02
#define LCD_BLINKON 0x01

// flags for function set
#define LCD_2LINE 0x08

void lcd_init (void) {
  _delay_ms(50);
  // Prepare command write
  pin_off_D(D_LCD_RS);
  pin_off_D(D_LCD_EN);
  // Get into predictable state
  lcd_write4bits(0x03);
  _delay_us(4500); // wait min 4.1ms
  lcd_write4bits(0x03);
  _delay_us(4500); // wait min 4.1ms
  lcd_write4bits(0x03);
  _delay_us(150);
  // 4-bit mode
  lcd_write4bits(0x02);
  // 2 lines, 8x5 font
  lcd_command(LCD_FUNCTIONSET | LCD_2LINE);
  lcd_display();
  lcd_clear();
  // Initialize to default text direction (for romance languages)
  lcd_command(LCD_ENTRYMODESET | LCD_ENTRYLEFT);
}

// Allows us to fill the first 8 CGRAM locations with custom characters
void lcd_create_character (uint8_t location, uint8_t const * map) {
  location &= 0x7; // we only have 8 locations 0-7
  lcd_command(LCD_SETCGRAMADDR | (location << 3));
  for (uint8_t i = 0; i < 8; ++i)
    lcd_write(map[i]);
}

void lcd_setCursor (uint8_t col, uint8_t row)
{
  uint8_t row_offset = row ? 0x40 : 0x00;
  lcd_command(LCD_SETDDRAMADDR | (col + row_offset));
}

void lcd_blink_cursor (void) {
  lcd_command(LCD_DISPLAYCONTROL | LCD_2LINE | LCD_DISPLAYON | LCD_BLINKON);
}

void lcd_solid_cursor (void) {
  lcd_command(LCD_DISPLAYCONTROL | LCD_2LINE | LCD_DISPLAYON | LCD_CURSORON);
}

void lcd_invisible_cursor (void) {
  lcd_command(LCD_DISPLAYCONTROL | LCD_2LINE | LCD_DISPLAYON);
}

void lcd_display (void) {
  // Turn the display on with no cursor or blinking
  lcd_command(LCD_DISPLAYCONTROL | LCD_2LINE | LCD_DISPLAYON);
}

void lcd_clear (void) {
  lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}

void lcd_command (uint8_t value) {
  pin_off_D(D_LCD_RS);
  lcd_write4bits(value>>4);
  lcd_write4bits(value);
}

void lcd_write (uint8_t value) {
  pin_on_D(D_LCD_RS);
  lcd_write4bits(value>>4);
  lcd_write4bits(value);
}

void lcd_write4bits (uint8_t value) {
  PORTD = (PORTD & 0xF0) | (value & 0x0F);
  lcd_pulse_enable();
}

void lcd_pulse_enable (void) {
  pin_off_D(D_LCD_EN);
  _delay_us(1);  // >140 ns
  pin_on_D(D_LCD_EN);
  _delay_us(1);  // enable pulse must be >450ns
  pin_off_D(D_LCD_EN);
  _delay_us(100);  // commands need > 37us to settle
}

void all_pin_Ds_as_output (void) {
  DDRD |= 0b11111111;
}

void pin_on_D (uint8_t pin) {
  PORTD |= _BV(pin);
}

void pin_off_D (uint8_t pin) {
  PORTD &= ~_BV(pin);
}
