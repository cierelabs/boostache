include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(example1 example1.cpp)
add_executable(example2 example2.cpp)
add_executable(example3 example3.cpp)
add_executable(simple_generate2 simple_generate2.cpp)

# TODO:  This was still in the CMakeLists.txt, but not the Jamfile...
#        is it obsolete?
# add_subdirectory(company_report)


