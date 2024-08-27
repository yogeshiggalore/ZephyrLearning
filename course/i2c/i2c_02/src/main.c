/*
 * Project: I2C_02 -> Interfacing LSM6DS3
 * LSM6DS3 accelerometer and gyroscope
 * config i2c in prj to enable i2c driver
*/

/* include required header here */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>

#include "lsm6ds3/lsm6ds3.h"

int get_device_binding_i2c0(void);

float x,y,z;
uint32_t step_cnt;

struct lsm6ds3 imu;

int main(void)
{
	k_msleep(5000);

	printk("demo started\n");

	imu.cfg.debug_lvl = IMU_DBG;
	imu.cfg.addr = 0x6A;
	imu.cfg.dft = true;
	lsm6ds3_begin( &imu, DEVICE_DT_GET(DT_NODELABEL(i2c0)) );

	while( true )
	{
		if( lsm6ds3_read_imu_data( &imu ) != 0)
		{
			printk("err\n");
		}
		else
		{
			printf("%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f\n", imu.val.temp, imu.val.acc_x, imu.val.acc_y, imu.val.acc_z, \
																imu.val.gyr_x, imu.val.gyr_y, imu.val.gyr_z);
		}
		k_msleep(100);
	}
	
    return 0;
}