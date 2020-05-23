#include <stdio.h>
#include <time.h>
#include "si7021_sensor.h"

int main()
{
	SI7021_RET_VAL ret = si7021_init();

	if (SI7021_SUCCESS == ret)
	{
		SI7021_READING reading;
		ret = si7021_read(&reading);
		if (SI7021_SUCCESS == ret)
		{
			printf("%ld, %f, %f\n", time(NULL), reading.temperature, reading.humidity);
		}
	}

	si7021_deinit();
	return 0;
}
