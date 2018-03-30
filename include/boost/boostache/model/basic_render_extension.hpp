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





namespace boost {
    namespace boostache {
        namespace extension
        {
            template< typename Stream, typename T>
            void render(Stream & stream, T const & context);



            template< typename Stream
                , typename T
                , typename Enable = typename std::enable_if<!vm::trait::is_variant<T>::value>::type
            >
                void render(Stream && stream, T const & context
                    , plain_attribute)
            {
                (std::forward<Stream>(stream) << context);
            }


            template< typename Stream
                , typename T
            >
                void render(Stream && stream, T const & context
                    , optional_attribute)
            {
                if (context)
                {
                    render(std::forward<Stream>(stream), *context);
                }
            }


            template< typename Stream
                , typename T
            >
                void render(Stream && stream, T const & context
                    , unused_attribute)
            {
            }


            template< typename Stream
                , typename T
            >
                void render(Stream && stream, T const & context
                    , associative_attribute)
            {
                bool not_first = false;
                for (auto const & item : context)
                {
                    if (not_first) {
                        stream << ",";
                    }
                    else {
                        not_first = true;
                    }
                    stream << "[";
                    render(std::forward<Stream>(stream), item.first);
                    stream << ":";
                    render(std::forward<Stream>(stream), item.second);
                    stream << "]";
                }
            }


            template< typename Stream
                , typename T
                
            >
                void render(Stream && stream, T const & context
                    , sequence_attribute)
            {
                bool not_first = false;
                for (auto const & item : context)
                {
                    if (not_first) {
                        stream << ",";
                    }
                    else {
                        not_first = true;
                    }
                    render(std::forward<Stream>(stream), item);
                }
            }



            // --------------------------------------------------------------------------
            // --------------------------------------------------------------------------

            template< typename Stream, typename T>
            void render(Stream & stream, T const & context)
            {
                render(stream
                    , context
                    , render_category_t<T>{});

            }

        }
    }
}



namespace boost { namespace boostache { namespace extension
{
    template< typename Stream, typename T, typename Stack>
    bool render_name(Stream & stream, T const & context, Stack const* stack, std::string const & name);



   template< typename Stream
	   , typename T
       , typename Stack
	   , typename Enable = typename std::enable_if<!vm::trait::is_variant<T>::value>::type
   >
	   bool render_name(Stream && stream, T const & context, Stack const* stack, std::string const & name
		   , plain_attribute) 
   {
	    //(std::forward<Stream>(stream) << context);
		return false;
   }


   template< typename Stream
           , typename T
       , typename Stack
           >
   bool render_name( Stream && stream, T const & context, Stack const* stack, std::string const & name
              , optional_attribute)
   {
	   //if (context)
	   //{
		  // render_name(std::forward<Stream>(stream), *context, stack, name);
	   //}
	   return false;
   }


   template< typename Stream
           , typename T
       , typename Stack
           >
	   bool render_name( Stream && stream, T const & context, Stack const* stack, std::string const & name
              , unused_attribute)
   {
	   return false;
   }


   template< typename Stream
           , typename T
       , typename Stack
           >
	   bool render_name( Stream && stream, T const & context, Stack const* stack, std::string const & name
              , associative_attribute)
   {
      auto iter = context.find(name);
      if(iter!=context.end())
      {
          render(std::forward<Stream>(stream), iter->second);
		 return true;
	  }
	  else
	  {
		  return false;
	  }
   }


   template< typename Stream
	   , typename T
       , typename Stack
   >
	   bool render_name(Stream && stream, T const & context, Stack const* stack, std::string const & name
		   , sequence_attribute)
   {
	   //for (auto const & item : context)
	   //{
		  // render_name(std::forward<Stream>(stream), item, stack, name);
	   //}
	   return false;
   }



   // --------------------------------------------------------------------------
   // --------------------------------------------------------------------------

   template< typename Stream, typename T, typename Stack>
   bool render_name(Stream & stream, T const & context, Stack const* stack, std::string const & name)
   {
       if (render_name(stream
           , context
           , stack
           , name
           , render_category_t<T>{}))
       {
           return true;
       }
 
   
       // ToDo: add condition whether to also check parent contexts

       Stack const* s = stack;
       while (s) {

           if (boost::apply_visitor(boostache::detail::make_unwrap_variant_visitor<bool>(
               [&stream, &s, &name](auto ctx) -> bool
           {
               return render_name(stream
                   , ctx
                   , s->parent
                   , name
                   , render_category_t<decltype(ctx)>{}
               );

           }
               )
               , s->current))
           {
               return true;
           }

           s = s->parent;
       }
       return false;

   
   }

}}}

#endif
