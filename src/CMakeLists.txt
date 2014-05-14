cmake_minimum_required(VERSION 2.8)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_VERBOSE_MAKEFILES ON)
include_directories(.)

add_library(liaw STATIC
  liaw2014/renderer.hpp
  liaw2014/renderer.cpp
  liaw2014/render_ast.hpp
  liaw2014/render_ast.cpp
  liaw2014/render_data.hpp
)

add_executable(test_liaw
  test_liaw.cpp
  test_change_delimiter.cpp
  test_collections_plustache.cpp
  #  test_html_escape_plustache.cpp
  #test_inverted_sections.cpp
  #test_multiple_plustache.cpp
  #test_nested_sections_plustache.cpp
  #test_partials_plustache.cpp
  #test_sections_plustache.cpp
  test_simple_plustache.cpp
)
add_dependencies(test_liaw liaw)
target_include_directories(test_liaw PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_liaw liaw boost_unit_test_framework)
add_test(test_liaw test_liaw)

add_subdirectory(liaw2014)
