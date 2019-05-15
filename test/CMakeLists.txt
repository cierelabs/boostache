cmake_minimum_required(VERSION 2.8)

macro(add_boost_test name source)
   add_executable( ${ARGV} )

   # compilation setup
   set_source_files_properties( ${source} PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK )

   # link setup
   target_link_libraries(
     ${name}
     ${Boost_LIBRARIES}
     ${Boost_SYSTEM_LIBRARY}
     ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
     )

   # unit test configuration
   add_test(NAME ${name} COMMAND ${name})
   set_tests_properties( ${name} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.. )
endmacro(add_boost_test)

add_subdirectory(vm)
add_subdirectory(frontend)
add_subdirectory(model)
add_subdirectory(mustache)
