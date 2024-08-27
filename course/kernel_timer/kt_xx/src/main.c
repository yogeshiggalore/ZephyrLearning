/*
 * Project kt_xx: kernel timer understanding
 * terminal used for serial print
 *  
 */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* timer values */
#define PERIOD_VALUE		1000
#define DURATION_VALUE		10000

/* kernel timer configuration */
struct k_timer test_timer;
uint32_t test_timer_status;

void test_timer_expiry_fn( struct k_timer *timer );
void test_timer_stop_fn( struct k_timer *timer );

uint32_t timer_last=0;

int main(void)
{
	/* timer init function
	 * register timer with expiry function and stop function
	 * expiry function calls when timeout happens
	 * stop function calls when timer stops prematurly 
	 */
	k_timer_init( &test_timer, test_timer_expiry_fn, test_timer_stop_fn );

	/* start timer 
	 * duration value is first time expiry value
	 * period value is interval expiry value 
	 */
	k_timer_start( &test_timer, K_MSEC( DURATION_VALUE ), K_MSEC( PERIOD_VALUE ) );

	/* get current uptime */
	timer_last = k_uptime_get_32();

	k_timer_status_sync(&test_timer);

	while( true )
	{
		/* status value that indicates how many times the timer has expired since the status value was last read  */
		test_timer_status = k_timer_status_get( &test_timer );
		printk("status:%u\n", test_timer_status);
		k_msleep(5000);
	}
	return 0;
}

void test_timer_expiry_fn( struct k_timer *timer )
{
	uint32_t time_now;
	uint32_t time_diff;

	time_now = k_uptime_get_32();
	time_diff = time_now - timer_last;

	printk("timer_last:%u ", timer_last);
	printk("time_now:%u ", time_now);
	printk("diff:%u\n\n", time_diff);

	timer_last = time_now;
}

void test_timer_stop_fn( struct k_timer *timer )
{
	printk("test_timer_stop_fn\n");
}
