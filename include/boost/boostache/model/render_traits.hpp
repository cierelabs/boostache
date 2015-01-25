/**
 *  \file render_traits.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_RENDER_TRAITS_HPP
#define BOOST_BOOSTACHE_MODEL_RENDER_TRAITS_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <type_traits>
#include <map>


namespace boost { namespace boostache { namespace extension
{
   template <typename T, typename Enable = void>
   struct render_category
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct render_category<T,
                          typename enable_if<vm::trait::is_variant<T>>::type>
      : mpl::identity<variant_attribute> {};

   template <typename T>
   struct render_category<boost::optional<T>>
      : mpl::identity<optional_attribute> {};


   template <typename T>
   struct render_category<std::map<std::string,T>>
      : mpl::identity<associative_attribute> {};


   template <typename T>
   struct render_category< T
                         , typename std::enable_if<
                              vm::trait::has_begin<
                                 typename vm::trait::not_a_map<T>::type 
                                 >::value 
                              >::type
                           >
      : mpl::identity<container_attribute>
   {};

}}}

#endif
