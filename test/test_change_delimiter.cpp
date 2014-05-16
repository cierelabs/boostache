#include <string>
#include <map>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


#include "test_template_fixture.hpp"


// Tests that a simple mustache tag is replaced
BOOST_FIXTURE_TEST_CASE(TestChangeDelimiter, TemplateFixture)
{
	template_string = "Hi I am {{name}}.\n";
	template_string += "{{=<\% \%>=}}";
	template_string += "I like <\%pet\%>.";

	set_tag_value("name", "Daniel");
	set_tag_value("pet", "turtles");

	generate_template();

	std::string expected = "Hi I am Daniel.\n";
	expected += "I like turtles.";
	BOOST_CHECK_EQUAL(expected, result);
}
