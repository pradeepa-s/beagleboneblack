#include <stdio.h>
#include <time.h>
#include "si7021_sensor.h"

static const char temperature_humidity_log_file[] = "/home/debian/si7021_log.log";

int main()
{
	SI7021_RET_VAL ret = si7021_init();
	char log_entry[100];

	if (SI7021_SUCCESS == ret)
	{
		SI7021_READING reading;
		ret = si7021_read(&reading);
		if (SI7021_SUCCESS == ret)
		{
			FILE* fp;

			fp = fopen(temperature_humidity_log_file, "a+");
			if (fp)
			{
				fprintf(fp, "%ld, %f, %f\n", time(NULL), reading.temperature, reading.humidity);
				fclose(fp);
			}
		}
	}

	si7021_deinit();
	return 0;
}
