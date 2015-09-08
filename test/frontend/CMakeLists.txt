add_boost_test(adapt_test adapt_test.cpp)
add_boost_test(grammar_basic grammar_basic.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/partial.mustache ${CMAKE_CURRENT_BINARY_DIR}/partial.mustache COPYONLY)
