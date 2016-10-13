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
#ifdef BOOSTACHE_USE_CPP11
template<class Tag>
struct Cpp11GenericLamdaSimulation_test_tag
{
    const Tag& tag;
    Cpp11GenericLamdaSimulation_test_tag(const Tag& tag):tag(tag){}
    template<class T>
    bool operator()(const T& ctx)const{
        return test(ctx,tag);
    }
};
#endif
   template <typename T>
   bool test( T const & context, std::string const & tag
            , variant_attribute)
   {
      return boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor<bool>(
                                 #ifdef BOOSTACHE_USE_CPP11
                                   Cpp11GenericLamdaSimulation_test_tag<std::string>(tag)
                                 #else
                                      [&tag](auto ctx)
                                      {
                                          return test(ctx,tag);
                                      }
                                 #endif
                                   )
                                 , context);
   }
#ifdef BOOSTACHE_USE_CPP11
   struct Cpp11GenericLamdaSimulation_test
   {
       template<class T>
       bool operator()(const T& ctx)const{
           return test(ctx);
       }
   };
#endif
   template <typename T>
   bool test( T const & context
            , variant_attribute)
   {
      return boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor<bool>(
                                 #ifdef BOOSTACHE_USE_CPP11
                                         Cpp11GenericLamdaSimulation_test{}
                                 #else
                                       [](auto ctx)
                                    {
                                       return test(ctx);
                                    }
                                 #endif
                                   )
                                 , context);
   }
#ifdef BOOSTACHE_USE_CPP11
   template<class Stream,class Name, class R = void>
   struct Cpp11GenericLamdaSimulation_render_stream_name
   {
       Stream& stream;
       Name& name;
       Cpp11GenericLamdaSimulation_render_stream_name(Stream& stream,Name& name):stream(stream),name(name){}
       template<class T>
       R operator()(const T& ctx)const{
           return render(stream,ctx,name);
       }
   };
#endif

   template< typename Stream, typename T >
   void render( Stream & stream, T const & context, std::string const & name
              , variant_attribute)
   {
       return boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor(
                                 #ifdef BOOSTACHE_USE_CPP11
                                      Cpp11GenericLamdaSimulation_render_stream_name<Stream,std::string const>(stream,name)
                                 #else
                                   [&stream,&name](auto ctx)
                                   {
                                      render(stream,ctx,name);
                                   }
                                 #endif
                                   )
                                 , context);
   }

}}}

#endif
