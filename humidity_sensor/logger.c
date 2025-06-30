#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

const char f_name[] = "/home/pradeepa/sensors.log";

void error_log(const char *fmt, ...)
{
    FILE* fp = fopen(f_name, "a+");
    if (fp) {
        va_list args;
        va_start(args, fmt);
        vfprintf(fp, fmt, args);
        va_end(args);
        fprintf(fp, "\n");
        fclose(fp);
    }
}
