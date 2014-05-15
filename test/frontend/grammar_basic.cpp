#include <boost/spirit/include/qi.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>
#include <boost/cppte/frontend/stache_grammar_def.hpp>
#include <boost/cppte/frontend/stache_printer.hpp>
#include <iostream>


namespace fe = boost::cppte::front_end;
namespace qi = boost::spirit::qi;

int main()
{
   using fe::stache_skipper;

   typedef std::string::iterator iterator_t;
   typedef fe::stache_grammar<iterator_t> grammar_t;

   fe::ast::stache_root ast;
   grammar_t grammar;

   std::string input( "Hello world \n"
                      "{{name}} is here.\n"
                      "{{& escaped_name}} is here\n"
                      "{{#foo}}\n"
                      "Some cool section {{whoot}} is here.\n"
                      "{{/foo}} done.\n"
                      "{{^bar}}\n"
                      "Some cool empty section {{whoot}} is here.\n"
                      "{{/bar}} done.\n"
      );

   iterator_t iter = input.begin();
   iterator_t iter_end = input.end();

   if( qi::phrase_parse( iter, iter_end
                       , grammar
                       , stache_skipper<iterator_t>()
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

   return 0;
}

