#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"

// Tests that a simple mustache tag is replaced
BOOST_FIXTURE_TEST_CASE(TestInvertedSectionMustacheFromString, TemplateFixture)
{
	template_string = "Hi I am {{name}}.\n";
	template_string += "{{# showme}}";
	template_string += "I like {{pet}}.";
	template_string += "{{/ showme}}";
	template_string += "{{^ inverted}}";
	template_string += "Hope you can see me.";
	template_string += "{{/ inverted}}";
	template_string += "{{^ inverted2}}";
	template_string += "me too.";
	template_string += "{{/ inverted2}}";
	template_string += "{{^ stealth}}";
	template_string += "I'm invisible.";
	template_string += "{{/ stealth}}";

	set_tag_value("name", "Daniel");
	set_tag_value("pet", "turtles");
	set_tag_value("showme", true);
	set_tag_value("stealth", true);
	set_tag_value("inverted", false);

	generate_template();

	std::string expected = "Hi I am Daniel.\n";
	expected += "I like turtles.";
	expected += "Hope you can see me.";
	expected += "me too.";
	BOOST_CHECK_EQUAL(expected, result);
}

