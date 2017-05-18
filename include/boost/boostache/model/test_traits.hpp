/**
 *  \file test_traits.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_TEST_TRAITS_HPP
#define BOOST_BOOSTACHE_MODEL_TEST_TRAITS_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <type_traits>
#include <map>
#include <vector>


namespace boost { namespace boostache { namespace extension
{
   // -------------------------------------------
   // test category
   template <typename T, typename Enable = void>
   struct test_category
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct test_category< T
                       , vm::trait::enable_if_is_variant_t<T>
                       >
      : mpl::identity<variant_attribute> {};

   template <>
   struct test_category<std::string>
      : mpl::identity<sequence_attribute> {};

   template <typename T>
   struct test_category< T
                       , vm::trait::enable_if_sequence_not_map_t<T>
                       >
      : mpl::identity<sequence_attribute> {};

   // template <typename T>
   // struct test_category<T,
   //                      decltype(T::empty())>
   //    : mpl::identity<sequence_attribute> {};

   template <typename T>
   struct test_category<boost::optional<T>>
      : mpl::identity<optional_attribute> {};

   template <typename T>
   struct test_category<std::map<std::string,T>>
      : mpl::identity<associative_attribute> {};

   template <typename T1, typename T2>
   struct test_category<vm::detail::stacked_context<T1, T2>>
	   : mpl::identity<stacked_context_attribute> {};


   template <typename T>
   using test_category_t = typename test_category<T>::type;
}}}

#endif
