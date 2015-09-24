cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(example1 example1.cpp)
target_link_libraries(example1 boost_boostache)

add_executable(example2 example2.cpp)
target_link_libraries(example2 boost_boostache)

add_executable(example3 example3.cpp)
target_link_libraries(example3 boost_boostache)
