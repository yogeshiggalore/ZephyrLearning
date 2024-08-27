/*
 * Project: I2C_01 -> Scan I2C devices in ports
 * xioa ble sense board has two i2c port i2c0 and i2c1
 * config i2c in prj to enable i2c driver
*/

/* include required header here */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include "ssd1306.h"

#define MAX_I2C_ADDRESS 127
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define NUMFLAKES     10 // Number of snowflakes in the animation example

static const unsigned char logo_bmp[] =
{ 
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 
};


/* device for two i2c port */
struct device *dev_i2c0;

int get_device_binding_i2c0(void);

void scan_i2c_port(const struct device *i2c_port);

void oled_write_command( const struct device *i2c_port, uint8_t i2c_addr, uint8_t cmd);
void oled_write_command_list( const struct device *i2c_port, uint8_t i2c_addr, const uint8_t *c, uint8_t n);
void test_oled(void);

int main()
{
    k_msleep( 1000 );

	printk("getting device binding for i2c0 and i2c1\n");
	get_device_binding_i2c0();

	scan_i2c_port( dev_i2c0 );
	k_msleep(5000);

	test_oled();

	while( true )
	{
		//oled_write_command(dev_i2c0, 0x3C, SSD1306_INVERTDISPLAY);
		k_msleep(500);
		//oled_write_command(dev_i2c0, 0x3C, SSD1306_NORMALDISPLAY);
		k_msleep(500);
	}
	return 1;
}


int get_device_binding_i2c0(void)
{
	int ret=0;

	dev_i2c0 = DEVICE_DT_GET( DT_NODELABEL(i2c0) );

	if( dev_i2c0 == NULL )
	{
		printk("device binding to port i2c0 failed. ");
		ret = -1;
	}
	else
	{
		if( !device_is_ready( dev_i2c0 ) )
		{
			printk(" device i2c0 is not ready yet\n ");
			ret = -1;
		}
	}

	return ret;
}

void scan_i2c_port(const struct device *i2c_port)
{
	uint8_t data;
	uint8_t i2c_addr;
	uint8_t status;
	uint8_t number_of_bytes;

	struct i2c_msg i2cmsg;

	i2cmsg.buf = &data;
	i2cmsg.len = 0;
	i2cmsg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	number_of_bytes = 1;

	printk("started scanning i2c devices\n\n");
	printk("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n00:    ");
	for(i2c_addr=1; i2c_addr<= MAX_I2C_ADDRESS; i2c_addr++)
	{
		if((i2c_addr % 0x10) == 0)	
		{
			printk("\n%2x: ",i2c_addr);
		}

		status = i2c_transfer(i2c_port, &i2cmsg, number_of_bytes, i2c_addr);

		if(status == 0)
		{
			printk("%2x ",i2c_addr);
		}
		else
		{
			printk("-- ");
		}

		k_sleep(K_MSEC(100));	
	}
	printk("\n\nstopped scanning i2c devices\n");
}

void oled_write_command( const struct device *i2c_port, uint8_t i2c_addr, uint8_t cmd)
{
	uint8_t data[2];
	uint8_t status;
	uint8_t number_of_bytes;

	struct i2c_msg i2cmsg;

	data[0] = 0; //Command is 0 and data/control should be 1
	data[1] = cmd; 

	i2cmsg.buf = &data;
	i2cmsg.len = 2;
	i2cmsg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	number_of_bytes = 1;
	status = i2c_transfer(i2c_port, &i2cmsg, number_of_bytes, i2c_addr);
	if(status == 0)
	{
		printk("yes\n");
	}
	else
	{
		printk("no\n");
	}
}

void oled_write_command_list( const struct device *i2c_port, uint8_t i2c_addr, const uint8_t *c, uint8_t n) 
{
	uint8_t data[n+1];
	uint8_t status;
	uint8_t number_of_bytes;

	struct i2c_msg i2cmsg;

	data[0] = 0; //Command is 0 and data/control should be 1
	for( int i=1; i<= n; i++ )
	{
		data[i] = *c;
		c++;
	}
	
	i2cmsg.buf = &data;
	i2cmsg.len = 2;
	i2cmsg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	number_of_bytes = 1;
	status = i2c_transfer(i2c_port, &i2cmsg, number_of_bytes, i2c_addr);
	if(status == 0)
	{
		printk("list yes\n");
	}
	else
	{
		printk("list no\n");
	}
}

void testdrawline() {
  int16_t i;

  ssd1306_clearDisplay(); // Clear display buffer

  for(i=0; i<WIDTH; i+=4) {
    ssd1306_drawLine(0, 0, i, HEIGHT-1, SSD1306_WHITE);
    ssd1306_display(); // Update screen with each newly-drawn line
    k_msleep(1);
  }
  for(i=0; i<HEIGHT; i+=4) {
    ssd1306_drawLine(0, 0, WIDTH-1, i, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }
  k_msleep(250);

  ssd1306_clearDisplay();

  for(i=0; i<WIDTH; i+=4) {
    ssd1306_drawLine(0, HEIGHT-1, i, 0, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }
  for(i=HEIGHT-1; i>=0; i-=4) {
    ssd1306_drawLine(0, HEIGHT-1, WIDTH-1, i, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }
  k_msleep(250);

  ssd1306_clearDisplay();

  for(i=WIDTH-1; i>=0; i-=4) {
    ssd1306_drawLine(WIDTH-1, HEIGHT-1, i, 0, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }
  for(i=HEIGHT-1; i>=0; i-=4) {
    ssd1306_drawLine(WIDTH-1, HEIGHT-1, 0, i, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }
  k_msleep(250);

  ssd1306_clearDisplay();

  for(i=0; i<HEIGHT; i+=4) {
    ssd1306_drawLine(WIDTH-1, 0, 0, i, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }
  for(i=0; i<WIDTH; i+=4) {
    ssd1306_drawLine(WIDTH-1, 0, i, HEIGHT-1, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }

  k_msleep(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  ssd1306_clearDisplay();

  for(int16_t i=0; i<HEIGHT/2; i+=2) {
    //ssd1306_drawRect(i, i, WIDTH-2*i, HEIGHT-2*i, SSD1306_WHITE);
    ssd1306_display(); // Update screen with each newly-drawn rectangle
    k_msleep(1);
  }

  k_msleep(2000);
}

void testfillrect(void) {
  ssd1306_clearDisplay();

  for(int16_t i=0; i<HEIGHT/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    ssd1306_fillRect(i, i, WIDTH-i*2, HEIGHT-i*2, SSD1306_INVERSE);
    ssd1306_display(); // Update screen with each newly-drawn rectangle
    k_msleep(1);
  }

  k_msleep(2000);
}

void testdrawcircle(void) {
  ssd1306_clearDisplay();

  for(int16_t i=0; i<WIDTH/2; i+=2) {
    ssd1306_drawCircle(WIDTH/2, HEIGHT/2, i, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }

  k_msleep(2000);
}

void testfillcircle(void) {
  ssd1306_clearDisplay();

  for(int16_t i=WIDTH/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    ssd1306_fillCircle(WIDTH / 2, HEIGHT / 2, i, SSD1306_INVERSE);
    ssd1306_display(); // Update screen with each newly-drawn circle
    k_msleep(1);
  }

  k_msleep(2000);
}

void testdrawroundrect(void) {
  ssd1306_clearDisplay();

  for(int16_t i=0; i<HEIGHT/2-2; i+=2) {
    ssd1306_drawRoundRect(i, i, WIDTH-2*i, HEIGHT-2*i,
      HEIGHT/4, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }

  k_msleep(2000);
}

void testfillroundrect(void) {
  ssd1306_clearDisplay();

  for(int16_t i=0; i<HEIGHT/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    ssd1306_fillRoundRect(i, i, WIDTH-2*i, HEIGHT-2*i,
      HEIGHT/4, SSD1306_INVERSE);
    ssd1306_display();
    k_msleep(1);
  }

  k_msleep(2000);
}

void testdrawtriangle(void) {
  ssd1306_clearDisplay();

  for(int16_t i=0; i<WIDTH/2; i+=5) {
    ssd1306_drawTriangle(
      WIDTH/2  , HEIGHT/2-i,
      WIDTH/2-i, HEIGHT/2+i,
      WIDTH/2+i, HEIGHT/2+i, SSD1306_WHITE);
    ssd1306_display();
    k_msleep(1);
  }

  k_msleep(2000);
}

void testfilltriangle(void) {
  ssd1306_clearDisplay();

  for(int16_t i=WIDTH/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    ssd1306_fillTriangle(
      WIDTH/2  , HEIGHT/2-i,
      WIDTH/2-i, HEIGHT/2+i,
      WIDTH/2+i, HEIGHT/2+i, SSD1306_INVERSE);
    ssd1306_display();
    k_msleep(1);
  }

  k_msleep(2000);
}

void testdrawchar(void) {
  ssd1306_clearDisplay();

  ssd1306_setTextSize(1);      // Normal 1:1 pixel scale
  ssd1306_setTextColor(SSD1306_WHITE); // Draw white text
  ssd1306_setCursor(0, 0);     // Start at top-left corner
  ssd1306_cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the ssd1306_ This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') ssd1306_write(' ');
    else          ssd1306_write(i);
  }

  ssd1306_display();
  k_msleep(2000);
}

void testdrawstyles(void) {
  ssd1306_clearDisplay();

  ssd1306_setTextSize(1);             // Normal 1:1 pixel scale
  ssd1306_setTextColor(SSD1306_WHITE);        // Draw white text
  ssd1306_setCursor(0,0);             // Start at top-left corner
  ssd1306_println(F("Hello, world!"));

  ssd1306_setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  ssd1306_println(3.141592);

  ssd1306_setTextSize(2);             // Draw 2X-scale text
  ssd1306_setTextColor(SSD1306_WHITE);
  ssd1306_print(F("0x")); ssd1306_println("%X",0xDEADBEEF);

  ssd1306_display();
  k_msleep(2000);
}

void testscrolltext(void) {
  ssd1306_clearDisplay();

  ssd1306_setTextSize(2); // Draw 2X-scale text
  ssd1306_setTextColor(SSD1306_WHITE);
  ssd1306_setCursor(10, 0);
  ssd1306_println(F("scroll"));
  ssd1306_display();      // Show initial text
  k_msleep(100);

  // Scroll in various directions, pausing in-between:
  ssd1306_startscrollright(0x00, 0x0F);
  k_msleep(2000);
  ssd1306_stopscroll();
  k_msleep(1000);
  ssd1306_startscrollleft(0x00, 0x0F);
  k_msleep(2000);
  ssd1306_stopscroll();
  k_msleep(1000);
  ssd1306_startscrolldiagright(0x00, 0x07);
  k_msleep(2000);
  ssd1306_startscrolldiagleft(0x00, 0x07);
  k_msleep(2000);
  ssd1306_stopscroll();
  k_msleep(1000);
}

void testdrawbitmap(void) {
  ssd1306_clearDisplay();

  ssd1306_drawBitmap(
    (WIDTH  - LOGO_WIDTH ) / 2,
    (HEIGHT - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  ssd1306_display();
  k_msleep(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, WIDTH);
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    printk(F("x: "));
    printk("%d",icons[f][XPOS]);
    printk(F(" y: "));
    printk("%d", icons[f][YPOS]);
    printk(F(" dy: "));
    printk("%d\n",icons[f][DELTAY]);
  }

  for(;;) { // Loop forever...
    ssd1306_clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      ssd1306_drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    ssd1306_display(); // Show the display buffer on the screen
    k_msleep(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= HEIGHT) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, WIDTH);
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}

void test_oled(void)
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(ssd1306_begin(SSD1306_SWITCHCAPVCC, 0x3C, true, true)) {
    printk(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  ssd1306_display();
  k_msleep(2000); // Pause for 2 seconds

  // Clear the buffer
  ssd1306_clearDisplay();

  // Draw a single pixel in white
  ssd1306_drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  ssd1306_display();
  k_msleep(2000);
  // ssd1306_display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // ssd1306_display(). These examples demonstrate both approaches...

  testdrawline();      // Draw many lines

  testdrawrect();      // Draw rectangles (outlines)

  testfillrect();      // Draw rectangles (filled)

  testdrawcircle();    // Draw circles (outlines)

  testfillcircle();    // Draw circles (filled)

  testdrawroundrect(); // Draw rounded rectangles (outlines)

  testfillroundrect(); // Draw rounded rectangles (filled)

  testdrawtriangle();  // Draw triangles (outlines)

  testfilltriangle();  // Draw triangles (filled)

  testdrawchar();      // Draw characters of the default font

  testdrawstyles();    // Draw 'stylized' characters

  testscrolltext();    // Draw scrolling text

  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  ssd1306_invertDisplay(true);
  k_msleep(1000);
  ssd1306_invertDisplay(false);
  k_msleep(1000);

  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
}
