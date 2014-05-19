#include <boost/boostache/simple_parser.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <iostream>
#include <sstream>

namespace boost { namespace boostache { namespace frontend
{
   namespace fe = boost::boostache::frontend;
   namespace qi = boost::spirit::qi;

   template <typename Iterator>
   bool simple_parse(Iterator first, Iterator last, fe::stache::ast::root& ast)
   {
      fe::stache::grammar<Iterator> grammar;

      bool result = qi::phrase_parse( first, last
                                    , grammar
                                    , qi::space_type()
                                    , ast
         );

      if( first != last )
      {
         return false;
      }
      return result;
   }

   bool simple_parse(std::istream& input, stache::ast::root& ast)
   {
      return simple_parse(spirit::istream_iterator(input), spirit::istream_iterator(), ast);
   }

   bool simple_parse(const std::string& input, stache::ast::root& ast)
   {
      std::istringstream i(input);
      i.unsetf(std::ios::skipws);
      return simple_parse(i, ast);
   }
}}}
