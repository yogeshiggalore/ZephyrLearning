/*
 * Project: LED_02 -> blink two leds, one led with firmware delay of 730ms and another with 456ms.
 * LED Red is connected to P0.26 of xiao ble sense board
 * LED App is connected to P0.17 of xiao ble sense board
 * LED Red will blink with 730 ms delay
 * LED App will blink with 456 ms delay
 * Add board folder and create xiao_ble_sense overlay file for app led 
*/

/* include required header files */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED_RED_DELAY_TIME_MS		730
#define LED_APP_DELAY_TIME_MS		456

/* define LED Red and app */
#define NODE_LED_RED    DT_ALIAS(led0)
#define NODE_LED_APP    DT_ALIAS(appled)

/* get device dt spec */
static const struct gpio_dt_spec dt_led_red = GPIO_DT_SPEC_GET( NODE_LED_RED, gpios );
static const struct gpio_dt_spec dt_led_app = GPIO_DT_SPEC_GET( NODE_LED_APP, gpios );

/* time counter */
uint16_t time_led_red_cntr=0;
uint16_t time_led_app_cntr=0;

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

	while( true )
	{
		/* check if sleep time over for red led */
		if( time_led_red_cntr > LED_RED_DELAY_TIME_MS )
		{
			time_led_red_cntr = 0;
			/* toggle red led pin */
			ret = gpio_pin_toggle_dt( &dt_led_red );
			if( ret < 0 )
			{
				return 0;
			}
		}

		if( time_led_app_cntr > LED_APP_DELAY_TIME_MS )
		{
			time_led_app_cntr = 0;
			/* toggle app led pin */
			ret = gpio_pin_toggle_dt( &dt_led_app );
			if( ret < 0 )
			{
				return 0;
			}
		}

		k_msleep(1);
		time_led_red_cntr++;
		time_led_app_cntr++;
	}

	return 0;
}
