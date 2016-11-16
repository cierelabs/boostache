/**
 *  \file example6.cpp
 *
 *  A slightly more complex example.
 *
 *  This time we have a user type that has been adapted to
 *  a Boost Fusion sequence
 *
 *  Copyright 2016 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS

#include <boost/boostache/boostache.hpp>
#include <boost/boostache/frontend/stache/grammar_def.hpp> // need to work out header only syntax
#include <boost/boostache/stache.hpp>
#include <boost/boostache/model/helper.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/boostache/model/enable_fusion.hpp>


#include <iostream>
#include <sstream>
#include <typeinfo>
#include <type_traits>



namespace boostache = boost::boostache;
namespace fusion = boost::fusion;
namespace fext = boost::fusion::extension;


// -------------------------------------------------------
// The data will be an invoice this time. The invoice
// consists of a list of line items. Each line item
// can be describes with the invoice_item struct
//

struct invoice_item
{
   std::string item_code;
   std::string description;
   int amount;
};

BOOST_FUSION_ADAPT_STRUCT(
   invoice_item,
   (std::string , item_code)
   (std::string , description)
   (int , amount)
)

using item_list_t = std::vector<invoice_item>;
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
                      "  {{item_code}}  {{description}}  ${{amount}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data description. invoice_items is a list of maps that
   // describe each item.
   item_list_t invoice_items = { 
                                 {"1234", "teddy bear", 23},
                                 {"1235", "computer"  , 9 } 
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
   boostache::generate(stream, templ, invoice);
   // ------------------------------------------------------------------

   std::cout << stream.str();
}
