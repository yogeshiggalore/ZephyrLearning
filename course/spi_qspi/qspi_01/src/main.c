/*
 * Project: QSPI_01 -> Interfacing QSPI with external flash
 * interfacing external flash using QSPI
 * config QSPI in prj to enable qspi driver
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/drivers/flash/nrf_qspi_nor.h>

const struct device *dev_qspi;
struct spi_config 	cfg_qspi;

int get_device_binding_qspi(void);
//int test_spi_commands( struct device *dev, struct spi_config *spi_cfg, );
void spi_jedec_id_test_send(void);
int qspi_read_cmd(const struct device *dev, uint8_t cmd_code, uint8_t *id, uint16_t len );

uint8_t jedec_id[4];

struct qspi_buf {
	uint8_t *buf;
	size_t len;
};

struct qspi_cmd_pck {
	uint8_t op_code;
	const struct qspi_buf *tx_buf;
	const struct qspi_buf *rx_buf;
};

/* not working code */
int main(void)
{
	int ret;

	k_msleep(5000);
	printk("program QSPI_01 started\n");

	printk("getting qspi device binding\n");
	ret = get_device_binding_qspi();    
	if( ret != 0 )
	{
		printk("device binding failed err:%d\n", ret);
	}

	while( true )
	{
		printk("loop\n");
		//flash_read_jedec_id( dev_qspi, jedec_id );
		qspi_read_cmd( dev_qspi, jedec_id );
		printk("id: %2X %2X %2X %2X", jedec_id[0], jedec_id[1], jedec_id[2], jedec_id[3]);
		k_msleep(5000);
	}
	return 0;
}


int get_device_binding_qspi(void)
{
	int ret=0;

	dev_qspi = DEVICE_DT_GET( DT_NODELABEL(p25q16h) );

	if( dev_qspi == NULL )
	{
		printk("device binding to port qspi failed. ");
		ret = -1;
	}
	else
	{
		if( !device_is_ready( dev_qspi ) )
		{
			printk(" device qspi is not ready yet\n ");
			ret = -1;
		}
	}

	return ret;
}

int qspi_read_cmd(const struct device *dev, uint8_t cmd_code, uint8_t *id, uint16_t len )
{
	const struct qspi_buf rx_buf = {
		.buf = id,
		.len = len
	};
	const struct qspi_cmd_pck cmd = {
		.op_code = cmd_code,
		.rx_buf = &rx_buf,
	};

	int ret = qspi_device_init(dev);

	if (ret == 0) {
		ret = qspi_send_cmd(dev, &cmd, false);
	}
	qspi_device_uninit(dev);

	return ret;
}

/*
void spi_jedec_id_test_send(void)
{
	int err;

	static uint8_t tx_buffer[4] = {
		0X9F,
		0x00,
		0x00,
		0x00

	};
	
	static uint8_t rx_buffer[4]= {0,0,0,0};

	const struct spi_buf tx_buf = {
		.buf = tx_buffer,
		.len = sizeof(tx_buffer)};
	const struct spi_buf_set tx = {
		.buffers = &tx_buf,
		.count = 1};

	struct spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = sizeof(rx_buffer),
	};
	const struct spi_buf_set rx = {
		.buffers = &rx_buf,
		.count = 1};

	gpio_pin_set(dev_gpio0, spi1cspin, 1);
	err = spi_transceive(dev_qspi, &cfg_spi1, &tx, &rx);
	gpio_pin_set(dev_gpio0, spi1cspin, 0);

	if (err)
	{
		printk("SPI error: %d\n", err);
	}
	else
	{
		// Connect MISO to MOSI for loopback 
		printk("SPI Tx bytes: ");
		for (int i = 0; i < sizeof(tx_buffer); i++)
		{
			printk("%x ", tx_buffer[i]);
		}
		printk(" SPI Rx bytes: ");
		for (int i = 0; i < sizeof(rx_buffer); i++)
		{
			printk("%x ", rx_buffer[i]);
		}
		printk("\n");
	}
}
*/