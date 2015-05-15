# The second test style spans two source files, one of which defines BOOST_TEST_DYN_LINK internally,
# so we don't need to set that.  However, the tests expect to be in a particular working directory
# and will fail if we don't execute from there
macro(add_boost_test_twopart name source)
add_boost_test_core(${ARGV})
set_tests_properties( ${name} PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/.. )
endmacro(add_boost_test_twopart)

add_boost_test_twopart( mustache_parser mustache_parser.cpp ../shared/parser_test.cpp )
add_boost_test_twopart( mustache_compiler mustache_compiler.cpp ../shared/parser_test.cpp )
add_boost_test_twopart( mustache_end2end mustache_end2end.cpp ../shared/parser_test.cpp )
