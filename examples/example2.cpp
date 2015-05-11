/**
 *  \file example2.cpp
 *
 *  A slightly more complex example.
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


// -------------------------------------------------------
// The data will be an invoice this time. The invoice
// consists of a list of line items. Each line item
// can be describes as map of string to strings.
//
using item_t = std::map<std::string, std::string>;
using item_list_t = std::vector<item_t>;
using invoice_t = std::map<std::string, item_list_t>;
// -------------------------------------------------------


int main()
{
   // ------------------------------------------------------------------
   // The template describing an invoice.
   std::string input( 
                      "Invoice"
                      "\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description. invoice_items is a list of maps that
   // describe each item.
   item_list_t invoice_items = { 
                                 { {"item_code"    , "1234"},
                                   {"description"  , "teddy bear"},
                                   {"amount"       , "$23"} },
                                 { {"item_code"    , "1235"},
                                   {"description"  , "computer"},
                                   {"amount"       , "$9"} } 
   };

   // we need to put the list into a map so that tag 'lines' can
   // be used to look up the items.
   invoice_t invoice = {{"lines", invoice_items}};
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
   bstache::generate(stream, templ, invoice);
   // ------------------------------------------------------------------

   std::cout << stream.str();
}
