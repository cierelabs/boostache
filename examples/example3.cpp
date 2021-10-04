/**
 *  \file example2.cpp
 *
 *  This is almost like example2, except we are going to
 *  clean things up with a variant.
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
#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>


namespace boostache = boost::boostache;


// -------------------------------------------------------
// From example2 we realize that if we had a data structure
// that could map strings to a list, string, or even another
// map then we could represent a much more complete invoice.
// A key/value map with the value being able to store these
// different types starts to look like an object in some
// languages.
// This is fairly simple to do with a variant. We are going
// to use the spirit version of it because it makes
// recursive structures easier to describe.
//

struct value_t;
using object_t = std::map<std::string, value_t>;
using list_t = std::vector<value_t>;

struct value_t : boost::spirit::extended_variant< std::string
                                                , object_t
                                                , list_t
                                                >
{
   value_t() : base_type() {}
   value_t(std::string const & rhs) : base_type(rhs) {}
   value_t(char const * rhs) : base_type(std::string{rhs}) {}
   value_t(object_t const & rhs) : base_type(rhs) {}
   value_t(list_t const & rhs) : base_type(rhs) {}
};
// -------------------------------------------------------


int example3()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input( 
                      "Invoice {{invoice_number}}"
                      "\n"
                      "{{# company}}"
                      "Company: {{name}}\n"
					   "Invoice again: {{invoice_number}}\n"
					   "{{#invoice_number}}\n"
					   "has invoice\n"
					   "{{/invoice_number}}\n"
					   "{{#paied}}\n"
					   "has paied\n"
					   "{{/paied}}\n"
					   "{{^paied}}\n"
					   "has not paied\n"
					   "{{/paied}}\n"
					   "         {{street}}\n"
                      "         {{city}}, {{state}}  {{zip}}\n"
                      "{{/ company}}"
                      "------------------------------------------------\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description.

   object_t invoice = 
      {{"invoice_number", "1234"},
       {"company"       , object_t{{"name"   , "FizSoft"},
                                   {"street" , "42 Level St."},
                                   {"city"   , "Ytic"},
                                   {"state"  , "CA"},
                                   {"zip"    , "98765"} }},
       {"lines"         , list_t{ object_t{{"item_code"    , "1234-2"},
                                           {"description"  , "Jolt Case"},
                                           {"amount"       , "$23"}},
                                  object_t{{"item_code"    , "1235-1"},
                                           {"description"  , "Computer"},
                                           {"amount"       , "$9"}}   }}
   };
                                                      
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Load the template
   // This parses the input and compiles the result. The return is the
   // compiled data structure
   using boostache::load_template;

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end());
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::stringstream stream;
   boostache::generate(stream, templ, invoice);
   // ------------------------------------------------------------------

   std::cout << stream.str();

   return 0;
}
