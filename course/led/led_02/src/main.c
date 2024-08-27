/*
 * Project: LED_02 -> blink two leds, one led with firmware delay of 1000ms and another with 100ms.
 * LED Red is connected to P0.26 of xiao ble sense board
 * LED App is connected to P0.17 of xiao ble sense board
 * LED Red will blink with 1000 ms delay
 * LED App will blink with 100 ms delay
 * Add board folder and create xiao_ble_sense overlay file for app led 
*/

/* include required header files */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED_RED_DELAY_TIME_MS		1000
#define LED_APP_DELAY_TIME_MS		100

/* define LED Red and app */
#define NODE_LED_RED    DT_ALIAS(led0)
#define NODE_LED_APP    DT_ALIAS(appled)

/* get device dt spec */
static const struct gpio_dt_spec dt_led_red = GPIO_DT_SPEC_GET( NODE_LED_RED, gpios );
static const struct gpio_dt_spec dt_led_app = GPIO_DT_SPEC_GET( NODE_LED_APP, gpios );

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
		
		for( int i=0; i<10; i++ )
		{
			ret = gpio_pin_toggle_dt( &dt_led_app );
			if( ret < 0 )
			{
				return 0;
			}

			k_msleep(LED_APP_DELAY_TIME_MS);
		}

		ret = gpio_pin_toggle_dt( &dt_led_red );
		if( ret < 0 )
		{
			return 0;
		}
	}

	return 0;
}
