cmake_minimum_required(VERSION 2.8)

cmake_policy(SET CMP0002 OLD)
add_custom_target(check COMMAND cd ${CMAKE_SOURCE_DIR}/src && CTEST_OUTPUT_ON_FAILURE=on make all test)
