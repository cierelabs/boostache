/**
 *  \file stache_model.hpp
 *
 *  Copyright 2014 Kevin Harris
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#pragma once

#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <initializer_list>

namespace boost { namespace boostache { namespace model
{
   typedef std::function<std::string()> stache_string_function;
   typedef std::function<bool()> stache_bool_function;

   struct stache_variant : boost::spirit::extended_variant<
        std::string
      , stache_string_function
      , stache_bool_function
      , boost::recursive_wrapper<std::vector<stache_variant> >
      , boost::recursive_wrapper<std::map<std::string, stache_variant> >
      >
   {
      stache_variant() : base_type() {}
      stache_variant(std::string const & rhs) : base_type(rhs) {}
      stache_variant(const char * rhs) : base_type(std::string{rhs}) {}
      stache_variant(stache_string_function const & rhs) : base_type(rhs) {}
      stache_variant(stache_bool_function const & rhs) : base_type(rhs) {}
      stache_variant(std::vector<stache_variant> const & rhs) : base_type(rhs) {}
      stache_variant(std::map<std::string, stache_variant> const & rhs) : base_type(rhs) {}

      template <typename T>
      stache_variant(std::initializer_list<T> l) : base_type(l) {}

      template <typename T>
      stache_variant & operator=(T const & rhs)
      {
         base_type::operator=(rhs);
         return *this;
      }
   };

   typedef std::vector<stache_variant> stache_model_vector;
   typedef std::map<std::string, stache_variant> stache_model;

}}}
