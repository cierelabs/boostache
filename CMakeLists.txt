cmake_minimum_required(VERSION 2.8)
project(booststache CXX)
enable_testing()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.53 COMPONENTS unit_test_framework REQUIRED)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DBOOST_SPIRIT_USE_PHOENIX_V3=1")

macro(add_boost_test name source)
   ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK) 

   add_executable(${name} ${source})

   target_include_directories(${name} PRIVATE ${Boost_INCLUDE_DIRS})
   target_link_libraries(${name} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LIBRARIES})

   add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(add_boost_test)

add_subdirectory(examples)
add_subdirectory(test)
