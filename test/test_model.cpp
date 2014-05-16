#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/cppte/frontend/stache_model.hpp>
#include <boost/cppte/frontend/stache_model_printer.hpp>
#include <boost/cppte/simple_parser.hpp>

using namespace boost::cppte::front_end;

namespace
{
	std::string print(const ast::stache_root& ast, const stache_model& model)
	{
		std::ostringstream out;
		boost::cppte::front_end::ast::print(out, ast, model);
		return out.str();
	}

	ast::stache_root parse(const std::string& text)
	{
		ast::stache_root ast;
		if( !boost::simple_parse_template(text, ast) )
		{
			throw std::runtime_error("Parse failed");
		}
		return ast;
	}
}

BOOST_AUTO_TEST_CASE(test_model_build)
{
	stache_model model;
	model["NAME"] = "Bob";
	model["LOCATION"] = stache_model { { "CITY", "Aspen" }, { "STATE", "Colorado" }, { "COUNTRY", "United States" } };
	model["GENERATED"] = []() { return "abc"; };

	BOOST_CHECK_EQUAL("Bob", boost::get<std::string>(model["NAME"]));
	BOOST_CHECK_THROW(boost::get<int>(model["NAME"]), boost::bad_get);
	BOOST_CHECK_EQUAL("Aspen", boost::get<std::string>(boost::get<stache_model>(model["LOCATION"])["CITY"]));
	BOOST_CHECK_EQUAL("abc", boost::get<stache_function>(model["GENERATED"])());
}

BOOST_AUTO_TEST_CASE(test_simple_model_formatting)
{
	stache_model model;
	model["NAME"] = "Boosties";
	ast::stache_root ast = parse("Hello, {{NAME}}!");
	std::string result = print(ast, model);
	// FIXME! This should have whitespace, but it looks like the parser is stripping it.
	BOOST_CHECK_EQUAL("Hello,"
		"" // <- This should be a space!
		"Boosties!", result);
}

BOOST_AUTO_TEST_CASE(test_section_printing)
{
	stache_model model;
	model["USER"] = stache_model {
		{ "NAME", "Bob" },
		{ "LOCATION", "Earth" },
		{ "FAVORITES", stache_model {
				{ "FOOD", "Pizza" },
				{ "MUSIC", "Classical" }
			}
		}
	};
	ast::stache_root ast = parse(
		"{{#USER}}"
		"user.name={{NAME}}\n"
		"user.location={{LOCATION}}\n"
		"{{#FAVORITES}}"
		"user.favorite.food={{FOOD}}\n"
		"user.favorite.music={{MUSIC}}\n"
		"{{/FAVORITES}}"
		"{{/USER}}");

	// FIXME! Whitespace!
	// Should have a trailing newline on all of these lines.
	BOOST_CHECK_EQUAL(
		"user.name=Bob"
		"user.location=Earth"
		"user.favorite.food=Pizza"
		"user.favorite.music=Classical",
		print(ast, model));
}
