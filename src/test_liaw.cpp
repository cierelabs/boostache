#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "liaw2014/renderer.hpp"

using namespace liaw2014;

struct fixture
{
	RenderAST render_template;
	RenderData data;
	std::string expected_output;

	void verify_render()
	{
		BOOST_CHECK_EQUAL(expected_output, Renderer(render_template).render(data));
	}
};

BOOST_FIXTURE_TEST_CASE(test_render_empty_is_empty, fixture)
{
	expected_output = std::string();
	verify_render();
}

BOOST_FIXTURE_TEST_CASE(test_render_of_plain_text, fixture)
{
	render_template.add_literal("This is plain text.");
	expected_output = "This is plain text.";
	verify_render();
}

BOOST_FIXTURE_TEST_CASE(test_render_of_multiple_literals, fixture)
{
	render_template.add_literal("Text 1\n");
	render_template.add_literal("Text 2\n");
	expected_output = "Text 1\nText 2\n";
	verify_render();
}
