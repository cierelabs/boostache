/**
 *  \file unwrap_variant.hpp
 *
 *  When the model node is a variant, we need to unwrap it and dispatch
 *  to the contained data type. This is a helper to do that for us.
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_UNWRAP_VARIANT_HPP
#define BOOST_BOOSTACHE_MODEL_UNWRAP_VARIANT_HPP

#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/detail/unwrap_variant_visitor.hpp>


namespace boost { namespace boostache { namespace extension
{
   template <typename T>
   boost::optional<bool> test( T const & context, std::string const & tag
            , variant_attribute)
   {
      return boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor<boost::optional<bool>>(
                                      [&tag](auto ctx)
                                      {
                                         return test_with_stack(ctx, tag/*, optional_test_tag{}*/);
                                      }
                                   )
                                 , context);
   }


   template <typename T>
   bool test(T const & context
       , variant_attribute)
   {
       return boost::apply_visitor(boostache::detail::make_unwrap_variant_visitor<bool>(
           [](auto ctx)
       {
           return test(ctx);
       }
           )
           , context);
   }


   template <typename T>
   boost::optional<bool> test_tag(T const & context, std::string const & tag
       , variant_attribute)
   {
       return boost::apply_visitor(boostache::detail::make_unwrap_variant_visitor<boost::optional<bool>>(
           [&tag](auto ctx)
       {
           return test_tag(ctx, tag);
       }
           )
           , context);
   }


   template< typename Stream, typename T>
   void render(Stream & stream, T const & context
       , variant_attribute)
   {
       boost::apply_visitor(boostache::detail::make_unwrap_variant_visitor(
           [&stream](auto ctx)
       {
// don't do the following line: it disables user-defined render-overloading
//           render(stream, ctx, render_category_t<decltype(ctx)>{});
           render(stream, ctx);
       }
           )
           , context);
   }

   template< typename Stream, typename T, typename Stack, typename Global>
   bool render_name(Stream & stream, T const & context, Stack const* stack, Global const* global, std::string const & name
       , variant_attribute)
   {
       return boost::apply_visitor(boostache::detail::make_unwrap_variant_visitor<bool>(
           [&stream, &stack, &global, &name](auto ctx)
       {
           return render_name(stream, ctx, stack, global, name, render_category_t<decltype(ctx)>{});
       }
           )
           , context);
   }

}}}

#endif
