/**
 *  \file example2.cpp
 *
 *  This is almost like example2, except we are going to
 *  clean things up with a variant.
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *  Copyright 2015 Michele Santullo
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/stache.hpp>
#include <boost/boostache/frontend/file_mapper.hpp>
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
// We also add a partial at the end. Note that the partial
// itself can contain more boostache (see footer.mst).
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


int main()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input(
                      "Invoice {{invoice_number}}\n"
                      "\n"
                      "{{# company}}"
                      "Company: {{name}}\n"
                      "         {{street}}\n"
                      "         {{city}}, {{state}}  {{zip}}\n"
                      "{{/ company}}"
                      "------------------------------------------------\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}\n"
                      "- {{>footer}} -"
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
   using boostache::frontend::file_mapper;

   //The file mapper coming with boostache  will try to read from a file
   //called <work_dir>/<partial_name>.<ext>
   //By default work_dir is the empty string (current directory), and ext
   //is ".mustache". In this example we change it to ".mst".
   //You can customize how partials are resolved by providing your own
   //mapper.
   boostache::frontend::file_mapper<char> fmapper(".mst");

   auto iter = input.begin();
   auto templ = load_template<boostache::format::stache>(iter, input.end(), fmapper);
   // ------------------------------------------------------------------

   // ------------------------------------------------------------------
   // Apply the compiled template and the data model to the generate
   // method
   std::stringstream stream;
   boostache::generate(stream, templ, invoice);
   // ------------------------------------------------------------------

   std::cout << stream.str();
}
