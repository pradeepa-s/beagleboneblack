# README

## Overview

This repository builds the `log_vitals` application.
`log_vitals` reads data from sensors connected to the device and records the readings in database/s or file/s.

## Structure of the repository

```
.
├── conf
│   ├── database        <- The database schema file.
│   ├── file_conf       <- File configuration. (When vitals are logged to file)
│   ├── si7021          <- si7021 configurations.
│   └── sqlite_conf     <- Sqlite configuration. (When vitals are logged using sqlite)
├── libs
│   ├── error_logger    <- Error logger library used to log errors
│   ├── reading_logger  <- Reading logger library supports logging a reading to either a file or database.
│   └── si7021          <- si7021 sensor interface
└── src                 <- Main executable
```

## Build and deploy with sqllite3

1. Install sqlite3

```bash
sudo apt install sqlite3 libsqlite3-dev
```

2. Build the project.

```bash
mkdir build
cmake -S . -B build -DLOG_TYPE=Sql
cmake --build build
```

4. The database name and location is in `sqlite_log_conf.h` file.
5. The schema file is `init.sql`.
6. Create the database using sqlite3. (Using defaults)

```bash
sudo mkdir /var/lib/log_vitals
sudo chown <user_name>:<user_name> /var/lib/log_vitals  <-- Please update user_name
sqlite3 /var/lib/log_vitals/data.db < init.sql
```

7. Run the log_vitals executable to log the reading in the database.
8. Use the following to query the last 10 entries.

```bash
sqlite3 /var/lib/log_vitals/data.db "SELECT * FROM sensor_readings ORDER BY timestamp DESC LIMIT 10;"
```

9. Copy the log_vitals into `/usr/local/bin`

```bash
cp build/log_vitals /usr/local/bin/
```

10. Setup the crontab to run the job every 30 mins.

```
crontab -e

*/30 * * * * /usr/local/bin/log_vitals
```

### Reset the database

```bash
sqlite3 /var/lib/log_vitals/data.db "DELETE FROM sensor_readings;"
sqlite3 /var/lib/log_vitals/data.db "DELETE FROM sqlite_sequence WHERE name='sensor_readings';"
```

## Build and deploy with file

1. Build the project.

```bash
mkdir build
cmake -S . -B build -DLOG_TYPE=File
cmake --build build
```

2. The file configurations are in `file_log_conf.h`
3. Run the log_vitals executable to log the reading in the file.
4. Follow the steps in sqlite section to add a cron job.
