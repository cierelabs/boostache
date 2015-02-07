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
	template_string =
		R"(<h1>{{header}}</h1>
{{#bug}}
{{/bug}}

{{#items}}
  {{#first}}<li><strong>{{name}}</strong></li>{{/first}}
  {{#link}}<li><a href="{{url}}">{{name}}</a></li>{{/link}}
{{/items}}

{{#empty}}
  <p>The list is empty.</p>
{{/empty}})";


	set_tag_value("header", "Colors");
	set_tag_value("empty", false);
        std::function<bool()> visible = []() { return true; };
        add_section_item("items", { { "name", "red" }, { "first", visible }, { "url", "#Red" } });
	add_section_item("items", { { "name", "green" }, { "link", visible }, { "url", "#Green" } });
	add_section_item("items", { { "name", "blue" }, { "link", visible }, { "url", "#Blue" } });

	generate_template();

	// Mustache is stripping newlines where it shouldn't be.
	std::string expected_if_whitespace_trimmed =
		"<h1>Colors</h1>\n"
		"<li><strong>red</strong></li>\n"
		"<li><a href=\"#Green\">green</a></li>\n"
		"<li><a href=\"#Blue\">blue</a></li>\n";


	std::string expected_if_whitespace_not_trimmed =
		"<h1>Colors</h1>\n\n\n\n"
		"  <li><strong>red</strong></li>\n  \n\n  \n"
		"  <li><a href=\"#Green\">green</a></li>\n\n  \n"
		"  <li><a href=\"#Blue\">blue</a></li>\n\n\n";

	BOOST_CHECK_EQUAL(expected_if_whitespace_not_trimmed, result);
}
