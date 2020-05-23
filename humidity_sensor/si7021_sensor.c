#include "si7021_sensor.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


static int file_handle;

static SI7021_RET_VAL verify_ic();

SI7021_RET_VAL si7021_init()
{
	SI7021_RET_VAL ret = SI7021_SUCCESS;
	const char filename[] = "/dev/i2c-2";
	const int addr = 0x40;

	file_handle = open(filename, O_RDWR);

	if (file_handle < 0)
	{
		ret = SI7021_FAIL;
	}
	else
	{
		if (ioctl(file_handle, I2C_SLAVE, addr) < 0)
		{
			close(file_handle);
			ret = SI7021_FAIL;
		}
	}

	// i2c initialization is successful
	if (ret == SI7021_SUCCESS)
	{
		ret = verify_ic();

		if (ret != SI7021_SUCCESS)
		{
			printf("Failed veri\n");
		//	close(file_handle);
		}
		close(file_handle);
	}

	return ret;
}


// --------------------------------

SI7021_RET_VAL verify_ic()
{
	SI7021_RET_VAL ret = SI7021_FAIL;

	const char electronic_id_2[2] = {0xFC, 0xC9};
	char id_val[4];


	if (write(file_handle, electronic_id_2, sizeof(electronic_id_2)) == sizeof(electronic_id_2))
	{
		if (read(file_handle, id_val, sizeof(id_val)) == sizeof(id_val))
		{
			if (id_val[0] == 0x15)
			{
				ret = SI7021_SUCCESS;
			}
		}
	}

	return ret;
}
