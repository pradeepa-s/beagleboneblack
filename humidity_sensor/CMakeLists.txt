cmake_minimum_required(VERSION 3.10)

project(LogVitals C)

add_executable(${PROJECT_NAME} main.c si7021_sensor.c logger.c)
