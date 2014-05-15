#include <string>
#include <map>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "gmock_to_boost.hpp"
#include "test_template_fixture.hpp"

// Tests that a simple mustache tag is replaced
TEST_F(TemplateFixture, TestSimpleMustacheFromString)
{
	template_string = "text {{title}} text";
	set_tag_value("title", "replaced");
	generate_template();

	const std::string expected = "text replaced text";
	// TODO: EXPECT_EQ(expected, result);
}

TEST_F(TemplateFixture, TestSimpleNotFoundMustacheFromString)
{
	template_string = "text {{fitle}} text";
	set_tag_value("title", "replaced");
	generate_template();

	const std::string expected = "text  text";
	// TODO: EXPECT_EQ(expected, result);
}
