#include "sensor_conf.h"
#include "si7021_sensor.h"
#include "logger.h"
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

#define MAX_FILENAME_LEN  (128)
#define MAX_LOG_ENTRY_LEN (100)

static long get_file_size(FILE* fp);
static void save_reading(FILE* fp, const float temperature, const float humidity);
static int get_next_filename(char* filename, size_t size);
static int backup_file(FILE* old_file, const char* new_filename);

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
 			FILE* fp;

 			fp = fopen(temperature_humidity_log_file, "a+");
 			if (fp) {
 				save_reading(fp, reading.temperature, reading.humidity);
 				if (get_file_size(fp) > MAXIMUM_FILE_SIZE) {
					char backup_filenam[MAX_FILENAME_LEN];
					get_next_filename(backup_filenam, sizeof(backup_filenam));
					if (0 == backup_file(fp, backup_filenam)) {
						freopen(temperature_humidity_log_file, "w", fp);
					}
 				}
 				fclose(fp);
 			}
            else {
                error_log("Failed to open the file");
            }
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
 			FILE* fp;

 			fp = fopen(temperature_humidity_log_file, "a+");
 			if (fp) {
 				save_reading(fp, reading.temperature, reading.humidity);
 				if (get_file_size(fp) > MAXIMUM_FILE_SIZE) {
					char backup_filenam[MAX_FILENAME_LEN];
					get_next_filename(backup_filenam, sizeof(backup_filenam));
					if (0 == backup_file(fp, backup_filenam)) {
						freopen(temperature_humidity_log_file, "w", fp);
					}
 				}
 				fclose(fp);
 			}
 		}
 	}
}
#endif

int backup_file(FILE* old_file, const char* new_filename) {
	FILE* new_file = fopen(new_filename, "w");

	if (new_file) {
		fseek(old_file, 0L, SEEK_SET);

		while (1) {
			char ch = fgetc(old_file);
			if (feof(old_file)) {
				break;
			}
			fputc(ch, new_file);
		}
		fclose(new_file);
		return 0;
	}
	return -1;
}

long get_file_size(FILE* fp) {
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}

void save_reading(FILE* fp, const float temperature, const float humidity) {
	fprintf(fp, "%ld, %f, %f\n", time(NULL), temperature, humidity);
}

int get_next_filename(char* filename, size_t size) {
	char temp_filename[MAX_FILENAME_LEN];
	memset(temp_filename, 0x00, sizeof(temp_filename));
	memcpy(temp_filename, temperature_humidity_log_file, FILENAME_END_LOCATION);

	for (int i = 1; ; i++) {
		sprintf(&temp_filename[FILENAME_END_LOCATION], "_%d.log", i);
		FILE* file = fopen(temp_filename, "r");
		if (file) {
			fclose(file);
		}
		else {
			break;
		}
	}

	if (strlen(temp_filename) > size) {
		return -1;
	}
	else {
		strcpy(filename, temp_filename);
		return 0;
	}
}
