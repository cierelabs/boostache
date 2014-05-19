// file used during initial development
// will be removed

#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/frontend/parse.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/stache/printer.hpp>

#include <iostream>

namespace boostache = boost::boostache;
namespace fe = boostache::frontend;
using boostache::frontend::parse;

int main()
{
   std::string input( "Hello world \n"
                      "{{name}} is here.\n"
                      "{{& escaped_name}} is here\n"
                      "{{ # foo}}\n"
                      "Some cool section {{whoot}} is {{foo}} {{bar}} here.\n"
                      "{{/foo}} done.\n"
                      "{{! no comment }}"
                      "{{^ bar}}\n"
                      "Some cool empty section {{{ whoot }}} is here.\n"
                      "{{ /bar}} done.\n"
                      "{{> partial}}\n"
      );

   auto iter = input.begin();

   auto ast = parse<boostache::format::stache>(iter,input.end());
   fe::stache::ast::print(std::cout, ast);

   return -1;
}

