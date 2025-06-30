#ifndef _SENSOR_CONF_H
#define _SENSOR_CONF_H

// #define DEBUG

#ifndef DEBUG
static const int MAXIMUM_FILE_SIZE = 500000;  // 500 kB
static const char temperature_humidity_log_file[] = "/home/pradeepa/si7021_log.log";
static const int FILENAME_END_LOCATION = sizeof(temperature_humidity_log_file);
static const char si7021_i2c_path[] = "/dev/i2c-1";
#else  // DEBUG
static const char temperature_humidity_log_file[] = "test_file.log";
static const int MAXIMUM_FILE_SIZE = 5000;
static const int FILENAME_END_LOCATION = 9;
#endif  // DEBUG


#endif  // _SENSOR_CONF_H
