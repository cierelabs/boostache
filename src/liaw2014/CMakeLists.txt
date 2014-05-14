add_custom_target(check COMMAND cd ${CMAKE_SOURCE_DIR}/src && CTEST_OUTPUT_ON_FAILURE=on make all test)
