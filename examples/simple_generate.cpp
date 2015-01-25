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
#include <boost/boostache/model/stache_model.hpp>
#include <boost/boostache/model/helper.hpp>
#include <iostream>
#include <sstream>

namespace bstache = boost::boostache;
namespace extn =  bstache::extension;



namespace boost { namespace boostache { namespace extension
{
   template <typename T>
   bool test( std::string const & name, std::function<T()> const & context
            , extn::plain_attribute)
   {
      return test(name,context());
   }

   template< typename Stream
           , typename T       >
   void render( Stream & stream, std::function<T()> const & context, std::string const & name
              , extn::plain_attribute)
   {
      render(stream,context(),name);
   }
}}}


// ----------------------------------------
// ----------------------------------------



using smodel = bstache::model::stache_model;
using svector = bstache::model::stache_model_vector;


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
   std::function<bool()> false_ = [](){return false;};
   std::function<bool()> true_  = [](){return true;};

   smodel data = {
      {"name","Jeff"},
      {"escaped_name","<h1>Jeff</h1>"},
      {"whoot","yipee"},
      {"bar",false_},
      {"foo", svector { smodel{{"whoot","yipee 1"}}
                      , smodel{{"whoot","yipee 2"},{"bar",true_}}
                      , smodel{{"whoot","yipee 3"}} }}
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

