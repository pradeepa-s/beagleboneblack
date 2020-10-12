#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include "si7021_sensor.h"
#include <string.h>

static const char temperature_humidity_log_file[] = "/home/debian/si7021_log.log";
static const int MAXIMUM_FILE_SIZE = 500000;  // 500 kB

long get_file_size(FILE* fp);
void save_reading(FILE* fp, const float temperature, const float humidity);
int get_next_filename(char* filename, size_t size);
int backup_file(FILE* old_file, const char* new_filename);

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
 				save_reading(fp, reading.temperature, reading.humidity);
 				if (get_file_size(fp) > MAXIMUM_FILE_SIZE)
 				{
					char backup_filenam[100];
					get_next_filename(backup_filenam, sizeof(backup_filenam));
					if (0 == backup_file(fp, backup_filenam))
					{
						freopen(temperature_humidity_log_file, "w", fp);
					}
 				}
 				fclose(fp);
 			}
 		}
 	}
 
 	si7021_deinit();
	return 0;
}

int backup_file(FILE* old_file, const char* new_filename)
{
	FILE* new_file = fopen(new_filename, "w");

	if (new_file)
	{
		fseek(old_file, 0L, SEEK_END);
		char ch;
		while((ch = fgetc(old_file)) != EOF)
		{
			fputc(ch, new_file);
		}
		fclose(new_file);
		return 0;
	}
	return -1;
}

long get_file_size(FILE* fp)
{
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return size;
}

void save_reading(FILE* fp, const float temperature, const float humidity)
{
	fprintf(fp, "%ld, %f, %f\n", time(NULL), temperature, humidity);
}

int get_next_filename(char* filename, size_t size)
{
	const int FILENAME_END_LOCATION = 23;
	char temp_filename[100];
	memset(temp_filename, 0x00, sizeof(temp_filename));
	memcpy(temp_filename, temperature_humidity_log_file, FILENAME_END_LOCATION);

	for (int i = 1; ; i++)
	{
		sprintf(&temp_filename[FILENAME_END_LOCATION], "_%d.log", i);
		FILE* file = fopen(temp_filename, "r");
		if (file)
		{
			fclose(file);
		}
		else
		{
			break;
		}
	}

	if (strlen(temp_filename) > size)
	{
		return -1;
	}
	else
	{
		strcpy(filename, temp_filename);
		return 0;
	}
}
