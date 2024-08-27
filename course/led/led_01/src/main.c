/*
 * Project led-01 : Blink red led with firmware delay of 500ms
 * LED Red is connected to pin P0.26 of xiao ble sense board
 * 
 */

/* include zephyr kernel */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* firmware delay time */
#define SLEEP_TIME_MS   500

/* device tree node identifier for led0 red led */
#define RED_LED_NODE    DT_ALIAS( led0 )

/* get red led device dt spec */
static const struct gpio_dt_spec dt_red_led = GPIO_DT_SPEC_GET( RED_LED_NODE, gpios );

int main(void)
{
    int ret;

	/* check devicetree readyness */
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
	gpio_pin_set_dt( &dt_red_led, true );

	while(1)
	{
		/* toggle red led pin */
		ret = gpio_pin_toggle_dt( &dt_red_led );
		if( ret < 0 )
		{
			return 0;
		}

		/* delay of 500ms */
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
