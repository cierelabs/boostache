#include <string>
#include <map>
#include <iostream>
#include <fstream>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


#include "test_template_fixture.hpp"
#include "gmock_to_boost.hpp"

using namespace liaw2014;
using namespace std;

struct ChangeDelimiterFixture : public TemplateFixture
{
	ChangeDelimiterFixture()
	{
		template_string = "Hi I am {{name}}.\n";
		template_string += "{{=<\% \%>=}}";
		template_string += "I like <\%pet\%>.";

		set_tag_value("name", "Daniel");
		set_tag_value("pet", "turtles");
		generate_template();
	}

	~ChangeDelimiterFixture()
	{
	}
};

// Tests that a simple mustache tag is replaced
BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(TestChangeDelimiter, 1)
TEST_F(ChangeDelimiterFixture, TestChangeDelimiter)
{
	std::string expected = "Hi I am Daniel.\n";
	expected += "I like turtles.";
	// TODO: EXPECT_EQ(expected, result);
}
