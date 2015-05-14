/**
 *  \file django.cpp
 *
 *  A simple example of how to use boostache.
 *
 *  Copyright 2015 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/django.hpp>
#include <boost/boostache/frontend/django/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/model/helper.hpp>
#include <iostream>
#include <sstream>

namespace boostache = boost::boostache;

struct my_node_t;
using map_t = std::map<std::string,my_node_t>;
using list_t = std::vector<my_node_t>;
struct my_node_t : boost::spirit::extended_variant<
     bool
   , std::string
   , map_t
   , list_t
   >
{
   my_node_t() : base_type() {}
   my_node_t(bool rhs) : base_type(rhs) {}
   my_node_t(std::string const & rhs) : base_type(rhs) {}
   my_node_t(char const * rhs) : base_type(std::string{rhs}) {}
   my_node_t(map_t const & rhs) : base_type(rhs) {}
   my_node_t(list_t const & rhs) : base_type(rhs) {}
};

int main()
{
   // ------------------------------------------------------------------
   // Describe the input template. We are going to use django format.
   std::string input(
         "My name is {{another.name}}. "
         "{# This is a comment #}"
         "I am {{pet}} years old."
         "{%% if another.notok %%}"
         "Nope"
         "{%% else %%}"
         "Yep"
         "{%% endif %%}\n");
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // The data model definition
   map_t data = {
      {"contacts" , map_t{{"foo","gorp"}}},
      {"foo"      , "bar"},
      {"me"       , "Daniel"},
      {"pet"      , "turtles"},
      {"lpet"     , "Turtles"},
      {"people"   , list_t{ map_t{{"name"    , "Tom"},
                                  {"job"     , "sweep floors"} },
                            map_t{{"name"    , "Sue"},
                                  {"job"     , "write code"} }
         }
      },
      {"title"      , "Multiple Mustaches"},
      {"comment"    , "this shouldn't be here"},
      {"showme"     , true},
      {"showme2"    , true},
      {"dontshowme" , false},
      {"next_more"  , "I like {{pet}}."},
      {"another"    , map_t{{"name"   , "Sam"},
                            {"ok"     , true },
                            {"not_ok" , false}}
      },
   };
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template.
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::django>(iter, input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to generate the result
   std::stringstream stream;
   boostache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   // print the result
   std::cout << stream.str();
}
