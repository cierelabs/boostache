#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/cppte/simple_parser.hpp>

BOOST_AUTO_TEST_CASE(parse_empty_input)
{
   boost::stache_root ast;
   BOOST_CHECK(boost::cppte::simple_parse_template("", ast));
}
