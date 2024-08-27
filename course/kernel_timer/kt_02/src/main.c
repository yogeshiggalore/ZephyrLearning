/*
 * Project kt_02: blink red led 10 times with 500ms timer value.
 * LED Red is connected to pin P0.26 of xiao ble sense board
 * Timer period value set to 500ms and duration value set to 5000ms
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* timer value 500ms */
#define PERIOD_VALUE		500
#define DURATION_VALUE		5000

#define LED_MAX_BLINK_COUNT	10

/* devicetree node for red led */
#define NODE_RED_LED	DT_ALIAS( led0 )

/* get red led dt spec */
static const struct gpio_dt_spec dt_red_led = GPIO_DT_SPEC_GET( NODE_RED_LED, gpios );

/* kernel timer configuration */
struct k_timer led_timer;
void led_timer_expiry_fn( struct k_timer *timer );
void led_timer_stop_fn( struct k_timer *timer );

uint8_t led_blink_counter=0;

int main(void)
{
    int ret=0;
	
	/* check device tree readiness */
	if( !gpio_is_ready_dt( &dt_red_led ) )
	{
		return 0;
	}

	/* if device is ready configure the red led as output pin */
	ret = gpio_pin_configure_dt( &dt_red_led, GPIO_OUTPUT_ACTIVE );
	if( ret < 0 )
	{
		return 0;
	}

	/* turn off red led ( active high ) */
	gpio_pin_set_dt( &dt_red_led, false );

	/* timer init */
	k_timer_init( &led_timer, led_timer_expiry_fn, led_timer_stop_fn );

	/* start timer */
	k_timer_start( &led_timer, K_MSEC( DURATION_VALUE ), K_MSEC( PERIOD_VALUE ) );

	return 0;
}

void led_timer_expiry_fn( struct k_timer *timer )
{
	/* toggle red led pin */
	gpio_pin_toggle_dt( &dt_red_led );

	led_blink_counter++;

	if( led_blink_counter > ( LED_MAX_BLINK_COUNT * 2 ) )
	{
		led_blink_counter=0;
		k_timer_stop( &led_timer );
	}
}

void led_timer_stop_fn( struct k_timer *timer )
{
	gpio_pin_set_dt( &dt_red_led, false );
}