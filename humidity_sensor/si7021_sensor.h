#ifndef _SI7021_SENSOR
#define _SI7021_SENSOR

typedef enum
{
	SI7021_SUCCESS,
	SI7021_FAIL
} SI7021_RET_VAL;

SI7021_RET_VAL si7021_init();

#endif  // _SI7021_SENSOR