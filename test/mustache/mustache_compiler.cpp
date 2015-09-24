/**
 *  \file test/mustache/mustache_compiler.cpp
 *
 *  Link with shared/parser_test to utilize loading and parsing test files.
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/parse.hpp>

#include <boost/boostache/backend/stache_compiler.hpp>
#include <boost/boostache/vm/printer.hpp>

#include <string>
#include <sstream>
#include <fstream>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace boostache = boost::boostache;
namespace fe = boost::boostache::frontend;

std::string print_ast(std::string const & filename)
{
   std::ifstream file(filename.c_str());
   if(!file)
   {
      BOOST_CHECK_MESSAGE(false, "Failed to open " << filename);
      return "";
   }

   std::ifstream istream(filename.c_str());
   auto ast = fe::parse<boostache::format::stache>(istream);
   auto engine_ast = boostache::backend::compile(ast);

   std::ostringstream stream;
   boostache::vm::ast::print(stream,engine_ast);
   return stream.str();
}


std::string test_dir = "mustache/compiler_test_dir";
