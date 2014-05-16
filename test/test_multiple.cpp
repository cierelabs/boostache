#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"

// Tests that multiple mustache tags are replaced
BOOST_FIXTURE_TEST_CASE(TestMultipleMustacheFromString, TemplateFixture)
{
	template_string = "<title>{{title}}</title>\n";
	template_string += "Hi I am {{name}}{{lastname}}.\n";
	template_string += "I like {{pet}}.{{!comment}}\n";

	set_tag_value("title", "Multiple Mustaches");
	set_tag_value("name", "Daniel");
	set_tag_value("pet", "turtles");

	generate_template();

	std::string expected = "<title>Multiple Mustaches</title>\n";
	expected += "Hi I am Daniel.\n";
	expected += "I like turtles.\n";
	// TODO: BOOST_CHECK_EQUAL(expected, result);
}
