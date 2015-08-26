/**
 *  \file traits.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_TRAITS_HPP
#define BOOST_BOOSTACHE_VM_TRAITS_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/variant/variant.hpp>
#include <map>


namespace boost { namespace boostache { namespace vm { namespace trait
{
    namespace detail
    {
       // this trait is enabled for the Spirit extended variant
       BOOST_MPL_HAS_XXX_TRAIT_DEF(adapted_variant_tag)
    }

   template <typename T>
   struct is_variant
      : detail::has_adapted_variant_tag<T>
   {};

   template <typename T0, typename... TN>
   struct is_variant< boost::variant<T0,TN...> >
      : mpl::true_
   {};

   using std::begin;
   template< class T
           , class = decltype(begin(std::declval<T>()))
           >
   std::true_type  has_begin_test(const T&);

   std::false_type has_begin_test(...);

   template<class T>
   using has_begin = decltype(has_begin_test(std::declval<T>()));


   template<class T>
   struct not_a_map
   {
      using type = T;
   };

   template<class T1,class T2>
   struct not_a_map<std::map<T1,T2>>
   {};

}}}}

#endif
