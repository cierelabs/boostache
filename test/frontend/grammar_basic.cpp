/**
 *  \file frontend/grammar_basic.cpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Basic stache gramar test
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/frontend/parse.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/stache/printer.hpp>

#include <sstream>
#include <string>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace boostache = boost::boostache;
namespace fe = boostache::frontend;
using boostache::frontend::parse;

BOOST_AUTO_TEST_CASE(stache_parse_test)
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

   // we expect everything got parsed
   BOOST_CHECK(iter==input.end());

   std::stringstream stream;
   fe::stache::ast::print(stream, ast);

   std::string expected( "Hello world <eol>\n"
                         "{{name}}<blank_text> is here.<eol>\n"
                         "{{&escaped_name}}<blank_text> is here<eol>\n"
                         "{{#foo}}<eol>\n"
                         "Some cool section {{whoot}}<blank_text> is {{foo}}<blank_text> {{bar}}<blank_text> here.<eol>\n"
                         "{{/foo}}<blank_text> done.<eol>\n"
                         "<comment>{{^bar}}<eol>\n"
                         "Some cool empty section {{&whoot}}<blank_text> is here.<eol>\n"
                         "{{/bar}}<blank_text> done.<eol>\n"
                         "{{>partial}}<eol>\n"
      );

   auto input_ast = stream.str();
   auto diff_iters = std::mismatch(input_ast.begin(), input_ast.end(),
                                   expected.begin(), expected.end());

   if(std::get<0>(diff_iters) != input_ast.end())
   {
      auto iter = std::get<0>(diff_iters);
      std::cout << "generated difference at:\n"
                << "   offset: " << std::distance(input_ast.begin(), iter) << "\n"
                << "   value: " << *iter << "\n"
                << "   value: 0x" << std::hex << int(*iter) << std::dec << "\n";
   }

   if(std::get<1>(diff_iters) != expected.end())
   {
      auto iter = std::get<1>(diff_iters);
      std::cout << "expected difference at:\n"
                << "   offset: " << std::distance(expected.begin(), iter) << "\n"
                << "   value: " << *iter << "\n"
                << "   value: 0x" << std::hex << int(*iter) << std::dec << "\n";
   }

   BOOST_CHECK_EQUAL(input_ast,expected);
}

