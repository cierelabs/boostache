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
   bool render(Stream & stream, T const & context, std::string const & name);



   template< typename Stream
	   , typename T
	   , typename Enable = typename std::enable_if<!vm::trait::is_variant<T>::value>::type
   >
	   bool render(Stream && stream, T const & context, std::string const & name
		   , plain_attribute) 
   {
	    (std::forward<Stream>(stream) << context);
		return true;
   }


   template< typename Stream
           , typename T
           >
   bool render( Stream && stream, T const & context, std::string const & name
              , optional_attribute)
   {
	   if (context)
	   {
		   render(std::forward<Stream>(stream), *context, name);
	   }
	   return true;
   }


   template< typename Stream
           , typename T
           >
	   bool render( Stream && stream, T const & context, std::string const & name
              , unused_attribute)
   {
	   return true;
   }


   template< typename Stream
           , typename T
           >
	   bool render( Stream && stream, T const & context, std::string const & name
              , associative_attribute)
   {
      auto iter = context.find(name);
      if(iter!=context.end())
      {
         render(std::forward<Stream>(stream),iter->second,name);
		 return true;
	  }
	  else
	  {
		  return false;
	  }
   }


   template< typename Stream
	   , typename T
   >
	   bool render(Stream && stream, T const & context, std::string const & name
		   , sequence_attribute)
   {
	   for (auto const & item : context)
	   {
		   render(std::forward<Stream>(stream), item, name);
	   }
	   return true;
   }


   template< typename Stream
	   , typename T
   >
	   bool render(Stream && stream, T const & context, std::string const & name
		   , stacked_context_attribute)
   {
	   if (!render(std::forward<Stream>(stream), context.child, name))
	   {
		   return render(std::forward<Stream>(stream), context.parent, name);
	   }
	   else
	   {
		   return true;
	   }
   }


   template< typename Stream
	   , typename T
   >
	   bool render(Stream && stream, T const & context, std::string const & name
		   , multi_context_attribute)
   {
	   if (!render(std::forward<Stream>(stream), context.context, name))
	   {
		   return false;
//		   return render(std::forward<Stream>(stream), context.parent, name);
	   }
	   else
	   {
		   return true;
	   }
   }


   // --------------------------------------------------------------------------
   // --------------------------------------------------------------------------

   template< typename Stream, typename T >
   bool render(Stream & stream, T const & context, std::string const & name)
   {
	   return render( stream
            , context
            , name
            , render_category_t<T>{} );
   }

}}}

#endif
