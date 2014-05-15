#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/cppte/frontend/stache_ast.hpp>
#include <boost/cppte/frontend/stache_grammar_def.hpp>
#include <boost/cppte/frontend/stache_printer.hpp>

#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include <iostream>

namespace fe = boost::cppte::front_end;
namespace qi = boost::spirit::qi;

int main()
{
   typedef std::string::iterator iterator_t;
   typedef fe::stache_grammar<iterator_t> grammar_t;

   fe::ast::stache_root ast;
   grammar_t grammar;

   std::string input( "Hello world \n"
                      "{{name}} is here.\n"
                      "{{& escaped_name}} is here\n"
                      "{{ # foo}}\n"
                      "Some cool section {{whoot}} is here.\n"
                      "{{/foo}} done.\n"
                      "{{! no comment }}"
                      "{{^ bar}}\n"
                      "Some cool empty section {{ whoot }} is here.\n"
                      "{{ /bar}} done.\n"
      );

   iterator_t iter = input.begin();
   iterator_t iter_end = input.end();

   if( qi::phrase_parse( iter, iter_end
                       , grammar
                       , qi::space_type()
                       , ast
          ) )
   {
      std::cout << "parse succeeded" << std::endl;
      fe::ast::print(std::cout, ast);
   }
   else
   {
      std::cout << "parse failed" << std::endl;
   }

   return -1;
}

