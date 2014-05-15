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

cmake_policy(SET CMP0002 OLD)
add_custom_target(check COMMAND cd ${CMAKE_SOURCE_DIR}/src && CTEST_OUTPUT_ON_FAILURE=on make all test)
add_subdirectory(liaw2014)
