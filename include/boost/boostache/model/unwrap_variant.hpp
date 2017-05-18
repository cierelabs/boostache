/**
 *  \file unwrap_variant.hpp
 *
 *  When the model node is a variant, we need to unwrap it and dispatch
 *  to the contained data type. This is a helper to do that for us.
 *
 *  Copyright 2014 Michael Caisse : ciere.com
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
      return boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor<bool>(
                                      [&tag](auto ctx)
                                      {
                                         return test(ctx, tag);
                                      }
                                   )
                                 , context);
   }


   template <typename T>
   bool test( T const & context
            , variant_attribute)
   {
      return boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor<bool>(
                                      [](auto ctx)
                                      {
                                         return test(ctx);
                                      }
                                   )
                                 , context);
   }


   template< typename Stream, typename T >
   bool render( Stream & stream, T const & context, std::string const & name
              , variant_attribute)
   {
      boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor(
                                      [&stream,&name](auto ctx)
                                      {
                                         render(stream,ctx,name);
                                      }
                                   )
                                 , context);

	  return true;
   }

}}}

#endif
