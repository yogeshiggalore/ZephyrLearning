/*
 * Project ht_02: blink red led 10 times with 500ms hardware timer value.
 * LED Red is connected to pin P0.26 of xiao ble sense board
 * Timer period value set to 500ms and duration value set to 5000ms
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/sys/printk.h>

/* timer value 500ms */
#define TIMER_VALUE_US		500000
#define ALARM_CHANNEL_ID 	0

#define LED_MAX_BLINK_COUNT	10

/* devicetree node for red led */
#define NODE_RED_LED	DT_ALIAS( led0 )

/* get red led dt spec */
static const struct gpio_dt_spec dt_red_led = GPIO_DT_SPEC_GET( NODE_RED_LED, gpios );

/* hardware timer 2 configuration */
#define TIMER2 DT_NODELABEL(timer2)
struct counter_alarm_cfg timer_cfg;
const struct device *const dev_timer_2 = DEVICE_DT_GET(TIMER2);

/* timer2 callback function */
void timer_2_callback_fun(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data);

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

	if (!device_is_ready(dev_timer_2))
	{
		return 0;
	}

	counter_start(dev_timer_2);
	int err;

	timer_cfg.flags = 0;
	timer_cfg.ticks = counter_us_to_ticks(dev_timer_2, TIMER_VALUE_US);
	timer_cfg.callback = timer_2_callback_fun;
	timer_cfg.user_data = &timer_cfg;

	err = counter_set_channel_alarm(dev_timer_2, ALARM_CHANNEL_ID, &timer_cfg);

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}

void timer_2_callback_fun(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
	int err;
	
	/* toggle the red led pin */
	gpio_pin_toggle_dt( &dt_red_led );

	if( led_blink_counter < ( LED_MAX_BLINK_COUNT * 2 ) )
	{
		led_blink_counter++;
		/* reload the values */
		err = counter_set_channel_alarm(dev, ALARM_CHANNEL_ID, user_data);
	}

}