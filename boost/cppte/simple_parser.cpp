#include <boost/cppte/simple_parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/cppte/frontend/stache_grammar_def.hpp>
#include <iostream>
#include <sstream>

namespace boost { namespace cppte { namespace front_end {
	namespace fe = boost::cppte::front_end;
	namespace qi = boost::spirit::qi;

	template <typename Iterator>
	bool simple_parse(Iterator first, Iterator last, fe::ast::stache_root& ast)
	{
		fe::stache_grammar<Iterator> grammar;

		bool result = qi::phrase_parse( first, last
			, grammar
			, fe::stache_skipper<Iterator>()
			, ast
		);

		if( first != last )
		{
			return false;
		}
		return result;
	}

	bool simple_parse(std::istream& input, ast::stache_root& ast)
	{
		return simple_parse(spirit::istream_iterator(input), spirit::istream_iterator(), ast);
	}

	bool simple_parse(const std::string& input, ast::stache_root& ast)
	{
		std::istringstream i(input);
		i.unsetf(std::ios::skipws);
		return simple_parse(i, ast);
	}
}}}
