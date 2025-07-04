cmake_minimum_required(VERSION 3.10)

project(log_vitals C)

add_subdirectory(conf/file_conf)
add_subdirectory(conf/sqlite_conf)
add_subdirectory(conf/si7021)
add_subdirectory(libs/error_logger)
add_subdirectory(libs/reading_logger)
add_subdirectory(libs/si7021)

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE si7021 si7021_conf reading_logger error_logger)
