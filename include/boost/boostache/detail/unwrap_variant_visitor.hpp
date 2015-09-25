/**
 *  \file detail/unwrap_variant_visitor.hpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_DETAIL_UNWRAP_VARIANT_VISITOR_HPP
#define BOOST_BOOSTACHE_DETAIL_UNWRAP_VARIANT_VISITOR_HPP


namespace boost { namespace boostache { namespace detail
{
   template <typename F, typename R>
   struct unwrap_variant_visitor
   {
      using result_type = R;

      template <typename U>
      unwrap_variant_visitor(U && f)
         : f_(f)
      {}

      template <typename T>
      R operator()(T const & context) const
      {
         return f_(context);
      }

      F f_;
   };

   template <typename F>
   struct unwrap_variant_visitor<F,void>
   {
      using result_type = void;

      template <typename U>
      unwrap_variant_visitor(U && f)
         : f_(f)
      {}

      template <typename T>
      void operator()(T const & context) const
      {
         f_(context);
      }

      F f_;
   };
   

   template <typename R=void, typename F>
   unwrap_variant_visitor<F,R> make_unwrap_variant_visitor(F && f)
   {
      return unwrap_variant_visitor<F,R>{std::forward<F>(f)};
   }
}}}


#endif // BOOST_BOOSTACHE_DETAIL_UNWRAP_VARIANT_VISITOR_HPP
