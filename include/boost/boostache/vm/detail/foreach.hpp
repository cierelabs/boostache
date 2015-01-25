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
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <map>
#include <vector>
#include <string>


namespace boost { namespace boostache { namespace vm
{
   template <typename Stream, typename Template, typename Context>
   void generate( Stream & stream
                , Template const & templ
                , Context const & context);
}}}

namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream, typename Node, typename Context>
   void foreach(Stream & stream, Node const & node, Context const & context);
}}}}

///// TESTING HACK!!!!
// #include <iostream>
// template <typename Stream, typename Template, typename Context>
// struct generate_specialization
// {
//    static void apply( Stream & stream
//                     , Template const & templ
//                     , Context const & context )
//    {
//       stream << context;
//    }
// };

// template <typename Stream, typename Template, typename Context>
// void generate(Stream & stream, Template const & templ, Context const & context)
// {
//    generate_specialization<Stream,Template,Context>::apply(stream,templ,context);
// }
/////////////////////////////////


namespace boost { namespace boostache
{
   template < typename T
            , typename Enable = void>
   struct supports_foreach : boost::mpl::identity<boost::mpl::false_>
   {};
}}

namespace boost { namespace boostache { namespace extension
{
   namespace detail
   {
      template <typename Stream, typename Node>
      struct unwrap_variant_foreach
      {
         typedef void result_type;

         unwrap_variant_foreach(Stream & stream, Node const & node) 
            : stream_(stream), node_(node)
         {}

         template <typename T>
         void operator()(T const & context) const
         {
            vm::detail::foreach(stream_,node_,context);
         }

         Stream & stream_;
         Node const & node_;
      };
   }

   // -------------------------------------------
   // foreach category
   template <typename T, typename Enable = void>
   struct foreach_category
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct foreach_category<T,
                           typename enable_if<vm::trait::is_variant<T>>::type>
      : mpl::identity<variant_attribute> {};

   template <>
   struct foreach_category<std::string>
      : mpl::identity<plain_attribute> {};

   template <typename T>
   struct foreach_category< T
                          , typename std::enable_if<
                               vm::trait::has_begin<
                                  typename vm::trait::not_a_map<T>::type
                                  >::value
                               >::type
                            >
      : mpl::identity<container_attribute>
   {};

   // template <typename T>
   // struct foreach_category<boost::optional<T>>
   //    : mpl::identity<optional_attribute> {};

   template <typename T>
   struct foreach_category<std::map<std::string,T>>
      : mpl::identity<associative_attribute> {};

}}}


namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , extension::container_attribute)
   {
      for(auto const & item : context)
      {
         generate(stream,node.value,item);
      }
   }

   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , extension::associative_attribute)
   {
      auto iter = context.find(node.name);
      if(iter != context.end())
      {
         foreach(stream,node,iter->second);
      }
      else
      {
         vm::generate(stream,node.value,context);
      }
   }

   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , extension::plain_attribute)
   {
      vm::generate(stream,node.value,context);
   }

   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , extension::variant_attribute)
   {
      extension::detail::unwrap_variant_foreach<Stream,Node> variant_foreach(stream,node);
      return boost::apply_visitor(variant_foreach, context);
   }

   /**
    *  Entry point for foreach is here.
    */
   template <typename Stream, typename Node, typename Context>
   void foreach(Stream & stream, Node const & node, Context const & context)
   {
      foreach( stream
             , node
             , context
             , typename extension::foreach_category<Context>::type{});
   }
   
}}}}


#endif
