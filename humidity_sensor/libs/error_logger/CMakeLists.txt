add_library(error_logger STATIC error_logger.c)
target_include_directories(error_logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
