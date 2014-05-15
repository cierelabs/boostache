#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "gmock_to_boost.hpp"
#include "test_template_fixture.hpp"


// Tests that a simple mustache tag is replaced
TEST_F(TemplateFixture, TestCollectionsSingle)
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
	// TODO: EXPECT_EQ(expected, result);
}

TEST_F(TemplateFixture, TestCollectionsMultiple)
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
	// TODO: EXPECT_EQ(expected, result);
}

TEST_F(TemplateFixture, TestCollectionMultipleWithMultipleFields)
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


	std::string expected = "Hi I am Daniel.\n";
	expected += "Hi Daniel, I am Tom, I do Accounting.";
	expected += "Hi Daniel, I am Jerry, I do Magic.";
	// TODO: EXPECT_EQ(expected, result);
}
