/**
 *  \file select_traits.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_SELECT_TRAITS_HPP
#define BOOST_BOOSTACHE_MODEL_SELECT_TRAITS_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/vm/detail/stacked_context.hpp>
#include <boost/boostache/vm/detail/multi_context.hpp>
#include <type_traits>
#include <map>
#include <vector>


namespace boost { namespace boostache { namespace extension
{
   // -------------------------------------------
   // test category
   template <typename T, typename Enable = void>
   struct select_category
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct select_category< T
                         , vm::trait::enable_if_is_variant_t<T>
                         >
      : mpl::identity<variant_attribute> {};

   template <typename T>
   struct select_category< T
                         , vm::trait::enable_if_sequence_not_map_t<T>
                         >
      : mpl::identity<sequence_attribute> {};

   template <typename T>
   struct select_category<boost::optional<T>>
      : mpl::identity<optional_attribute> {};

   template <typename T>
   struct select_category<std::map<std::string, T>>
	   : mpl::identity<associative_attribute> {};

   template <typename T>
   using select_category_t = typename select_category<T>::type;

}}}

#endif
