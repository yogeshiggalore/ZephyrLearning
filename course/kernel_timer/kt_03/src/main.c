/*
 * Project kt_03: blink two leds, one led with one timer with 730ms and another with 456ms
 * LED Red is connected to pin P0.26 of xiao ble sense board
 * LED App is connected to P0.17 of xiao ble sense board
 * blink red led with 730ms and app led with 456ms
 * Add board folder and create xiao_ble_sense overlay file for app led 
 * Set one Timer with period value of 730ms and duration value set to 5000ms
 * Set another Timer with period value of 456ms and duration value set to 5000ms
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* timer value for red and app leds */
#define LED_RED_PERIOD_VALUE		730
#define LED_RED_DURATION_VALUE		5000

#define LED_APP_PERIOD_VALUE		456
#define LED_APP_DURATION_VALUE		5000

#define LED_MAX_BLINK_COUNT	10

/* devicetree node for red led and app led */
#define NODE_LED_RED	DT_ALIAS( led0 )
#define NODE_LED_APP    DT_ALIAS( appled )

/* get red led and app led dt spec */
static const struct gpio_dt_spec dt_led_red = GPIO_DT_SPEC_GET( NODE_LED_RED, gpios );
static const struct gpio_dt_spec dt_led_app = GPIO_DT_SPEC_GET( NODE_LED_APP, gpios );

/* kernel timer configuration */
struct k_timer led_red_timer;
struct k_timer led_app_timer;

void led_red_timer_expiry_fn( struct k_timer *timer );
void led_red_timer_stop_fn( struct k_timer *timer );
void led_app_timer_expiry_fn( struct k_timer *timer );
void led_app_timer_stop_fn( struct k_timer *timer );

int main(void)
{
    int ret=0;

	/* check led red devicetree readyness */
	if( !gpio_is_ready_dt( &dt_led_red ) )
	{
		return 0;
	}
	else
	{
		/* configure device dt */
		ret = gpio_pin_configure_dt( &dt_led_red, GPIO_OUTPUT_ACTIVE );
		if( ret < 0 )
		{
			return 0;
		}
		else
		{
			/* turn on red led */
			gpio_pin_set_dt( &dt_led_red, true );
		}
	}

	/* check led app devicetree readyness */
	if( !gpio_is_ready_dt( &dt_led_app ) )
	{
		return 0;
	}
	else
	{
		/* configure device dt */
		ret = gpio_pin_configure_dt( &dt_led_app, GPIO_OUTPUT_ACTIVE );
		if( ret < 0 )
		{
			return 0;
		}
		else
		{
			/* turn on app led */
			gpio_pin_set_dt( &dt_led_app, true );
		}
	}

	/* timer init */
	k_timer_init( &led_red_timer, led_red_timer_expiry_fn, led_red_timer_stop_fn );
	k_timer_init( &led_app_timer, led_app_timer_expiry_fn, led_app_timer_stop_fn );

	/* start timer */
	k_timer_start( &led_red_timer, K_MSEC( LED_RED_DURATION_VALUE ), K_MSEC( LED_RED_PERIOD_VALUE ) );
	k_timer_start( &led_app_timer, K_MSEC( LED_APP_DURATION_VALUE ), K_MSEC( LED_APP_PERIOD_VALUE ) );

	return 0;
}

void led_red_timer_expiry_fn( struct k_timer *timer )
{
	gpio_pin_toggle_dt( &dt_led_red );
}

void led_red_timer_stop_fn( struct k_timer *timer )
{
	gpio_pin_set_dt( &dt_led_red, false );
}

void led_app_timer_expiry_fn( struct k_timer *timer )
{
	gpio_pin_toggle_dt( &dt_led_app );
}

void led_app_timer_stop_fn( struct k_timer *timer )
{
	gpio_pin_set_dt( &dt_led_app, false );
}