cmake_minimum_required(VERSION 3.10)

project(log_vitals C)

if (LOG_TYPE STREQUAL "File")
    add_executable(${PROJECT_NAME} main.c si7021_sensor.c error_logger.c file_log.c)
elseif (LOG_TYPE STREQUAL "Sql")
    add_executable(${PROJECT_NAME} main.c si7021_sensor.c error_logger.c sqlite_log.c)
    target_link_libraries(${PROJECT_NAME} sqlite3)
else()
    message(FATAL_ERROR "Invalid LOG_TYPE selected. -> ${LOG_TYPE}")
endif()
