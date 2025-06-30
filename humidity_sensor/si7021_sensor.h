#ifndef _SI7021_SENSOR
#define _SI7021_SENSOR

typedef enum
{
	SI7021_SUCCESS,
    SI7021_I2C_OPEN,
    SI7021_IOCTL,
    SI7021_INVALID_IC,
	SI7021_FAIL
} SI7021_RET_VAL;

typedef struct
{
	float humidity;
	float temperature;
} SI7021_READING;

SI7021_RET_VAL si7021_init();
void si7021_deinit();
SI7021_RET_VAL si7021_read(SI7021_READING* sensor_reading);

#endif  // _SI7021_SENSOR
