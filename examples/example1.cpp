/**
 *  \file simple_generate.cpp
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

namespace bstache = boost::boostache;
namespace extn =  bstache::extension;


using item_t = std::map<std::string, std::string>;
using item_list_t = std::vector<item_t>;
using invoice_t = std::map<std::string, item_list_t>;


int main()
{
   // ------------------------------------------------------------------
   // The input template
   std::string input( 
                      "Invoice"
                      "\n"
                      "{{#lines}}"
                      "  {{item_code}}  {{description}}  {{amount}}\n"
                      "{{/lines}}"
      );
   // ------------------------------------------------------------------


   item_list_t invoice_items = { 
                                 { {"item_code"    , "1234"},
                                   {"description"  , "teddy bear"},
                                   {"amount"       , "$23"} },
                                 { {"item_code"    , "1235"},
                                   {"description"  , "computer"},
                                   {"amount"       , "$9"} } 
   };

   invoice_t invoice = {{"lines", invoice_items}};


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
   bstache::generate(stream, templ, invoice);
   // ------------------------------------------------------------------

   std::cout << stream.str();
   return -1;
}

