/*
 * Project: HT_06 -> blink different colors using rgb led with timer of 500ms
 * LED Red connected to P0.26 of xiao ble sense board
 * LED Green connected to P0.30 of xiao ble sense board
 * LED Blue connected to P0.06 of xiao ble sense board
*/

/* include headers */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/sys/printk.h>

/* timer value 500ms */
#define LED_TIMER_VALUE_US		500000

#define ALARM_CHANNEL_ID	0

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

/* hardware timer configuration */
#define TIMER2 DT_NODELABEL(timer2)
struct counter_alarm_cfg timer_2_cfg;
const struct device *const dev_timer_2 = DEVICE_DT_GET(TIMER2);

uint8_t current_led=LED_BLACK;

/* configure gpio pins */
int configure_rgb_led( const struct gpio_dt_spec *spec );

void rgb_turn_on_led( int led );

/* timer2 callback function */
void timer_2_callback_fun(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data);

int main(void)
{
	int err;
	configure_rgb_led( &dt_led_red );
	configure_rgb_led( &dt_led_green );
	configure_rgb_led( &dt_led_blue );

	counter_start(dev_timer_2);

	timer_2_cfg.flags = 0;
	timer_2_cfg.ticks = counter_us_to_ticks(dev_timer_2, LED_TIMER_VALUE_US);
	timer_2_cfg.callback = timer_2_callback_fun;
	timer_2_cfg.user_data = &timer_2_cfg;

	err = counter_set_channel_alarm(dev_timer_2, ALARM_CHANNEL_ID, &timer_2_cfg);

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

void timer_2_callback_fun(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
	int err;
	
	rgb_turn_on_led( current_led );
	current_led++;
	if( current_led > LED_WHITE )
	{
		current_led = LED_BLACK;
	}

	/* reload the values */
	err = counter_set_channel_alarm(dev, ALARM_CHANNEL_ID, user_data);
}