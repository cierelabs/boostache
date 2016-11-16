/**
 *  \file enable_fusion.hpp
 *
 *  Copyright 2016 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_ENABLE_FUSION_HPP
#define BOOST_BOOSTACHE_MODEL_ENABLE_FUSION_HPP

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <type_traits>


namespace boost { namespace boostache { namespace extension
{
   namespace fusion_ext = boost::fusion::extension;
   
   template <typename T, typename Enable=void>
   struct is_fusion_adapted_struct
      : std::false_type
   {};

   template <typename T>
   struct is_fusion_adapted_struct< T
                                  , std::enable_if_t<(fusion_ext::struct_size<T>::value > 0)>
                                  >
      : std::true_type
   {};

   template <typename T>
   constexpr bool is_fusion_adapted_struct_v = is_fusion_adapted_struct<T>::value;


   template <typename T>
   struct render_category< T
                         , std::enable_if_t<is_fusion_adapted_struct_v<T>> >
      : mpl::identity<fusion_adapted_attribute> {};


   template <typename T>
   struct test_category< T
                       , std::enable_if_t<is_fusion_adapted_struct_v<T>> >
      : mpl::identity<fusion_adapted_attribute> {};


   template <typename T>
   struct test_category< T
                       , std::enable_if_t<boost::fusion::traits::is_sequence<T>::vaue>
                       >
      : mpl::identity<fusion_attribute> {};



   // --------------------------------------------------------------------
   namespace fusion_detail
   {
      template <int I, typename F, typename S>
      void apply(F && func, S const & s)
      {
         func( fusion_ext::access::struct_member<S,I>::
                  template apply<S const> :: call(s) );
      };

      template <typename S, int I>
      struct find_member_and_apply
      {
         template <typename F>
         static void call(S const & s, std::string const & name, F && func)
         {
            if(fusion_ext::struct_member_name<S,I>::call() == name)
            {
               apply<I>(std::forward<F>(func), s);
            }
            else
            {
               
               return find_member_and_apply<S,I-1>::call(s, name, std::forward<F>(func));
            }
         }
      };

      template <typename S>
      struct find_member_and_apply<S,0>
      {
         template <typename F>
         static void call(S const & s, std::string const & tag, F && func)
         {
            if(fusion_ext::struct_member_name<S,0>::call() == tag)
            {
               apply<0>(std::forward<F>(func), s);
            }
         }
      };


      template <typename Stream, typename S>
      void render_member(Stream & stream, S const & s, std::string const & name)
      {
         find_member_and_apply
            < S
              , fusion_ext::struct_size<S>::value - 1
              >::call( s, name
                     , [&stream](auto v)
                       {
                          stream << v;
                       }
                 );
      }


      template <typename S>
      bool test_member(S const & s, std::string const & name)
      {
         bool return_value;

         find_member_and_apply
            < S
              , fusion_ext::struct_size<S>::value - 1
              >::call( s, name
                       , [&return_value](auto v)
                       {
                          return_value = boostache::extension::test(v);
                       }
                 );

         return return_value;
      }
   }

   template< typename Stream, typename S >
   void render( Stream & stream
              , S const & context
              , std::string const & name
              , fusion_adapted_attribute )
   {
      fusion_detail::render_member(stream, context, name);
   }   


   template< typename S >
   bool test( S const & context
            , std::string const & name
            , fusion_adapted_attribute )
   {
      return fusion_detail::test_member(context, name);
   }

}}}


// ---------------------------------------------------------
// ---------------------------------------------------------

#endif
