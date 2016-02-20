/**
 *  \file detail/foreach.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_FOREACH_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_FOREACH_HPP

#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/model/category.hpp>
#include <boost/boostache/detail/unwrap_variant_visitor.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/optional.hpp>
#include <map>
#include <vector>
#include <string>


namespace boost { namespace boostache { namespace vm
{
   template <typename Stream, typename Template, typename Context>
   void generate( Stream & stream
                , Template const & templ
                , Context const & context);

   namespace detail
   {
      template <typename Stream, typename Node, typename Context>
      void foreach(Stream & stream, Node const & node, Context const & context);
   }
}}}


namespace boost { namespace boostache
{
   template < typename T
            , typename Enable = void>
   struct supports_foreach : boost::mpl::identity<boost::mpl::false_>
   {};
}}


namespace boost { namespace boostache { namespace extension
{
   // -------------------------------------------
   // foreach category
   template <typename T, typename Enable = void>
   struct foreach_category
      : mpl::identity<plain_attribute> {};

   template <>
   struct foreach_category<std::string>
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct foreach_category< T
                          , vm::trait::enable_if_is_variant_t<T>
                          >
      : mpl::identity<variant_attribute> {};

   template <typename T>
   struct foreach_category< T
                          , vm::trait::enable_if_sequence_not_map_t<T>
                          >
      : mpl::identity<sequence_attribute> {};

   template <typename T>
   struct foreach_category<boost::optional<T>>
      : mpl::identity<optional_attribute> {};

   template <typename T>
   struct foreach_category<std::map<std::string,T>>
      : mpl::identity<associative_attribute> {};

   template <typename T>
   using foreach_category_t = typename foreach_category<T>::type;

}}}


namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream, typename Node, typename Context, typename Category>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , Category)
   {
      generate(stream, node.value, context);
   }
#ifdef BOOSTACHE_USE_CPP11
   template<class Stream,class Node>
   struct Cpp11GenericLamdaSimulation_foreach
   {
       Stream& stream;
       const Node& node;
       Cpp11GenericLamdaSimulation_foreach(Stream& stream,const Node& node):stream(stream),node(node){}
       template<class T>
       void operator()(const T& ctx)const{
           vm::detail::foreach(stream, node, ctx);
       }
   };
#endif

   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , extension::variant_attribute)
   {
      boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor(
                          #ifdef BOOSTACHE_USE_CPP11
                               Cpp11GenericLamdaSimulation_foreach<Stream,Node>(stream,node)
                          #else
                                [&stream,&node](auto ctx)
                               {
                                  vm::detail::foreach(stream, node, ctx);
                               }
                          #endif
                            )
                          , context);
   }


   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , extension::sequence_attribute)
   {
      for(auto const & item : context)
      {
         generate(stream, node.value, item);
      }
   }


   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & ctx
               , extension::optional_attribute)
   {
      if(ctx)
      {
         foreach( stream, node, *ctx
                , extension::foreach_category_t<decltype(*ctx)>{});
      }
      else
      {
         generate(stream, node.value, ctx);
      }
   }


   /**
    *  Entry point for foreach
    */
   template <typename Stream, typename Node, typename Context>
   void foreach(Stream & stream, Node const & node, Context const & context)
   {
      using boostache::vm::detail::foreach;
      foreach( stream
             , node
             , context
             , extension::foreach_category_t<Context>{});
   }
   
}}}}


#endif
