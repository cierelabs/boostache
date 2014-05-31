/**
 *  \file unwrap_variant.hpp
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


namespace boost { namespace boostache { namespace extension
{

   namespace detail
   {
      struct unwrap_variant_test
      {
         typedef bool result_type;

         unwrap_variant_test(std::string const & n) : name_(n) {}

         template <typename T>
         bool operator()(T const & context) const
         {
            return test(name_, context);
         }

         std::string const & name_;
      };

      template <typename Stream>
      struct unwrap_variant_render
      {
         typedef void result_type;

         unwrap_variant_render(Stream & s, std::string const & n) 
            : stream_(s), name_(n) 
         {}

         template <typename T>
         void operator()(T const & context) const
         {
            render(stream_,context,name_);
         }

         Stream & stream_;
         std::string const & name_;
      };
   }


   template <typename T>
   bool test( std::string const & name, T const & context
            , variant_attribute)
   {
      detail::unwrap_variant_test variant_test(name);
      return boost::apply_visitor(variant_test, context);
   }


   template< typename Stream, typename T >
   void render( Stream & stream, T const & context, std::string const & name
              , variant_attribute)
   {
      detail::unwrap_variant_render<Stream> variant_render(stream,name);
      return boost::apply_visitor(variant_render, context);
   }

}}}

#endif
