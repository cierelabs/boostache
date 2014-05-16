// file used during initial dev
// will be removed
//
#include <boost/spirit/include/qi.hpp>
#include <boost/cppte/frontend/stache_ast.hpp>
#include <boost/cppte/frontend/stache_grammar_def.hpp>
#include <boost/cppte/frontend/stache_printer.hpp>
#include <boost/cppte/backend/stache_compiler.hpp>
#include <boost/cppte/vm/printer.hpp>
#include <iostream>


namespace fe = boost::cppte::front_end;
namespace vm = boost::cppte::vm;
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
      fe::ast::print(std::cout, ast);
      std::cout << "------------------------------" << std::endl;
      std::cout << std::endl;
      std::cout << "compile" << std::endl;
      std::cout << "------------------------------" << std::endl;
      auto engine_ast = boost::cppte::backend::compile(ast);
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

