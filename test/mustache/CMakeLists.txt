foreach( ex mustache_parser mustache_compiler mustache_end2end )
    add_boost_test( ${ex} ${ex}.cpp ../shared/parser_test.cpp )
    target_link_libraries( ${ex} Boost::filesystem )
endforeach()
