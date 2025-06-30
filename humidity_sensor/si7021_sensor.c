#include "si7021_sensor.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>


static const int addr = 0x40;
static int file_handle;

static SI7021_RET_VAL verify_ic();
static SI7021_RET_VAL i2c_transfer(const char* cmd, const size_t cmd_length, char* read_buf, const size_t read_length);


SI7021_RET_VAL si7021_init(const char *drv_path) {
	SI7021_RET_VAL ret = SI7021_SUCCESS;
	file_handle = open(drv_path, O_RDWR);

	if (file_handle < 0) {
		ret = SI7021_I2C_OPEN;
	}
	else {
		if (ioctl(file_handle, I2C_SLAVE, addr) < 0) {
			si7021_deinit();
			ret = SI7021_IOCTL;
		}
	}

	// i2c initialization is successful
	if (ret == SI7021_SUCCESS) {
		ret = verify_ic();
	}

	return ret;
}

SI7021_RET_VAL si7021_read(SI7021_READING* sensor_reading) {
	SI7021_RET_VAL ret = SI7021_FAIL;

	if (!sensor_reading) {
		return ret;
	}

	const char measure_rel_humidity = 0xE5;
	const char read_temperature = 0xE0;
	unsigned short temperature, humidity;

	// Read humidity
	ret = i2c_transfer(&measure_rel_humidity, sizeof(measure_rel_humidity), (char*)&humidity, sizeof(humidity));

	if (ret == SI7021_SUCCESS) {
		// Read temperature
		ret = i2c_transfer(&read_temperature, sizeof(read_temperature), (char*)&temperature, sizeof(temperature));
	}

	if (ret == SI7021_SUCCESS) {
		// Correct endian-ness
		temperature = ((temperature & 0x00FF) << 8) | ((temperature & 0xFF00) >> 8);
		humidity = ((humidity & 0x00FF) << 8) | ((humidity & 0xFF00) >> 8);
		sensor_reading->humidity= ((125.0 * humidity) / 65536.0) - 6;
		sensor_reading->temperature = ((175.72 * temperature) / 65536.0) - 46.85;
	}

	return ret;
}

void si7021_deinit() {
	close(file_handle);
}


// --------------------------------

SI7021_RET_VAL verify_ic()
{
	SI7021_RET_VAL ret = SI7021_INVALID_IC;

	const char electronic_id_2[2] = {0xFC, 0xC9};
	char id_val[4];

	if (i2c_transfer(electronic_id_2, 2, id_val, 4) == SI7021_SUCCESS) {
		if (id_val[0] == 0x15) {
			ret = SI7021_SUCCESS;
		}
	}

	return ret;
}

SI7021_RET_VAL i2c_transfer(const char* cmd, const size_t cmd_length, char* read_buf, const size_t read_length)
{
	size_t transfer_length = write(file_handle, cmd, cmd_length);

	if (transfer_length == cmd_length) {
		transfer_length = read(file_handle, read_buf, read_length);

		if (transfer_length == read_length) {
			return SI7021_SUCCESS;
		}
	}

	return SI7021_FAIL;
}
