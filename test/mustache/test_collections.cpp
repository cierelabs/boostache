#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"


// Tests that a simple mustache tag is replaced
BOOST_FIXTURE_TEST_CASE(TestCollectionsSingle, TemplateFixture)
{
	template_string = "Hi I am {{me}}.\n";
	template_string += "{{# people}}";
	template_string += "Hi {{name}}!";
	template_string += "{{/ people}}";

	set_tag_value("me", "Daniel");
	object_t tom;
	tom["name"] = "Tom";
	add_section_item("people", tom);

	generate_template();

	std::string expected = "Hi I am Daniel.\n";
	expected += "Hi Tom!";
	BOOST_CHECK_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(TestCollectionsMultiple, TemplateFixture)
{
	template_string = "Hi I am {{me}}.\n";
	template_string += "{{# people}}";
	template_string += "Hi {{name}}!";
	template_string += "{{/ people}}";

	set_tag_value("me", "Daniel");
	object_t tom;
	tom["name"] = "Tom";
	add_section_item("people", tom);
	object_t jerry;
	jerry["name"] = "Jerry";
	add_section_item("people", jerry);

	generate_template();

	std::string expected = "Hi I am Daniel.\n";
	expected += "Hi Tom!";
	expected += "Hi Jerry!";
	BOOST_CHECK_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(TestCollectionMultipleWithMultipleFields, TemplateFixture)
{
	template_string = "Hi I am {{me}}.\n";
	template_string += "{{# people}}";
	template_string += "Hi {{me}}, I am {{name}}, I do {{work}}.";
	template_string += "{{/ people}}";

	set_tag_value("me", "Daniel");
	object_t tom;
	tom["name"] = "Tom";
	tom["work"] = "Accounting";
	add_section_item("people", tom);

	object_t jerry;
	jerry["name"] = "Jerry";
	jerry["work"] = "Magic";
	add_section_item("people", jerry);

	generate_template();

	std::string expected = "Hi I am Daniel.\n";
	expected += "Hi Daniel, I am Tom, I do Accounting.";
	expected += "Hi Daniel, I am Jerry, I do Magic.";
	BOOST_CHECK_EQUAL(expected, result);
}
