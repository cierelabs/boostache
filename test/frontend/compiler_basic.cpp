// file used during initial dev
// will be removed
//
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/frontend/parse.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/stache/printer.hpp>

#include <boost/boostache/backend/stache_compiler.hpp>
#include <boost/boostache/vm/printer.hpp>
#include <iostream>


namespace boostache = boost::boostache;
namespace fe = boostache::frontend;
namespace vm = boostache::vm;
using boostache::frontend::parse;


int main()
{
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

   auto iter = input.begin();

   auto ast = parse<boostache::format::stache>(iter,input.end());

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

   return -1;
}

