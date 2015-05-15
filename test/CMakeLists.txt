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

add_boost_test( foreach vm/foreach.cpp )

add_subdirectory(frontend)

add_boost_test( traits model/traits.cpp )
add_boost_test( render_traits model/render_traits.cpp )

# The second test style spans two source files, one of which defines BOOST_TEST_DYN_LINK internally,
# so we don't need to set that.  However, the tests expect to be in a particular working directory
# and will fail if we don't execute from there
macro(add_boost_test_twopart name source)
add_boost_test_core(${ARGV})
set_tests_properties( ${name} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
endmacro(add_boost_test_twopart)

add_boost_test_twopart( mustache_parser mustache/mustache_parser.cpp shared/parser_test.cpp )
add_boost_test_twopart( mustache_compiler mustache/mustache_compiler.cpp shared/parser_test.cpp )
add_boost_test_twopart( mustache_end2end mustache/mustache_end2end.cpp shared/parser_test.cpp )


