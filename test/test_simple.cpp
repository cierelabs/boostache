#include <string>
#include <map>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"

// Tests that a simple mustache tag is replaced
BOOST_FIXTURE_TEST_CASE(TestSimpleMustacheFromString, TemplateFixture)
{
	template_string = "text {{title}} text";
	set_tag_value("title", "replaced");
	generate_template();

	const std::string expected = "text replaced text";
	// TODO: BOOST_CHECK_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(TestSimpleNotFoundMustacheFromString, TemplateFixture)
{
	template_string = "text {{fitle}} text";
	set_tag_value("title", "replaced");
	generate_template();

	const std::string expected = "text  text";
	// TODO: BOOST_CHECK_EQUAL(expected, result);
}
