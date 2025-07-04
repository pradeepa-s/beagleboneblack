#include "reading_logger.h"
#include "file_log_conf.h"
#include "error_logger.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_FILENAME_LEN  (128)
#define MAX_LOG_ENTRY_LEN (100)

static long get_file_size(FILE* fp);
static void save_reading(FILE* fp, const float temperature, const float humidity);
static int get_next_filename(char* filename, size_t size);
static int backup_file(FILE* old_file, const char* new_filename);


void log_reading(double t, double h) {
    FILE* fp;

    fp = fopen(temperature_humidity_log_file, "a+");
    if (fp) {
        save_reading(fp, t, h);
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
