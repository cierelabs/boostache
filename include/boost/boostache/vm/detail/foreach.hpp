/**
 *  \file detail/foreach.hpp
 *
 *  Copyright 2014, 2015 Michael Caisse : ciere.com
 *  Copyright 2017 Tobias Loew : die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_FOREACH_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_FOREACH_HPP

#include <boost/boostache/vm/traits.hpp>
#include <boost/boostache/vm/detail/stacked_context.hpp>
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
      template <typename Stream, typename Node, typename Context, typename Stack>
      void foreach(Stream & stream, Node const & node, Context const & context, Stack const* stack);
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

   //// returns the category of the child context !!!
   //template <typename ParentContext, typename ChildContext>
   //struct foreach_category<vm::detail::stacked_context<ParentContext, ChildContext>>
	  //: foreach_category<ChildContext> {};

   template <typename T>
   using foreach_category_t = typename foreach_category<T>::type;

}}}


namespace boost { namespace boostache { namespace vm { namespace detail
{
	//template<typename Context>
	//const Context & extract_child_context(const Context & context)
	//{
	//	return context;
	//}

	//template <typename ParentContext, typename ChildContext>
	//const ChildContext & extract_child_context(const stacked_context<ParentContext, ChildContext> & context)
	//{
	//	return context.child;
	//}

	//template<typename Context>
	//boost::optional<std::string> extract_bound_variable(const Context & context)
	//{
	//	return boost::none;
	//}

	//template <typename ParentContext, typename ChildContext>
	//boost::optional<std::string> extract_bound_variable(const stacked_context<ParentContext, ChildContext> & context)
	//{
	//	return context.bound_variable;
	//}




	template <typename Stream, typename Node, typename Context, typename Stack, typename Category>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
       , Stack const* stack
               , Category)
   {
      generate(stream, node.value, context, stack);
   }


   template <typename Stream, typename Node, typename Context, typename Stack>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
       , Stack const* stack
       , extension::variant_attribute)
   {
       boost::apply_visitor( boostache::detail::make_unwrap_variant_visitor(
                               [&stream,&node,&stack](auto ctx)
                               {
                                  vm::detail::foreach(stream, node, ctx, stack);
                               }
                            )
                          , context);
   }


   template <typename Stream, typename Node, typename Context, typename Stack>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
       , Stack const* stack
       , extension::sequence_attribute)
   {
       for(auto const & item : context)
      {
         generate(stream, node.value, item, stack);
      }
   }


   template <typename Stream, typename Node, typename Context, typename Stack>
   void foreach( Stream & stream
               , Node const & node
               , Context const & ctx
       , Stack const* stack
       , extension::optional_attribute)
   {
      if(ctx)
      {
          foreach(stream, node, *ctx, stack
              , extension::foreach_category_t<decltype(*ctx)>{});
      }
      else
      {
         generate(stream, node.value, ctx, stack);
      }
   }



   /**
    *  Entry point for foreach
    */
   template <typename Stream, typename Node, typename Context, typename Stack>
   void foreach(Stream & stream, Node const & node, Context const & context, Stack const* stack)
   {
      using boostache::vm::detail::foreach;
      foreach( stream
             , node
             , context
            , stack
             , extension::foreach_category_t<Context>{});
   }
   
}}}}


#endif
