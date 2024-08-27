/*
 * Project kt_04: blink two leds, one led with 730ms and another led with 456ms using single timer
 * LED Red is connected to pin P0.26 of xiao ble sense board
 * LED App is connected to P0.17 of xiao ble sense board
 * blink red led with 730ms and app led with 456ms
 * Add board folder and create xiao_ble_sense overlay file for app led 
 * Set Timer with period value of 1ms and duration value set to 1ms
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/sys/printk.h>

/* timer value for red and app leds */
#define LED_TIMER_VALUE_US			1000
#define LED_RED_TIMER_VALUE			730
#define LED_APP_TIMER_VALUE			456

#define ALARM_CHANNEL_ID	0

/* devicetree node for red led and app led */
#define NODE_LED_RED	DT_ALIAS( led0 )
#define NODE_LED_APP    DT_ALIAS( appled )

/* get red led and app led dt spec */
static const struct gpio_dt_spec dt_led_red = GPIO_DT_SPEC_GET( NODE_LED_RED, gpios );
static const struct gpio_dt_spec dt_led_app = GPIO_DT_SPEC_GET( NODE_LED_APP, gpios );

/* hardware timer configuration */
#define TIMER2 DT_NODELABEL(timer2)
struct counter_alarm_cfg timer_2_cfg;
const struct device *const dev_timer_2 = DEVICE_DT_GET(TIMER2);

uint16_t led_red_time_counter=0;
uint16_t led_app_time_counter=0;

/* timer2 callback function */
void timer_2_callback_fun(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data);

int main(void)
{
    int ret=0;
	int err;

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

	counter_start(dev_timer_2);

	timer_2_cfg.flags = 0;
	timer_2_cfg.ticks = counter_us_to_ticks(dev_timer_2, LED_TIMER_VALUE_US);
	timer_2_cfg.callback = timer_2_callback_fun;
	timer_2_cfg.user_data = &timer_2_cfg;

	err = counter_set_channel_alarm(dev_timer_2, ALARM_CHANNEL_ID, &timer_2_cfg);

	return 0;
}

void timer_2_callback_fun(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
	int err;
	
	led_red_time_counter++;
	led_app_time_counter++;

	if( led_red_time_counter > LED_RED_TIMER_VALUE )
	{
		led_red_time_counter = 0;
		gpio_pin_toggle_dt( &dt_led_red );
	}

	if( led_app_time_counter > LED_APP_TIMER_VALUE )
	{
		led_app_time_counter = 0;
		gpio_pin_toggle_dt( &dt_led_app );
	}

	/* reload the values */
	err = counter_set_channel_alarm(dev, ALARM_CHANNEL_ID, user_data);
}
