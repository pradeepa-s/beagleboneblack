#include "reading_logger.h"
#include "sqlite_log_conf.h"
#include <sqlite3.h>
#include <stdio.h>


void log_reading(double t, double h) {
    sqlite3 *db;

    int rc = sqlite3_open(log_vitals_db_name, &db);

    if (rc != SQLITE_OK) {
        printf("Error %d\n\r", rc);
        return;
    }

    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO sensor_readings (temperature, humidity) VALUES (%f, %f)", t, h);

    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Error %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_close(db);
}
