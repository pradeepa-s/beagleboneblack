#include "sensor_conf.h"
#include "si7021_sensor.h"
#include "reading_logger.h"
#include "error_logger.h"
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>

#define MAX_FILENAME_LEN  (128)
#define MAX_LOG_ENTRY_LEN (100)

#ifndef DEBUG
static void run_actual();
#else  // DEBUG
static void run_test(unsigned int count);
#endif  // DEBUG

int main() {
#ifndef DEBUG
	run_actual();
#else  // DEBUG
    error_log("Running in debug mode.");
	int i = 0;
	while(i++ <= 1500) {
		run_test(i);
	}
#endif  // DEBUG
	return 0;
}

#ifndef DEBUG
void run_actual() {
 	SI7021_RET_VAL sensor_ret = si7021_init(si7021_i2c_path);
 	char log_entry[MAX_LOG_ENTRY_LEN];

 	if (SI7021_SUCCESS == sensor_ret) {
 		SI7021_READING reading;
 		sensor_ret = si7021_read(&reading);
 		if (SI7021_SUCCESS == sensor_ret) {
            log_reading(reading.temperature, reading.humidity);
 		}
 	}

    if (sensor_ret != SI7021_SUCCESS) {
        error_log("Sensor library failed: %d", sensor_ret);
    }

 	si7021_deinit();
}
#else  // DEBUG
void run_test(unsigned int count) {
 	SI7021_RET_VAL sensor_ret = SI7021_SUCCESS;
 	char log_entry[MAX_LOG_ENTRY_LEN];

 	if (SI7021_SUCCESS == sensor_ret) {
 		SI7021_READING reading;

		reading.humidity = count;
		reading.temperature = count + 1;

 		if (SI7021_SUCCESS == sensor_ret) {
            log_reading(reading.temperature, reading.humidity);
 		}
 	}
}
#endif   // DEBUG

