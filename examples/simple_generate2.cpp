/**
 *  \file simple_generate2.cpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
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

// ------------ includes for our own model type ------------
#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <map>
#include <list>
#include <type_traits>

namespace boost { namespace boostache { namespace extension
{
   // template <typename T>
   // struct test_category<std::list<T>> 
   //    : mpl::identity<sequence_attribute>
   // {};
}}}
// ---------------------------------------------------------



struct my_type;
using my_object_t = std::map<std::string,my_type>;
using my_list_t = std::list<my_type>;

struct my_type : boost::spirit::extended_variant< 
     bool
   , std::string
   , int
   , my_object_t
   , my_list_t
   >
{
   my_type() : base_type() {}
   my_type(bool rhs) : base_type(rhs) {}
   my_type(std::string const & rhs) : base_type(rhs) {}
   my_type(const char * rhs) : base_type(std::string(rhs)) {}
   my_type(int rhs) : base_type(rhs) {}
   my_type(my_object_t const & rhs) : base_type(rhs) {}
   my_type(my_list_t const & rhs) : base_type(rhs) {}
};


// ----------------------------------------
// ----------------------------------------


namespace bstache = boost::boostache;
namespace extn =  bstache::extension;



int main()
{
   // ------------------------------------------------------------------
   // The input template
   std::string input( 
                      "Hello world \n"
                      "{{name}} is here.\n"
                      "{{& escaped_name}} is here\n"
                      "{{#foo}}\n"
                      "  Some cool section {{whoot}} is here.\n"
                      "  {{^bar}}\n"
                      "    Some cool empty section {{whoot}} is here.\n"
                      "  {{/bar}} done.\n"
                      "{{/foo}} done.\n"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data model definition
   // my_object_t data;
   my_object_t data = {
      {"name","Jeff"},
      {"escaped_name","<h1>Jeff</h1>"},
      {"whoot","yipee"},
      {"bar",false},
      {"foo", my_list_t { my_object_t{{"whoot","yipee 1"}}
                        , my_object_t{{"whoot","yipee 2"},{"bar",true}}
                        , my_object_t{{"whoot","yipee 3"}} }}
   };
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   auto iter = input.begin();
   auto templ = bstache::load_template<bstache::format::stache>(iter,input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::stringstream stream;
   bstache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << stream.str();

   return -1;
}

