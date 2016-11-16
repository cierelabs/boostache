/**
 *  \file example4.cpp
 *
 *  A slightly more complex example.
 *
 *  This time we write an extension that understands how to render
 *  a user data type (the invoice_item struct).
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

struct invoice_item
{
   std::string item_code;
   std::string description;
   std::string amount;
};

using item_list_t = std::vector<invoice_item>;
using invoice_t = std::map<std::string, item_list_t>;
// -------------------------------------------------------


namespace boost { namespace boostache { namespace extension
{
   // create a user extension for our invoice_item data type
   // We need to handle how to render as well as test for
   // truthfulness
   //
   template< typename Stream >
   void render( Stream & stream, invoice_item const & context
              , std::string const & name, plain_attribute)
   {
      if(name == "item_code")
      {
         stream << context.item_code;
      }
      else if(name == "description")
      {
         stream << context.description;
      }
      else if(name == "amount")
      {
         stream << context.amount;
      }
   }   

   bool test( invoice_item const & context
            , plain_attribute)
   {
      return true;
   }
}}}


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
                                 {"1234", "teddy bear", "$23"},
                                 {"1235", "computer"  , "$9" } 
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
