/**
 *  \file basic_test_extension.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_BASIC_TEST_EXTENSION_HPP
#define BOOST_BOOSTACHE_MODEL_BASIC_TEST_EXTENSION_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/model/test_traits.hpp>
#include <type_traits>


namespace boost { namespace boostache { namespace extension
{

   template <typename T>
   bool test(std::string const & name, T const & context);


   template <typename T>
   bool test( std::string const & name, T const & context
            , unused_attribute)
   {
      return false;
   }

   template <typename T>
   bool test( std::string const & name, T const & context
            , plain_attribute)
   {
      return !!context;
   }

   template <typename T>
   bool test( std::string const & name, T const & context
            , container_attribute)
   {
      return !context.empty();
   }

   template <typename T>
   bool test( std::string const & name, T const & context
            , optional_attribute)
   {
      return !!context;
   }

   template <typename T>
   bool test( std::string const & name, T const & context
            , associative_attribute)
   {
      auto iter = context.find(name);
      if(iter!=context.end())
      {
         return test(name,iter->second);
      }
      else
      {
         return false;
      }
   }

   // --------------------------------------------------------------------------
   // --------------------------------------------------------------------------

   template <typename T>
   bool test(std::string const & name, T const & context)
   {
      return test( name
                 , context
                 , typename test_category<T>::type{});
   }

}}}

#endif
