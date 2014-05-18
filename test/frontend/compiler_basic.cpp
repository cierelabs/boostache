// file used during initial dev
// will be removed
//
#include <boost/spirit/include/qi.hpp>
#include <boost/boostache/frontend/stache/ast.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/stache/printer.hpp>
#include <boost/boostache/backend/stache_compiler.hpp>
#include <boost/boostache/vm/printer.hpp>
#include <iostream>


namespace fe = boost::boostache::frontend;
namespace vm = boost::boostache::vm;
namespace qi = boost::spirit::qi;

int main()
{
   typedef std::string::iterator iterator_t;
   typedef fe::stache::grammar<iterator_t> grammar_t;

   fe::stache::ast::root ast;
   grammar_t grammar;

   std::string input( "Hello world \n"
                      "{{name}} is here.\n"
                      "{{& escaped_name}} is here\n"
                      "{{#foo}}\n"
                      "Some cool section {{whoot}} is here.\n"
                      "{{^bar}}\n"
                      "Some cool empty section {{whoot}} is here.\n"
                      "{{/bar}} done.\n"
                      "{{/foo}} done.\n"
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
      std::cout << "------------------------------" << std::endl;
      fe::stache::ast::print(std::cout, ast);
      std::cout << "------------------------------" << std::endl;
      std::cout << std::endl;
      std::cout << "compile" << std::endl;
      std::cout << "------------------------------" << std::endl;
      auto engine_ast = boost::boostache::backend::compile(ast);
      std::cout << "------------------------------" << std::endl;
      std::cout << std::endl;
      std::cout << "print engine_ast" << std::endl;
      vm::ast::print(std::cout,engine_ast);
      std::cout << "------------------------------" << std::endl;
   }
   else
   {
      std::cout << "parse failed" << std::endl;
   }

   return -1;
}

