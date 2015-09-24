include_directories(${CMAKE_SOURCE_DIR}/include)

add_boost_test( render_traits render_traits.cpp)
# TODO:  Does not build, cannot find simple_parser.hpp , which was removed
#
# add_boost_test( test_default_model test_default_model.cpp)
# add_boost_test( test_ptree_model test_ptree_model.cpp)
add_boost_test( traits traits.cpp)

# TODO:  Doesn't link, no matching function for call to 'test'
# add_boost_test( unwrap_variant unwrap_variant.cpp)
