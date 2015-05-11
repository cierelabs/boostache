/**
 *  \file exampe1.cpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/stache.hpp>
#include <boost/boostache/model/helper.hpp>
#include <iostream>
#include <sstream>

namespace bstache = boost::boostache;
namespace extn =  bstache::extension;


using map_t = std::map<std::string, std::string>;


int main()
{
   // ------------------------------------------------------------------
   // The input template

   std::string input("My name is {{name}}. I am {{age}} years old.\n");

   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description

   map_t data = { { "name"  , "Jeroen" },
                  { "age"   , "42"     }
   };

   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure

   auto iter = input.begin();
   auto templ = bstache::load_template<bstache::format::stache>( iter
                                                               , input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::stringstream stream;
   bstache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << stream.str();
}

