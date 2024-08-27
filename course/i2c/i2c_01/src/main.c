/*
 * Project: I2C_01 -> Scan I2C devices in ports
 * xioa ble sense board has two i2c port i2c0 and i2c1
 * config i2c in prj to enable i2c driver
*/

/* include required header here */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#define MAX_I2C_ADDRESS 127

/* device for two i2c port */
struct device *dev_i2c0;
struct device *dev_i2c1;

int get_device_binding_i2c0(void);
int get_device_binding_i2c1(void);

void scan_i2c_port(const struct device *i2c_port);

int main()
{
    k_msleep( 1000 );

	printk("getting device binding for i2c0 and i2c1\n");
	get_device_binding_i2c0();
	get_device_binding_i2c1();
	

	while( true )
	{
		scan_i2c_port( dev_i2c0 );
		k_msleep(5000);
		scan_i2c_port( dev_i2c1 );
	}
	return 1;
}


int get_device_binding_i2c0(void)
{
	int ret=0;

	dev_i2c0 = DEVICE_DT_GET( DT_NODELABEL(i2c0) );

	if( dev_i2c0 == NULL )
	{
		printk("device binding to port i2c0 failed. ");
		ret = -1;
	}
	else
	{
		if( !device_is_ready( dev_i2c0 ) )
		{
			printk(" device i2c0 is not ready yet\n ");
			ret = -1;
		}
	}

	return ret;
}

int get_device_binding_i2c1(void)
{
	int ret=0;

	dev_i2c1 = DEVICE_DT_GET( DT_NODELABEL(i2c1) );

	if( dev_i2c1 == NULL )
	{
		printk("device binding to port i2c1 failed. ");
		ret = -1;
	}
	else
	{
		if( !device_is_ready( dev_i2c1 ) )
		{
			printk(" device i2c1 is not ready yet\n ");
			ret = -1;
		}
	}

	return ret;
}

void scan_i2c_port(const struct device *i2c_port)
{
	uint8_t *data;
	uint8_t i2c_addr;
	uint8_t status;
	uint8_t number_of_bytes;

	struct i2c_msg i2cmsg;

	i2cmsg.buf = &data;
	i2cmsg.len = 0;
	i2cmsg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	number_of_bytes = 1;

	printk("started scanning i2c devices\n\n");
	printk("    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n00:    ");
	for(i2c_addr=1; i2c_addr<= MAX_I2C_ADDRESS; i2c_addr++)
	{
		if((i2c_addr % 0x10) == 0)	
		{
			printk("\n%2x: ",i2c_addr);
		}

		status = i2c_transfer(i2c_port, &i2cmsg, number_of_bytes, i2c_addr);

		if(status == 0)
		{
			printk("%2x ",i2c_addr);
		}
		else
		{
			printk("-- ");
		}

		k_sleep(K_MSEC(100));	
	}
	printk("\n\nstopped scanning i2c devices\n");
}