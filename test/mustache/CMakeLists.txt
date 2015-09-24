add_boost_test( mustache_parser mustache_parser.cpp     ../shared/parser_test.cpp )
add_boost_test( mustache_compiler mustache_compiler.cpp ../shared/parser_test.cpp )
add_boost_test( mustache_end2end mustache_end2end.cpp   ../shared/parser_test.cpp )
