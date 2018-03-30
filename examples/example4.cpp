/**
 *  \file exampe1.cpp
 *
 *  A simple example of how to use boostache.
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

namespace boostache = boost::boostache;


// Our input data type is going to be a map of strings to strings
using map_t = std::map<std::string, std::string>;


int example4()
{
   // ------------------------------------------------------------------
   // Describe the input template. We are going to use mustache format.
   std::string input("\
       <h1>{{header}}< / h1>\n\
   {{#bug}}\n\
   {{ / bug}}\n\
\n\
   {{#items}}\n\
   {{#first}}\n\
   {{#tag}}\n\
   {{#items2}}\n\
   <li><strong>{{name}}< / strong>< / li>\n\
   {{ / items2}}\n\
   {{ / tag}}\n\
   {{ / first}}\n\
   {{#link}}\n\
   <li><a href = \n\"{{url}}\n\">{{name}}< / a>< / li>\n\
   {{ / link}}\n\
   {{ / items}}\n\
\n\
   {{#empty}}\n\
   <p>The list is empty.< / p>\n\
   {{ / empty}}\
");
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // The data description. Just a simple map of strings to strings.
   map_t data = { { "name"  , "Jeroen" },
                  { "age"   , "42"     }
   };
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template.
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;
   using boostache::format::stache;

   auto iter = input.begin();
   auto templ = load_template<stache>(iter, input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to generate the result
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   // print the result
   std::cout << stream.str();

   return 0;
}
