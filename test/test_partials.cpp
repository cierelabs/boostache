#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"

// Tests that a simple mustache tag is replaced
BOOST_FIXTURE_TEST_CASE(TestPartialsFromString, TemplateFixture)
{
	template_string = "Hi I am {{name}}.\n";
	template_string += "{{> next_more}}\n";
	template_string += "What do I like? {{lpet}}!!";

	register_partial_template("next_more", "I like {{pet}}.");

	set_tag_value("name", "Daniel");
	set_tag_value("pet", "turtles");
	set_tag_value("lpet", "Turtles");

	generate_template();

	std::string expected = "Hi I am Daniel.\n";
	expected += "I like turtles.\n";
	expected += "What do I like? Turtles!!";
	// TODO: BOOST_CHECK_EQUAL(expected, result);
}
