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
#include <iostream>
#include <strstream>

namespace bstache = boost::boostache;

// ----------------------------------------
// The following specializations allow
// the engine to understand how to handle
// our model::stache_model type.
//
// Extending is simply a matter of supplying
// the render and test methods that can be
// found via ADL.
// ----------------------------------------
namespace boost { namespace boostache { namespace model
{
   struct print_value
   {
      typedef void result_type;
      print_value(std::ostream & s) : stream(s) {}

      template <typename T>
      void operator()(T const &) const
      {}

      void operator()(std::string const & v) const
      {
         stream << v;
      }

      void operator()(model::stache_variant const & v) const
      {
         boost::apply_visitor(*this, v);
      }

   private:
      std::ostream & stream;
   };

   template <typename Stream>
   inline void render(Stream & stream, stache_model const & context, std::string const & name)
   {
      auto iter = context.find(name);
      if(iter != context.end())
      {
         print_value printer(stream);
         printer(iter->second);
      }
   }

   struct test_value
   {
      typedef bool result_type;

      template <typename T>
      bool operator()(T const &) const
      {
         return false;
      }

      bool operator()(std::string const &) const
      {
         return true;
      }

      bool operator()(model::stache_model_vector const & v) const
      {
         return !v.empty();
      }

      bool operator()(model::stache_model const & v) const
      {
         return !v.empty();
      }

      bool operator()(model::stache_bool_function const & v) const
      {
         return v();
      }

      bool operator()(model::stache_variant const & v) const
      {
         return boost::apply_visitor(*this, v);
      }
   };

   bool test(std::string const & name, stache_model const & context)
   {
      auto iter = context.find(name);
      if(iter != context.end())
      {
         test_value tester;
         return tester(iter->second);
      }
      return false;
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

   bstache::model::stache_model data = {{"name","Jeff"},{"whoot","yipee"},{"bar",false_},{"foo",true_}};
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

