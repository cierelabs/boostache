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
  {{#first}}
    <li><strong>{{name}}</strong></li>
  {{/first}}
  {{#link}}
    <li><a href="{{url}}">{{name}}</a></li>
  {{/link}}
{{/items}}

{{#empty}}
  <p>The list is empty.</p>
{{/empty}})";

/*
{
  "header": "Colors",
  "items": [
      {"name": "red", "first": true, "url": "#Red"},
      {"name": "green", "link": true, "url": "#Green"},
      {"name": "blue", "link": true, "url": "#Blue"}
  ],
  "empty": false
}
*/
	set_tag_value("header", "Colors");
	set_tag_value("empty", "false");
	add_section_item("items", { { "name", "red" }, { "first", "true" }, { "url", "#Red" } });
	add_section_item("items", { { "name", "green" }, { "link", "true" }, { "url", "#Green" } });
	add_section_item("items", { { "name", "blue" }, { "link", "true" }, { "url", "#Blue" } });


	generate_template();

	std::string expected = R"(<h1>Colors</h1>
<li><strong>red : Colors</strong></li>
<li><a href="#Green">green</a></li>
<li><a href="#Blue">blue</a></li>)";
	BOOST_CHECK_EQUAL(expected, result);
}
