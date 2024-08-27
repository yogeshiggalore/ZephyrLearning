#ifndef LSM6DS3
#define LSM6DS3

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>

#define IMU_DEBUG_ENABLE    1

#define LSM6DS3_CFG_DFT_GYRO ( LSM6DS3_GYRO_ODR_104_HZ | LSM6DS3_GYRO_FS_2000_DPS )
#define LSM6DS3_CFG_DFT_ACCL ( LSM6DS3_ACCL_ODR_104_HZ | LSM6DS3_ACCL_FS_4G | LSM6DS3_ACCL_LPS_EN )

/* IMU module debug level */
enum IMU_DBG_LVL
{
	IMU_NON = 0,	// none
	IMU_DBG,		// print debug data
	IMU_ERR,		// print error data
	IMU_WAR,		// print warning data
	IMU_INF,		// print info data
	IMU_ENUMERR,
};

/* gyroscope performance mode */
enum LSM6DS3_GYRO_PERFM_MODE
{
    LSM6DS3_GYRO_PERFM_MODE_ENABLE=0,
    LSM6DS3_GYRO_PERFM_MODE_DISABLE,
};

/* gyroscope output data rate */
enum LSM6DS3_GYRO_ODR
{
    LSM6DS3_GYRO_ODR_0_HZ       =0x00,
    LSM6DS3_GYRO_ODR_12_5_HZ    =0x10,
    LSM6DS3_GYRO_ODR_26_HZ      =0x20,
    LSM6DS3_GYRO_ODR_52_HZ      =0x30,
    LSM6DS3_GYRO_ODR_104_HZ     =0x40,
    LSM6DS3_GYRO_ODR_208_HZ     =0x50,
    LSM6DS3_GYRO_ODR_416_HZ     =0x60,
    LSM6DS3_GYRO_ODR_833_HZ     =0x70,
    LSM6DS3_GYRO_ODR_1660_HZ    =0x80,
    LSM6DS3_GYRO_ODR_3333_HZ    =0x90,
    LSM6DS3_GYRO_ODR_6666_HZ    =0xA0,
};

/* gyroscope full scale */
enum LSM6DS3_GYRO_FS
{
    LSM6DS3_GYRO_FS_245_DPS  = 0x00,
    LSM6DS3_GYRO_FS_125_DPS  = 0x02,
    LSM6DS3_GYRO_FS_500_DPS  = 0x04,
    LSM6DS3_GYRO_FS_1000_DPS = 0x08,
    LSM6DS3_GYRO_FS_2000_DPS = 0x0C,
};

/* accelerometer output data rate */
enum LSM6DS3_ACCL_ODR
{
    LSM6DS3_ACCL_ODR_0_HZ       =0x00,
    LSM6DS3_ACCL_ODR_12_5_HZ    =0x10,
    LSM6DS3_ACCL_ODR_26_HZ      =0x20,
    LSM6DS3_ACCL_ODR_52_HZ      =0x30,
    LSM6DS3_ACCL_ODR_104_HZ     =0x40,
    LSM6DS3_ACCL_ODR_208_HZ     =0x50,
    LSM6DS3_ACCL_ODR_416_HZ     =0x60,
    LSM6DS3_ACCL_ODR_833_HZ     =0x70,
    LSM6DS3_ACCL_ODR_1660_HZ    =0x80,
    LSM6DS3_ACCL_ODR_3333_HZ    =0x90,
    LSM6DS3_ACCL_ODR_6666_HZ    =0xA0,
};

/* accelerometer full scale */
enum LSM6DS3_ACCL_FS
{
    LSM6DS3_ACCL_FS_2G  = 0x00,
    LSM6DS3_ACCL_FS_4G  = 0x08,
    LSM6DS3_ACCL_FS_8G  = 0x0C,
    LSM6DS3_ACCL_FS_16G = 0x04,
};

/* accelerometer bandwidth 0 */
enum LSM6DS3_ACCL_BW0
{
    LSM6DS3_ACCL_BW0_1500_HZ  = 0x00,
    LSM6DS3_ACCL_BW0_400_HZ  = 0x01,
};

/* accelerometer low pass filter */
enum LSM6DS3_ACCL_LPS
{
    LSM6DS3_ACCL_LPS_DI  = 0x00,
    LSM6DS3_ACCL_LPS_EN  = 0x02,
};

/* gyro angular rate sensor */
enum LSM6DS3_GYRO_HIGH_PERM_MODE
{
    LSM6DS3_GYRO_HIGH_PERM_MODE_EN = 0x00,
    LSM6DS3_GYRO_HIGH_PERM_MODE_DI = 0x80,
};

/* gyroscope high pass filter */
enum LSM6DS3_GYRO_HP_FILTER
{
    LSM6DS3_GYRO_HP_FILTER_EN = 0x40,
    LSM6DS3_GYRO_HP_FILTER_DI = 0x00,
};

/* gyroscope high pass filter cutoff */
enum LSM6DS3_GYRO_HP_FIL_CUTOFF
{
    LSM6DS3_GYRO_HP_FIL_CUTOFF_16   = 0x00,
    LSM6DS3_GYRO_HP_FIL_CUTOFF_65   = 0x10,
    LSM6DS3_GYRO_HP_FIL_CUTOFF_260  = 0x20,
    LSM6DS3_GYRO_HP_FIL_CUTOFF_1    = 0x30,
};

/* gyroscope rounding status */
enum LSM6DS3_GYRO_ROUND_STS
{
    LSM6DS3_GYRO_ROUND_STS_EN = 0x04,
    LSM6DS3_GYRO_ROUND_STS_DI = 0x00,
};

/* accelerometer lpf2 */
enum LSM6DS3_ACCL_LPF2
{
    LSM6DS3_ACCL_LPF2_EN = 0x80,
    LSM6DS3_ACCL_LPF2_DI = 0x00,
};

/* accelerometer lpf2 hpf */
enum LSM6DS3_ACCL_LPF2_HPF
{
    LSM6DS3_ACCL_LPF2_HPF_ODR_50 = 0x00,
    LSM6DS3_ACCL_LPF2_HPF_ODR_100 = 0x20,
    LSM6DS3_ACCL_LPF2_HPF_ODR_9 = 0x40,
    LSM6DS3_ACCL_LPF2_HPF_ODR_400 = 0x60,
};


/* i2c registers for lsm6ds3 */
enum LSM6DS3_I2C_REGISTER
{
    REG_LSM6DS3_FUN_CFG_ACC     = 0x01,
    REG_LSM6DS3_SYNC_TIME_FRAME = 0x04,
    REG_LSM6DS3_SYNC_RES_RATION = 0x05,
    REG_LSM6DS3_FIFO_CTRL1      = 0x06,
    REG_LSM6DS3_FIFO_CTRL2      = 0x07,
    REG_LSM6DS3_FIFO_CTRL3      = 0x08,
    REG_LSM6DS3_FIFO_CTRL4      = 0x09,
    REG_LSM6DS3_FIFO_CTRL5      = 0x0A,
    REG_LSM6DS3_DRY_PUL_CFG_G   = 0x0B,
    REG_LSM6DS3_INTRPT_1_CTRL   = 0x0D,
    REG_LSM6DS3_INTRPT_2_CTRL   = 0x0E,
    REG_LSM6DS3_WHO_AM_I_REG    = 0X0F,
    REG_LSM6DS3_CTRL1_XL        = 0X10,
    REG_LSM6DS3_CTRL2_G         = 0X11,
    REG_LSM6DS3_CTRL3_C         = 0X12,
    REG_LSM6DS3_CTRL4_C         = 0X13,
    REG_LSM6DS3_CTRL5_C         = 0X14,
    REG_LSM6DS3_CTRL6_C         = 0X15,
    REG_LSM6DS3_CTRL7_G         = 0X16,
    REG_LSM6DS3_CTRL8_XL        = 0X17,
    REG_LSM6DS3_CTRL9_XL        = 0X18,
    REG_LSM6DS3_CTRL10_C        = 0X19,
    REG_LSM6DS3_MST_CFG         = 0X1A,
    REG_LSM6DS3_WAKE_UP_SRC     = 0X1B,
    REG_LSM6DS3_TAP_SRC         = 0X1C,
    REG_LSM6DS3_D6D_SRC         = 0X1D,
    REG_LSM6DS3_STATUS_REG      = 0X1E,
    REG_LSM6DS3_OUT_TEMP_L      = 0x20,
    REG_LSM6DS3_OUT_TEMP_H      = 0x21,
    REG_LSM6DS3_OUTX_L_G        = 0X22,
    REG_LSM6DS3_OUTX_H_G        = 0X23,
    REG_LSM6DS3_OUTY_L_G        = 0X24,
    REG_LSM6DS3_OUTY_H_G        = 0X25,
    REG_LSM6DS3_OUTZ_L_G        = 0X26,
    REG_LSM6DS3_OUTZ_H_G        = 0X27,
    REG_LSM6DS3_OUTX_L_XL       = 0X28,
    REG_LSM6DS3_OUTX_H_XL       = 0X29,
    REG_LSM6DS3_OUTY_L_XL       = 0X2A,
    REG_LSM6DS3_OUTY_H_XL       = 0X2B,
    REG_LSM6DS3_OUTZ_L_XL       = 0X2C,
    REG_LSM6DS3_OUTZ_H_XL       = 0X2D,
    REG_LSM6DS3_SENSE_HUB1      = 0x2E,
    REG_LSM6DS3_SENSE_HUB2      = 0x2F,
    REG_LSM6DS3_SENSE_HUB3      = 0x2E,
    REG_LSM6DS3_SENSE_HUB4      = 0x30,
    REG_LSM6DS3_SENSE_HUB5      = 0x31,
    REG_LSM6DS3_SENSE_HUB6      = 0x32,
    REG_LSM6DS3_SENSE_HUB7      = 0x33,
    REG_LSM6DS3_SENSE_HUB8      = 0x34,
    REG_LSM6DS3_SENSE_HUB9      = 0x35,
    REG_LSM6DS3_SENSE_HUB10     = 0x36,
    REG_LSM6DS3_SENSE_HUB11     = 0x37,
    REG_LSM6DS3_SENSE_HUB12     = 0x38,
    REG_LSM6DS3_FIFO_STATUS1    = 0x3A,
    REG_LSM6DS3_FIFO_STATUS2    = 0x3B,
    REG_LSM6DS3_FIFO_STATUS3    = 0x3C,
    REG_LSM6DS3_FIFO_STATUS4    = 0x3D,
    REG_LSM6DS3_FIFO_DATA_OUT_L = 0x3E,
    REG_LSM6DS3_FIFO_DATA_OUT_H = 0x3F,
    REG_LSM6DS3_TIME_STAMP0     = 0x40,
    REG_LSM6DS3_TIME_STAMP1     = 0x41,
    REG_LSM6DS3_TIME_STAMP2     = 0x42,
    REG_LSM6DS3_STEP_TIMESTMP_L = 0x49,
    REG_LSM6DS3_STEP_TIMESTMP_H = 0x4A,
    REG_LSM6DS3_STEP_COUNTER_L  = 0x4B,
    REG_LSM6DS3_STEP_COUNTER_H  = 0x4C,
    REG_LSM6DS3_SENSE_HUB13     = 0x4D,
    REG_LSM6DS3_SENSE_HUB14     = 0x4E,
    REG_LSM6DS3_SENSE_HUB15     = 0x4F,
    REG_LSM6DS3_SENSE_HUB16     = 0x50,
    REG_LSM6DS3_SENSE_HUB17     = 0x51,
    REG_LSM6DS3_SENSE_HUB18     = 0x52,
    REG_LSM6DS3_FUN_SRC1        = 0x53,
    REG_LSM6DS3_FUN_SRC2        = 0x54,
    REG_LSM6DS3_WRIST_TILT_IA   = 0x55,
    REG_LSM6DS3_TAP_CFG         = 0x58,
    REG_LSM6DS3_TAP_THS_6D      = 0x59,
    REG_LSM6DS3_INT_DUR2        = 0x5A,
    REG_LSM6DS3_WAKE_UP_THS     = 0x5B,
    REG_LSM6DS3_WAKE_UP_DUR     = 0x5C,
    REG_LSM6DS3_FREE_FALL       = 0x5D,
    REG_LSM6DS3_MD1_CFG         = 0x5E,
    REG_LSM6DS3_MD2_CFG         = 0x5F,
    REG_LSM6DS3_MST_CMD_CODE    = 0x60,
    REG_LSM6DS3_SYNC_SPI_ERR    = 0x61,
    REG_LSM6DS3_OUT_MAG_RAW_XL  = 0x66,
    REG_LSM6DS3_OUT_MAG_RAW_XH  = 0x67,
    REG_LSM6DS3_OUT_MAG_RAW_YL  = 0x68,
    REG_LSM6DS3_OUT_MAG_RAW_YH  = 0x69,
    REG_LSM6DS3_OUT_MAG_RAW_ZL  = 0x6A,
    REG_LSM6DS3_OUT_MAG_RAW_ZH  = 0x6B,
    REG_LSM6DS3_X_OFS_USR       = 0x73,
    REG_LSM6DS3_Y_OFS_USR       = 0x74,
    REG_LSM6DS3_Z_OFS_USR       = 0x75,
};


enum LSM6DS3_ERRS{
    LSM6DS3_ERR_NONE=0,
    LSM6DS3_ERR_DEV_BINDING,
    LSM6DS3_ERR_DEV_NOT_READY,
    LSM6DS3_ERR_CFG,
};

struct lsm6ds3_iconfig;

struct lsm6ds3_config{
    uint8_t addr;
    uint8_t debug_lvl;
    uint8_t err;
    bool dft;
    enum LSM6DS3_ACCL_FS accl_fs;
    enum LSM6DS3_GYRO_FS gyro_fs;
    float gyro_scaling;
};

struct lsm6ds3_values
{
    float acc_x;
    float acc_y;
    float acc_z;
    int acc_err;

    float gyr_x;
    float gyr_y;
    float gyr_z;
    int gyr_err;

    float temp;
    int tem_err;
};

struct lsm6ds3
{
    struct lsm6ds3_iconfig *icfg;
    struct lsm6ds3_config cfg;
    struct lsm6ds3_values val;
};

int lsm6ds3_begin( struct lsm6ds3 *imu, struct device *dev );
void scan_i2c_port( struct lsm6ds3 *imu );
int lsm6ds3_read_imu_data( struct lsm6ds3 *imu );
int lsm6ds3_get_accel_data( struct lsm6ds3 *imu );
float lsm6ds3_get_accel_set_fs_value( struct lsm6ds3 *imu );
float lsm6ds3_get_gyro_set_fs_value( struct lsm6ds3 *imu );
int lsm6ds3_write_command(const struct device *i2c_port, uint8_t i2c_addr,uint8_t cmd, uint8_t value);
int lsm6ds3_readRegisters(const struct device *i2c_port, uint8_t i2c_addr, uint8_t *write_buf, uint8_t write_len, uint8_t *read_buf, size_t read_len);
int lsm6ds3_writeRegisters(const struct device *i2c_port, uint8_t i2c_addr,uint8_t *write_buf, uint8_t write_len);

#endif
