#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/boostache/simple_parser.hpp>

namespace fe = boost::boostache::frontend;

BOOST_AUTO_TEST_CASE(parse_empty_input)
{
   fe::stache::ast::root ast;
   BOOST_CHECK(boost::boostache::simple_parse_template("", ast));
   BOOST_CHECK(ast.empty());
}

BOOST_AUTO_TEST_CASE(parse_simple_tag)
{
   using boost::boostache::frontend::stache::ast::variable;
   fe::stache::ast::root ast;
   BOOST_CHECK(boost::boostache::simple_parse_template("{{TAGNAME}}", ast));
   BOOST_REQUIRE_EQUAL(1u, ast.size());
   variable* tag = boost::get<variable>(&ast[0]);
   BOOST_REQUIRE(tag != nullptr);
   BOOST_CHECK_EQUAL("TAGNAME", tag->value);
}

BOOST_AUTO_TEST_CASE(parse_tag_and_text)
{
   fe::stache::ast::root ast;
   BOOST_CHECK(boost::boostache::simple_parse_template("Hello {{NAME}}!", ast));
}

BOOST_AUTO_TEST_CASE(parse_incomplete_tag)
{
   fe::stache::ast::root ast;
   BOOST_CHECK(!boost::boostache::simple_parse_template("{{TAGNAME", ast));
}
