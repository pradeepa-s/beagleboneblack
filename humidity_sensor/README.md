# README

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
5. Create a schema file `init.sql`.

```sql
CREATE TABLE sensor_readings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    temperature REAL,
    humidity REAL
);
```

6. Create the database using sqlite3.

```bash
sudo mkdir /var/lib/log_vitals
sudo chown <user_name>:<user_name> /var/lib/log_vitals  <-- Please update user_name
sqlite3 /var/lib/log_vitals/data.db < init.sql
```

7. Run the log_vitals executable to log the reading in the database.

8. Use the following to query the readings.

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

# How to reset the database?

```bash
sqlite3 /var/lib/log_vitals/data.db "DELETE FROM sensor_readings;"
sqlite3 /var/lib/log_vitals/data.db "DELETE FROM sqlite_sequence WHERE name='sensor_readings';"
```
