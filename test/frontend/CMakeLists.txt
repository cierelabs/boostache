include_directories(${CMAKE_SOURCE_DIR}/include)

add_boost_test(adapt_test adapt_test.cpp)
add_boost_test(grammar_basic grammar_basic.cpp)
