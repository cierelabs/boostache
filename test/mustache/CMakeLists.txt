include_directories(${CMAKE_SOURCE_DIR}/include)

add_boost_test(mustache_compiler mustache_compiler.cpp)
add_boost_test(mustache_end2end mustache_end2end.cpp)
add_boost_test(mustache_parser mustache_parser.cpp)

