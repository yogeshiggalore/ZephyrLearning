/*
 * Project ht_xx: hardware timer understanding
 * terminal used for serial print
 * 
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/sys/printk.h>

/* timer value 1000ms */
#define TIMER_VALUE_US		1000000
#define ALARM_CHANNEL_ID 	0

/* hardware timer 2 configuration */
#define TIMER2 DT_NODELABEL(timer2)

#define TEST_MAX_VALUE			10

/* counter alarm configuration */
struct counter_alarm_cfg timer_cfg;

/* counter top configuration */
struct counter_top_cfg top_cfg;

/* counter configuration info */
struct counter_config_info	config_info;

/* get timer device */
const struct device *const dev_timer_2 = DEVICE_DT_GET(TIMER2);

static uint8_t alarm_test_counter=0;
static uint8_t top_test_counter=0;

/* timer2 callback function */
void timer_2_alarm_cfg_cb_fn(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data);
void timer_2_top_cfg_cb_fn(const struct device *dev, void *user_data);

void testing_alarm_configuration(void);
void testing_top_configuration(void);

int main(void)
{
    int ret=0;
	int err;

	k_msleep( 5000 );

	if (!device_is_ready(dev_timer_2))
	{
		printk("counter hardware is not ready\n");
		return 0;
	}

	if( counter_start(dev_timer_2) != 0)
	{
		printk("fail to start counter\n");
		return 0;
	}
	
	testing_alarm_configuration();

	while (1) {
		k_sleep(K_FOREVER);
	}

	return 0;
}

void timer_2_alarm_cfg_cb_fn(const struct device *dev, uint8_t chan_id, uint32_t ticks, void *user_data)
{
	int err;

	printk("alarm timeout\n");

	if( alarm_test_counter < TEST_MAX_VALUE)
	{
		alarm_test_counter++;
		/* reload the values */
		err = counter_set_channel_alarm(dev, ALARM_CHANNEL_ID, user_data);
	}
	else
	{
		alarm_test_counter=0;
		printk("exceded alarm test counter\n");
		err = counter_cancel_channel_alarm(dev, ALARM_CHANNEL_ID);
		if( err == 0 )
		{
			printk("alarm cancel success\n");
			testing_top_configuration();
		}
		else
		{
			printk("alarm cancel failed\n");
		}
	}
}


void timer_2_top_cfg_cb_fn(const struct device *dev, void *user_data)
{
	int err;

	printk("top cfg timeout\n");

	if( top_test_counter < TEST_MAX_VALUE)
	{
		top_test_counter++;
		/* reload the values */
		err = counter_set_top_value(dev, user_data);
	}
	else
	{
		top_test_counter=0;
		printk("exceded top test counter\n");
		top_cfg.flags = 0;
	}
}

void testing_alarm_configuration(void)
{
	int err;

	printk("\n\n\ntesting alarm options\n");
	printk("timer_cfg.flags = 0\n");
	printk("timer ticks to 1 seconds\n");

	alarm_test_counter=0;

	timer_cfg.flags = 0;
	timer_cfg.ticks = counter_us_to_ticks(dev_timer_2, TIMER_VALUE_US);
	timer_cfg.callback = timer_2_alarm_cfg_cb_fn;
	timer_cfg.user_data = &timer_cfg;

	err = counter_set_channel_alarm(dev_timer_2, ALARM_CHANNEL_ID, &timer_cfg);
	if( err == 0)
	{
		printk("alarm configuration set success\n");
	}
	else
	{
		printk("alarm configuration set failed err:%d\n", err);
	}
} 

void testing_top_configuration(void)
{
	int err;

	printk("\n\n\ntop config options\n");
	printk("top_cfg.flags = COUNTER_TOP_CFG_DONT_RESET\n");
	printk("top config ticks to 1 seconds\n");

	top_test_counter=0;

	top_cfg.ticks = counter_us_to_ticks(dev_timer_2, TIMER_VALUE_US);
	top_cfg.flags = COUNTER_TOP_CFG_DONT_RESET;
	top_cfg.callback = timer_2_top_cfg_cb_fn;
	top_cfg.user_data = &top_cfg;

	err = counter_set_top_value(dev_timer_2, &top_cfg );

	if( err == 0)
	{
		printk("top configuration set success\n");
	}
	else
	{
		printk("top configuration set failed err:%d\n", err);
	}
}