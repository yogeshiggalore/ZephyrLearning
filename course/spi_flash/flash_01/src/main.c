/*
 * Project: flash_01 -> Interfacing flash read, write and erase API
 * interfacing external flash using QSPI
 * config Nordic qspi flash api in prj
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/drivers/flash/nrf_qspi_nor.h>

/* device flash */
const struct device *dev_flash;

int get_device_binding_external_flash(void);

uint8_t val_cntr=0;
int status;
uint8_t read_buf[8];
uint8_t write_buf[8];
uint8_t loop_cntr=0;

int main(void)
{

	k_msleep(5000);
	printk("Project: flash_01 -> Interfacing flash read, write and erase API\n");
	
	status = get_device_binding_external_flash();
	if( status != 0  )
	{
		printk("failed to config external flash\n");
	}
	while( true )
	{
		status = flash_erase(dev_flash, 0, 4096);
		if( status == 0 )
		{
			printk("erase data success\n");
			for(loop_cntr=0;loop_cntr<sizeof(write_buf); loop_cntr++)
			{
				write_buf[loop_cntr] = val_cntr;
			}
			val_cntr++;
			status = flash_write(dev_flash, 0, write_buf, sizeof(write_buf));
			if( status == 0 )
			{
				printk("write success\n");
				status = flash_read( dev_flash, 0, read_buf, sizeof(read_buf) );
				if( status == 0 )
				{
					
					printk("read_buf: ");
					for(loop_cntr=0;loop_cntr<sizeof(read_buf); loop_cntr++)
					{
						printk("%2X ", read_buf[loop_cntr]);
					}
					printk("\n");
				}
				else
				{
					printk("flash data read err:%d", status);
				}
			}
			else
			{
				printk("write failed err:%d\n", status);
			}
		}
		else
		{
			printk("erase failed err:%d\n", status);
		}

		k_msleep(5000);
	}

	return 0;
}


int get_device_binding_external_flash(void)
{
	int ret=0;

	dev_flash = DEVICE_DT_GET( DT_NODELABEL(p25q16h) );

	if( dev_flash == NULL )
	{
		printk("device binding to port flash failed. ");
		ret = -1;
	}
	else
	{
		if( !device_is_ready( dev_flash ) )
		{
			printk(" device flash is not ready yet\n ");
			ret = -1;
		}
	}

	return ret;
}