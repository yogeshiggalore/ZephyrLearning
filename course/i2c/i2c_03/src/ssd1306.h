#ifndef _SSD1306_H_
#define _SSD1306_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#define SSD1306_128_32
// #define SSD1306_96_16

/// The following "raw" color names are kept for backwards client compatability
/// They can be disabled by predefining this macro before including the Adafruit
/// header client code will then need to be modified to use the scoped enum
/// values directly
// #ifndef NO_ADAFRUIT_SSD1306_COLOR_COMPATIBILITY
// #define BLACK SSD1306_BLACK     ///< Draw 'off' pixels
// #define WHITE SSD1306_WHITE     ///< Draw 'on' pixels/
// #define INVERSE SSD1306_INVERSE ///< Invert pixels
// #endif

#define HEIGHT 32
#define WIDTH 128

/// fit into the SSD1306_ naming scheme
#define SSD1306_BLACK 0		///< Draw 'off' pixels
#define SSD1306_WHITE 1		///< Draw 'on' pixels
#define SSD1306_INVERSE 2	///< Invert pixels

#define SSD1306_MEMORYMODE 0x20			///< See datasheet
#define SSD1306_COLUMNADDR 0x21			///< See datasheet
#define SSD1306_PAGEADDR 0x22			///< See datasheet
#define SSD1306_SETCONTRAST 0x81		///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D			///< See datasheet
#define SSD1306_SEGREMAP 0xA0			///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5		///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6		///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7		///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8		///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE			///< See datasheet
#define SSD1306_DISPLAYON 0xAF			///< See datasheet
#define SSD1306_COMSCANINC 0xC0			///< Not currently used
#define SSD1306_COMSCANDEC 0xC8			///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3	///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5	///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9		///< See datasheet
#define SSD1306_SETCOMPINS 0xDA			///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB		///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00	///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10	///< Not currently used
#define SSD1306_SETSTARTLINE 0x40	///< See datasheet

#define SSD1306_EXTERNALVCC 0x01	///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02	///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26				///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27					///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29	///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A	///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E						///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F						///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3				///< Set scroll range

#define SSD1306_LCDWIDTH 128///< DEPRECATED: width w/SSD1306_128_32 defined
#define SSD1306_LCDHEIGHT 32///< DEPRECATED: height w/SSD1306_128_32 defined

bool ssd1306_begin( uint8_t switchvcc, uint8_t i2caddr, bool reset, bool periphBegin);
void ssd1306_display( void );
void ssd1306_ssd1306_clearDisplay( void );
void ssd1306_ssd1306_invertDisplay( bool i );
void ssd1306_dim( bool dim );
void ssd1306_drawPixel( int16_t x, int16_t y, uint16_t color );
void ssd1306_drawFastHLine( int16_t x, int16_t y, int16_t w, uint16_t color );
void ssd1306_drawFastVLine( int16_t x, int16_t y, int16_t h, uint16_t color );
void ssd1306_startscrollright( uint8_t start, uint8_t stop );
void ssd1306_startscrollleft( uint8_t start, uint8_t stop );
void ssd1306_startscrolldiagright( uint8_t start, uint8_t stop );
void ssd1306_startscrolldiagleft( uint8_t start, uint8_t stop );
void ssd1306_stopscroll( void );
void ssd1306_ssd1306_command( uint8_t c );
bool ssd1306_getPixel( int16_t x, int16_t y );
uint8_t* ssd1306_getBuffer( void );
void ssd1306_drawFastHLineInternal( int16_t x, int16_t y, int16_t w, uint16_t color );
void ssd1306_drawFastVLineInternal( int16_t x, int16_t y, int16_t h, uint16_t color );
void ssd1306_ssd1306_command1( uint8_t c );
void ssd1306_ssd1306_commandList( const uint8_t* c, uint8_t n );

#endif // __H_
