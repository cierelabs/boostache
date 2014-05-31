/**
 *  \file frontend/compiler_basic.cpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Basic compiler test
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/frontend/parse.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/stache/printer.hpp>

#include <boost/boostache/backend/stache_compiler.hpp>
#include <boost/boostache/vm/printer.hpp>

#include <sstream>
#include <string>
#include <iostream>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>


namespace boostache = boost::boostache;
namespace fe = boostache::frontend;
namespace vm = boostache::vm;
using boostache::frontend::parse;


BOOST_AUTO_TEST_CASE(stache_parse_compile_test)
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

   // we expect everything got parsed
   BOOST_CHECK(iter==input.end());

   auto engine_ast = boost::boostache::backend::compile(ast);

   std::stringstream stream;
   fe::stache::ast::print(stream, ast);
   vm::ast::print(stream,engine_ast);

   std::string expected( "Hello world \n"
                         "{{name}} is here.\n"
                         "{{&escaped_name}} is here\n"
                         "{{#foo}}\n"
                         "Some cool section {{whoot}} is here.\n"
                         "{{^bar}}\n"
                         "Some cool empty section {{whoot}} is here.\n"
                         "{{/bar}} done.\n"
                         "{{/foo}} done.\n"
                         "<literal node> : Hello world \n"
                         "\n"
                         "<render> : name\n"
                         "<literal node> :  is here.\n"
                         "\n"
                         "<render> : escaped_name\n"
                         "<literal node> :  is here\n"
                         "\n"
                         "<if> : --------------------------- \n"
                         "<then> : --------------------------- \n"
                         "<literal node> : \n"
                         "Some cool section \n"
                         "<render> : whoot\n"
                         "<literal node> :  is here.\n"
                         "\n"
                         "<if> : --------------------------- \n"
                         "<then> : --------------------------- \n"
                         "WHOA! we have an undefined\n"
                         "<else> : --------------------------- \n"
                         "<literal node> : \n"
                         "Some cool empty section \n"
                         "<render> : whoot\n"
                         "<literal node> :  is here.\n"
                         "\n"
                         "-------------------------------------\n"
                         "<literal node> :  done.\n"
                         "\n"
                         "<else> : --------------------------- \n"
                         "WHOA! we have an undefined\n"
                         "-------------------------------------\n"
                         "<literal node> :  done.\n"
                         "\n"
      );

   BOOST_CHECK_EQUAL(expected,stream.str());
}

