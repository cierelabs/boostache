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
#include <boost/boostache/frontend/file_mapper.hpp>

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
   auto ast = parse<boostache::format::stache>(iter,input.end(), boostache::frontend::file_mapper<char>());

   // we expect everything got parsed
   BOOST_CHECK(iter==input.end());

   std::stringstream stream;
   fe::stache::ast::print(stream, ast);

   std::string expected( "Hello world \n"
                         "{{name}} is here.\n"
                         "{{&escaped_name}} is here\n"
                         "{{#foo}}\n"
                         "Some cool section {{whoot}} is {{foo}} {{bar}} here.\n"
                         "{{/foo}} done.\n"
                         "{{^bar}}\n"
                         "Some cool empty section {{&whoot}} is here.\n"
                         "{{/bar}} done.\n"
                         "{{>partial}}\n"
      );

   BOOST_CHECK_EQUAL(expected,stream.str());
}

