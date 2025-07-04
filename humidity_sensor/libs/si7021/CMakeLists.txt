add_library(si7021 STATIC si7021_sensor.c)
target_include_directories(si7021 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
