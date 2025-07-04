#ifndef _FILE_LOG_CONF_H
#define _FILE_LOG_CONF_H

#include <stddef.h>

#ifndef DEBUG
static const size_t MAXIMUM_FILE_SIZE = 500 * 1024;
static const char temperature_humidity_log_file[] = "/home/pradeepa/si7021_log.log";
static const size_t FILENAME_END_LOCATION = sizeof(temperature_humidity_log_file) - 5;
#else  // DEBUG
static const size_t MAXIMUM_FILE_SIZE = 5000;
static const char temperature_humidity_log_file[] = "test_file.log";
static const size_t FILENAME_END_LOCATION = sizeof(test_file);
#endif  // DEBUG

#endif  // _FILE_LOG_CONF_H
