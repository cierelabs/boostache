/**
 *  \file traits.hpp
 *
 *  Copyright 2014 - 2016 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_TRAITS_HPP
#define BOOST_BOOSTACHE_VM_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>
#include <boost/variant/variant.hpp>
#include <map>
#include <unordered_map>
#include <type_traits>


namespace boost { namespace boostache { namespace vm { namespace trait
{
   // ----------------------------------------------------------------
   // variant traits
   // ----------------------------------------------------------------
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
      : std::true_type
   {};

   template <typename T>
   using enable_if_is_variant_t = typename std::enable_if<is_variant<T>::value>::type;
   // ----------------------------------------------------------------
   // ----------------------------------------------------------------


   // ----------------------------------------------------------------
   // begin traits
   // ----------------------------------------------------------------
   using std::begin;
   template< class T
           , class = decltype(begin(std::declval<T>()))
           >
   std::true_type  has_begin_test(const T&);

   std::false_type has_begin_test(...);

   template <typename T>
   struct has_begin
   {
      using type = decltype(has_begin_test(std::declval<T>()));
   };

   template<class T>
   using has_begin_t = typename has_begin<T>::type;


   // ----------------------------------------------------------------
   // map traits
   // ----------------------------------------------------------------
   template<class T>
   struct is_map
      : std::false_type
   {};

   template<class T1,class T2>
   struct is_map<std::map<T1,T2>>
      : std::true_type
   {};

   template<class T1,class T2>
   struct is_map<std::unordered_map<T1,T2>>
      : std::true_type
   {};

   template <typename T>
   using is_map_t = typename is_map<T>::type;

   template <typename T>
   using enable_if_sequence_not_map_t =
      typename std::enable_if<
         std::integral_constant< bool
                                 , vm::trait::has_begin_t<T>::value
                                 & !vm::trait::is_map<T>::value >::value
      >::type;
   // ----------------------------------------------------------------
   // ----------------------------------------------------------------

}}}}

#endif
