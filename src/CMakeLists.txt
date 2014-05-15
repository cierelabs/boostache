cmake_minimum_required(VERSION 2.8)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(.)

add_library(liaw STATIC
  liaw2014/renderer.hpp
  liaw2014/renderer.cpp
  liaw2014/render_ast.hpp
  liaw2014/render_ast.cpp
  liaw2014/render_data.hpp
)

macro(add_liaw_test name source)
add_executable(${name} ${source})
add_dependencies(${name} liaw)
target_include_directories(${name} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${name} liaw boost_unit_test_framework)
add_test(${name} ${name})
endmacro(add_liaw_test)

add_liaw_test(test_liaw test_liaw.cpp)
add_liaw_test(test_change_delimiter test_change_delimiter.cpp)
add_liaw_test(test_collections_plustache test_collections_plustache.cpp)
add_liaw_test(test_sections_plustache test_sections_plustache.cpp)
add_liaw_test(test_simple_plustache test_simple_plustache.cpp)
add_liaw_test(test_multiple_plustache test_multiple_plustache.cpp)
add_liaw_test(test_nested_sections_plustache test_nested_sections_plustache.cpp)
add_liaw_test(test_partials_plustache test_partials_plustache.cpp)

#add_executable(other_tests
  #  test_html_escape_plustache.cpp
  #test_inverted_sections.cpp
#)

cmake_policy(SET CMP0002 OLD)
add_custom_target(check COMMAND cd ${CMAKE_SOURCE_DIR}/src && CTEST_OUTPUT_ON_FAILURE=on make all test)
add_subdirectory(liaw2014)
