#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "liaw2014/renderer.hpp"

using namespace liaw2014;

BOOST_AUTO_TEST_CASE(test_render_empty_is_empty)
{
	RenderAST render_template;
	RenderData data;
	Renderer renderer(render_template);

	BOOST_CHECK_EQUAL(std::string(), renderer.render(data));
}

BOOST_AUTO_TEST_CASE(test_render_of_plain_text)
{
	RenderAST render_template;
	render_template.set_template_text("This is plain text.");
	RenderData data;
	Renderer renderer(render_template);

	BOOST_CHECK_EQUAL("This is plain text.", renderer.render(data));
}
