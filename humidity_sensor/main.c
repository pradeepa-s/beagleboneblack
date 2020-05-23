#include <stdio.h>
#include "si7021_sensor.h"

int main()
{
	SI7021_RET_VAL ret = si7021_init();

	if (SI7021_SUCCESS == ret)
	{
		printf("Success\n");
	}
	return 0;
}
