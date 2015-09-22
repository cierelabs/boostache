/**
 *  \file basic_render_extension.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_MODEL_BASIC_RENDER_EXTENSION_HPP
#define BOOST_BOOSTACHE_MODEL_BASIC_RENDER_EXTENSION_HPP

#include <boost/optional.hpp>
#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/model/render_traits.hpp>
#include <type_traits>


namespace boost { namespace boostache { namespace extension
{
   template< typename Stream, typename T >
   void render(Stream & stream, T const & context, std::string const & name);



   template< typename Stream
           , typename T
           , typename Enable = typename std::enable_if<!vm::trait::is_variant<T>::value>::type
           >
   auto render( Stream && stream, T const & context, std::string const & name
              , plain_attribute) -> decltype(std::forward<Stream>(stream)<<context)
   {
      return (std::forward<Stream>(stream) << context);
   }


   template< typename Stream
           , typename T
           >
   void render( Stream && stream, T const & context, std::string const & name
              , optional_attribute)
   {
      render(std::forward<Stream>(stream),*context,name);
   }


   template< typename Stream
           , typename T
           >
   void render( Stream && stream, T const & context, std::string const & name
              , unused_attribute)
   {
   }


   template< typename Stream
           , typename T
           >
   void render( Stream && stream, T const & context, std::string const & name
              , associative_attribute)
   {
      auto iter = context.find(name);
      if(iter!=context.end())
      {
         render(std::forward<Stream>(stream),iter->second,name);
      }
   }


   template< typename Stream
           , typename T
           >
   void render( Stream && stream, T const & context, std::string const & name
              , sequence_attribute)
   {
      for(auto const & item : context)
      {
         render(std::forward<Stream>(stream),item,name);
      }
   }


   // --------------------------------------------------------------------------
   // --------------------------------------------------------------------------

   template< typename Stream, typename T >
   void render(Stream & stream, T const & context, std::string const & name)
   {
      render( stream
            , context
            , name
            , typename render_category<T>::type{} );
   }

}}}

#endif
