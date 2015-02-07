#include <string>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "test_template_fixture.hpp"


BOOST_FIXTURE_TEST_CASE(TestHtmlEscapeMustacheFromString, TemplateFixture)
{
	template_string = "text {{ title }} text\n";
	template_string += "text {{ ritle }} text";
	set_tag_value("title", "<pre>\"\\&foo\\</pre>");
	set_tag_value("ritle", "<pre>\"\\\\&</pre>");

	generate_template();

	std::string expected = "text &lt;pre&gt;&quot;&#92;&amp;foo&#92;&lt;\
									/pre&gt; text\n";
	expected += "text &lt;pre&gt;&quot;&#92;&#92;&amp;&lt;/pre&gt; text";
	BOOST_CHECK_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(TestHtmlUnEscapeMustacheFromString, TemplateFixture)
{
	template_string = "text {{{ title }}} text\n";
	template_string += "text {{& ritle }} text";
	set_tag_value("title", "<pre>\"\\&foo\\</pre>");
	set_tag_value("ritle", "<pre>\"\\\\&</pre>");

	generate_template();

	std::string expected = "text <pre>\"\\&foo\\</pre> text\n";
	expected += "text <pre>\"\\\\&</pre> text";
	BOOST_CHECK_EQUAL(expected, result);
}

