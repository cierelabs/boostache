/**
 *  \file test/mustache/mustache_parser.cpp
 *
 *  Link with shared/parser_test to test the mustache parser
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/test/unit_test.hpp>
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp>
#include <boost/boostache/frontend/stache/ast.hpp>
#include <boost/boostache/frontend/stache/printer.hpp>
#include <boost/boostache/frontend/file_mapper.hpp>
#include <boost/boostache/frontend/parse.hpp>
#include <string>
#include <sstream>
#include <fstream>

namespace bstache = boost::boostache;
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
   auto ast = fe::parse<bstache::format::stache>(istream, bstache::frontend::file_mapper<char>());
   std::ostringstream stream;
   fe::stache::ast::print(stream,ast);
   return stream.str();
}


std::string test_dir = "mustache/parser_test_dir";
