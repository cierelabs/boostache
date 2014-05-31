/**
 *  \file load_test.cpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
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
#include <strstream>

namespace bstache = boost::boostache;
namespace extn =  bstache::extension;



namespace boost { namespace boostache { namespace extension
{
   template <typename T>
   struct render_category<std::vector<T>> 
      : mpl::identity<unused_attribute>
   {};

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




int main()
{
   // ------------------------------------------------------------------
   // The input template
   std::string input( "Hello world \n"
                      "{{name}} is here.\n"
                      "{{& escaped_name}} is here\n"
                      "{{#foo}}\n"
                      "Some cool section {{whoot}} is here.\n"
                      "{{^bar}}\n"
                      "Some cool empty section {{whoot}} is here.\n"
                      "{{/bar}} done.\n"
                      "{{/foo}} done.\n"
      );
   // ------------------------------------------------------------------


   // ------------------------------------------------------------------
   // The data model definition
   std::function<bool()> false_ = [](){return false;};
   std::function<bool()> true_  = [](){return true;};

   //bstache::model::stache_model data = {{"name","Jeff"},{"whoot","yipee"},{"bar",false_},{"foo",true_}};
   bstache::model::stache_model data = {
      {"name","Jeff"},
      {"whoot","yipee"},
      {"bar",false_},
      {"foo",{
            {{"whoot","yipee 1"}},
            {{"whoot","yipee 2"}},
            {{"whoot","yipee 3"}},
            {{"whoot","yipee 4"}},
         }
      }
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
   std::ostringstream stream;
   bstache::generate(stream, templ, data);
   // ------------------------------------------------------------------

   std::cout << stream.str();

   return -1;
}

