#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include "lsm6ds3.h"

#define MAX_I2C_ADDRESS 127

uint8_t IMU_DEBUG_LVL = IMU_DBG;

struct lsm6ds3_iconfig
{
	struct device *dev;
};

#ifdef IMU_DEBUG_ENABLE
#define IMU_PRINT( level, fmt, ... ) \
	if ( level >= IMU_DEBUG_LVL )printk( fmt, __VA_ARGS__ )
#else
#define IMU_PRINT( level, fmt, ... )
#endif

int lsm6ds3_begin( struct lsm6ds3 *imu, struct device *dev )
{
	int ret=0;
	

	IMU_DEBUG_LVL = imu->cfg.debug_lvl;
	imu->icfg = (struct lsm6ds3_iconfig*) malloc(sizeof(struct lsm6ds3_iconfig) );

	imu->icfg->dev = dev;

	if( imu->icfg->dev == NULL )
	{
		imu->cfg.err = LSM6DS3_ERR_DEV_BINDING;
		IMU_PRINT( IMU_ERR, "%s", "device binding to port i2c0 failed\n" );
		ret = -1;
	}
	else
	{
		if( !device_is_ready( imu->icfg->dev ) )
		{
			imu->cfg.err = LSM6DS3_ERR_DEV_NOT_READY;
			IMU_PRINT( IMU_ERR, "%s", " device i2c0 is not ready yet\n ");
			ret = -1;
		}
	}

	if( ret == 0 )
	{
		/* config default defined */
		if( imu->cfg.dft )
		{
			//set the gyroscope control register to work at 104 Hz, 2000 dps and in bypass mode
			ret = lsm6ds3_write_command( imu->icfg->dev, imu->cfg.addr, REG_LSM6DS3_CTRL2_G, LSM6DS3_CFG_DFT_GYRO );

			if( ret == 0 )
			{
				//Set the Accelerometer control register to work at 104 Hz, 4 g,and in bypass mode and enable ODR/4 low pass filter 
				ret = lsm6ds3_write_command( imu->icfg->dev, imu->cfg.addr, REG_LSM6DS3_CTRL1_XL, LSM6DS3_CFG_DFT_ACCL );
			}
			
			if( ret == 0 )
			{
				// set gyroscope power mode to high performance and bandwidth to 16 MHz
				ret = lsm6ds3_write_command( imu->icfg->dev, imu->cfg.addr, REG_LSM6DS3_CTRL7_G, 0x00 );
			}
				
			if( ret == 0 )
			{
				// Set the ODR config register to ODR/4
				ret = lsm6ds3_write_command( imu->icfg->dev, imu->cfg.addr, REG_LSM6DS3_CTRL8_XL, 0x09 );
			}

			if( ret == 0 )
			{
				imu->cfg.accl_fs = LSM6DS3_ACCL_FS_4G; 
				imu->cfg.gyro_fs = LSM6DS3_GYRO_FS_2000_DPS;
			}
			else
			{
				imu->cfg.err = LSM6DS3_ERR_CFG;
			}
		}
	}

	return ret;
}

int lsm6ds3_writeRegisters(const struct device *i2c_port, uint8_t i2c_addr, uint8_t *write_buf, uint8_t write_len)
{
	return i2c_write(i2c_port, write_buf, write_len, i2c_addr);
}

int lsm6ds3_write_command(const struct device *i2c_port, uint8_t i2c_addr, uint8_t cmd, uint8_t value)
{
	uint8_t write_buf[2];
	int ret;

	write_buf[0] = cmd;
	write_buf[1] = value;
	ret = lsm6ds3_writeRegisters( i2c_port, i2c_addr, &write_buf, sizeof(write_buf) );
	return ret;
}

void scan_i2c_port( struct lsm6ds3 *imu )
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

		status = i2c_transfer( imu->icfg->dev, &i2cmsg, number_of_bytes, i2c_addr);

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

int lsm6ds3_read_imu_data( struct lsm6ds3 *imu )
{
	int16_t data[7];
    uint8_t reg_addr = REG_LSM6DS3_OUT_TEMP_L;
	float accl_fs;
	float gyro_fs;

	if (lsm6ds3_readRegisters( imu->icfg->dev, imu->cfg.addr, &reg_addr, 1, (uint8_t*)data, sizeof(data)) != 0)
    {
        imu->val.acc_x = 0;
        imu->val.acc_y = 0;
        imu->val.acc_z = 0;
		imu->val.acc_err = -1;

		imu->val.gyr_x = 0;
		imu->val.gyr_y = 0;
		imu->val.gyr_z = 0;
		imu->val.gyr_err = -1;

		imu->val.temp = 0;
		imu->val.tem_err = -1;

		return -1;
    }

    imu->val.acc_err = 0;
	imu->val.gyr_err = 0;
	imu->val.tem_err = 0;

	imu->val.temp = (data[0] * 1.0) / 16;
	imu->val.temp += 25;

	accl_fs = lsm6ds3_get_accel_set_fs_value(imu);
	imu->val.acc_x = (data[1] * accl_fs) / 32768.0;
    imu->val.acc_y = (data[2] * accl_fs) / 32768.0;
    imu->val.acc_z = (data[3] * accl_fs) / 32768.0;
	
	gyro_fs = lsm6ds3_get_gyro_set_fs_value(imu);
	imu->val.gyr_x = (data[4] * gyro_fs) / 32768.0;
	imu->val.gyr_y = (data[5] * gyro_fs) / 32768.0;
    imu->val.gyr_z = (data[6] * gyro_fs) / 32768.0;

    return 0;
}

float lsm6ds3_get_accel_set_fs_value( struct lsm6ds3 *imu )
{
	float fs_val;

	switch (imu->cfg.accl_fs)
	{
	case LSM6DS3_ACCL_FS_2G:
		fs_val = 2.0;
		break;
	case LSM6DS3_ACCL_FS_4G:
		fs_val = 4.0;
		break;
	case LSM6DS3_ACCL_FS_8G:
		fs_val = 8.0;
		break;
	case LSM6DS3_ACCL_FS_16G:
		fs_val = 16.0;
		break;
	default:
		fs_val = 4.0;
		break;
	}
	return fs_val;
}

float lsm6ds3_get_gyro_set_fs_value( struct lsm6ds3 *imu )
{
	float fs_val;

	switch (imu->cfg.gyro_fs)
	{
	case LSM6DS3_GYRO_FS_245_DPS:
		fs_val = 245.0;
		break;
	case LSM6DS3_GYRO_FS_125_DPS:
		fs_val = 125.0;
		break;
	case LSM6DS3_GYRO_FS_500_DPS:
		fs_val = 500.0;
		break;
	case LSM6DS3_GYRO_FS_1000_DPS:
		fs_val = 1000.0;
		break;
	case LSM6DS3_GYRO_FS_2000_DPS:
		fs_val = 2000.0;
		break;
	default:
		fs_val = 245.0;
		break;
	}
	return fs_val;
}

int lsm6ds3_get_accel_data( struct lsm6ds3 *imu )
{
    int16_t data[3];
    uint8_t addr = REG_LSM6DS3_OUTX_L_XL;
	float fs_value;

    if (lsm6ds3_readRegisters( imu->icfg->dev, imu->cfg.addr, &addr, 1, (uint8_t*)data, sizeof(data)) != 0)
    {
        imu->val.acc_x = 0;
        imu->val.acc_y = 0;
        imu->val.acc_z = 0;
		imu->val.acc_err = -1;
		return -1;
    }

	fs_value = lsm6ds3_get_accel_set_fs_value(imu);

    imu->val.acc_x = (data[0] * fs_value) / 32768.0;
    imu->val.acc_y = (data[1] * fs_value) / 32768.0;
    imu->val.acc_z = (data[2] * fs_value) / 32768.0;
	imu->val.acc_err = 0;
    return 0;
}

int lsm6ds3_readRegisters(const struct device *i2c_port, uint8_t i2c_addr, uint8_t *write_buf, uint8_t write_len, uint8_t *read_buf, size_t read_len)
{
	int status=0;

	if( i2c_write_read( i2c_port, i2c_addr, write_buf, write_len, read_buf, read_len) != 0 )
	{
		status = -1;
	}
	return status;
}