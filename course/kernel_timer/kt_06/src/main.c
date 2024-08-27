/*
 * Project: LED_04 -> blink different colors using rgb led with firmware delay of 500ms
 * LED Red connected to P0.26 of xiao ble sense board
 * LED Green connected to P0.30 of xiao ble sense board
 * LED Blue connected to P0.06 of xiao ble sense board
*/

/* include headers */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* timer value 500ms */
#define PERIOD_VALUE		500
#define DURATION_VALUE		500

/* define rgb led nodes */
#define NODE_LED_RED	DT_ALIAS( led0 )
#define NODE_LED_GREEN	DT_ALIAS( led1 )
#define NODE_LED_BLUE	DT_ALIAS( led2 )

/* get device dt spec  */
static const struct gpio_dt_spec dt_led_red = GPIO_DT_SPEC_GET( NODE_LED_RED, gpios );
static const struct gpio_dt_spec dt_led_green = GPIO_DT_SPEC_GET( NODE_LED_GREEN, gpios );
static const struct gpio_dt_spec dt_led_blue = GPIO_DT_SPEC_GET( NODE_LED_BLUE, gpios );

enum RGB_LED
{
    LED_BLACK=0,
	LED_RED,
	LED_GREEN,
	LED_BLUE,
    LED_AMBER,
    LED_PINK,
    LED_UNKOWN1,
    LED_UNKOWN2,
    LED_WHITE,
};

/* kernel timer configuration */
struct k_timer led_timer;
void led_timer_expiry_fn( struct k_timer *timer );
void led_timer_stop_fn( struct k_timer *timer );

uint8_t current_led=LED_BLACK;

/* configure gpio pins */
int configure_rgb_led( const struct gpio_dt_spec *spec );

void rgb_turn_on_led( int led );

int main(void)
{
	configure_rgb_led( &dt_led_red );
	configure_rgb_led( &dt_led_green );
	configure_rgb_led( &dt_led_blue );

	/* timer init */
	k_timer_init( &led_timer, led_timer_expiry_fn, led_timer_stop_fn );

	/* start timer */
	k_timer_start( &led_timer, K_MSEC( DURATION_VALUE ), K_MSEC( PERIOD_VALUE ) );

	while( true )
	{

	}
	
    return 0;
}

int configure_rgb_led( const struct gpio_dt_spec *spec )
{
	int status=0;

	if( !gpio_is_ready_dt( spec ) )
	{
		status = -1; 
	}

	if( status == 0 )
	{
		status = gpio_pin_configure_dt( spec, GPIO_OUTPUT_ACTIVE );
	}

	if( status == 0 )
	{
		gpio_pin_set_dt( spec, false );
	} 

	return status;
}

void rgb_turn_on_led( int led )
{
	switch ( led )
	{
	case LED_BLACK:
		gpio_pin_set_dt( &dt_led_red, false );
		gpio_pin_set_dt( &dt_led_green, false );
		gpio_pin_set_dt( &dt_led_blue, false );
		break;

        case LED_RED:
		gpio_pin_set_dt( &dt_led_red, true );
		gpio_pin_set_dt( &dt_led_green, false );
		gpio_pin_set_dt( &dt_led_blue, false );
		break;
	
	case LED_GREEN:
		gpio_pin_set_dt( &dt_led_red, false );
		gpio_pin_set_dt( &dt_led_green, true );
		gpio_pin_set_dt( &dt_led_blue, false );
		break;

	case LED_BLUE:
		gpio_pin_set_dt( &dt_led_red, false );
		gpio_pin_set_dt( &dt_led_green, false );
		gpio_pin_set_dt( &dt_led_blue, true );
		break;

        case LED_AMBER:
		gpio_pin_set_dt( &dt_led_red, true );
		gpio_pin_set_dt( &dt_led_green, true );
		gpio_pin_set_dt( &dt_led_blue, false );
		break;
        
        case LED_UNKOWN1:
		gpio_pin_set_dt( &dt_led_red, true );
		gpio_pin_set_dt( &dt_led_green, false );
		gpio_pin_set_dt( &dt_led_blue, true );
		break;

        case LED_UNKOWN2:
		gpio_pin_set_dt( &dt_led_red, false );
		gpio_pin_set_dt( &dt_led_green, true );
		gpio_pin_set_dt( &dt_led_blue, true );
		break;

        case LED_WHITE:
		gpio_pin_set_dt( &dt_led_red, true );
		gpio_pin_set_dt( &dt_led_green, true );
		gpio_pin_set_dt( &dt_led_blue, true );
		break;

	default:
		gpio_pin_set_dt( &dt_led_red, false );
		gpio_pin_set_dt( &dt_led_green, false );
		gpio_pin_set_dt( &dt_led_blue, false );
		break;
	}
}

void led_timer_expiry_fn( struct k_timer *timer )
{
	rgb_turn_on_led( current_led );
	current_led++;
	if( current_led > LED_WHITE )
	{
		current_led = LED_BLACK;
	}
}

void led_timer_stop_fn( struct k_timer *timer )
{
	gpio_pin_set_dt( &dt_led_red, false );
	gpio_pin_set_dt( &dt_led_green, false );
	gpio_pin_set_dt( &dt_led_blue, false );
}