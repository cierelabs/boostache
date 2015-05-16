cmake_minimum_required(VERSION 2.8)

macro(add_boost_test_core name source)
add_executable( ${ARGV} )
target_link_libraries(${name} ${Boost_LIBRARIES})
add_test(NAME ${name} COMMAND ${name})
endmacro(add_boost_test_core)

# There are two styles of test represented here
# The first is in a single file and needs BOOST_TEST_DYN_LINK set
macro(add_boost_test name source)
add_boost_test_core(${ARGV})
set_source_files_properties( ${source} PROPERTIES COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK )
endmacro(add_boost_test)

add_subdirectory(vm)
add_subdirectory(frontend)
add_subdirectory(model)
# The second test style is defined and used exclusively within the "mustache" directory
add_subdirectory(mustache)
