/**
 *  \file detail/foreach.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
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
      template <typename Stream, typename Node, typename Context, typename Stack, typename Global>
      void foreach(Stream & stream, Node const & node, Context const & context, Stack const* stack, Global const* global);
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

	template <typename Stream, typename Node, typename Context, typename Stack, typename Global, typename Category>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
       , Stack const* stack, Global const* global
               , Category)
   {
      generate(stream, node.value, context, stack, global);
   }


   template <typename Stream, typename Node, typename Context, typename Stack, typename Global>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
       , Stack const* stack, Global const* global
       , extension::variant_attribute)
   {
       boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor(
                               [&stream,&node,&stack,&global](auto ctx)
                               {
                                  vm::detail::foreach(stream, node, ctx, stack, global);
                               }
                            )
                          , context);
   }


   template <typename Stream, typename Node, typename Context, typename Stack, typename Global>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
       , Stack const* stack, Global const* global
       , extension::sequence_attribute)
   {
       for(auto const & item : context)
      {

           if (node.name) {
               Stack local(item, node.name, stack);

               generate(stream, node.value, item/*context*/, &local, global);
           }
           else {
               generate(stream, node.value, item, stack, global);
           }


      }
   }


   template <typename Stream, typename Node, typename Context, typename Stack, typename Global>
   void foreach( Stream & stream
               , Node const & node
               , Context const & ctx
       , Stack const* stack, Global const* global
       , extension::optional_attribute)
   {
      if(ctx)
      {
          foreach(stream, node, *ctx, stack, global
              , extension::foreach_category_t<decltype(*ctx)>{});
      }
      else
      {
         generate(stream, node.value, ctx, stack, global);
      }
   }



   /**
    *  Entry point for foreach
    */
   template <typename Stream, typename Node, typename Context, typename Stack, typename Global>
   void foreach(Stream & stream, Node const & node, Context const & context, Stack const* stack, Global const* global)
   {
      using boostache::vm::detail::foreach;
      foreach( stream
             , node
             , context
            , stack, global
             , extension::foreach_category_t<Context>{});
   }
   
}}}}


#endif
