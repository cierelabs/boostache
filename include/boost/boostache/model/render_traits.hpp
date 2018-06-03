/**
 *  \file render_traits.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_RENDER_TRAITS_HPP
#define BOOST_BOOSTACHE_MODEL_RENDER_TRAITS_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/vm/detail/multi_context.hpp>
#include <boost/boostache/model/category.hpp>
#include <type_traits>
#include <map>


namespace boost { namespace boostache { namespace extension
{
   template <typename T, typename Enable = void>
   struct render_category
      : mpl::identity<plain_attribute> {};

   template <>
   struct render_category<std::string>
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct render_category< T
                         , vm::trait::enable_if_is_variant_t<T> >
      : mpl::identity<variant_attribute> {};

   template <typename T>
   struct render_category<boost::optional<T>>
      : mpl::identity<optional_attribute> {};


   template <typename T>
   struct render_category<std::map<std::string,T>>
      : mpl::identity<associative_attribute> {};

   template <typename T>
   struct render_category< T
                         , vm::trait::enable_if_sequence_not_map_t<T>
                         >
      : mpl::identity<sequence_attribute> {};

   template <typename T>
   using render_category_t = typename render_category<T>::type;

}}}

#endif
