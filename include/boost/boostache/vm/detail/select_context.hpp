/**
 *  \file detail/select_context.hpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *  Copyright 2017 Tobias Loew : die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/vm/detail/stacked_context.hpp>
#include <boost/boostache/model/select_traits.hpp>
#include <boost/boostache/detail/unwrap_variant_visitor.hpp>
#include <boost/boostache/model/basic_test_extension.hpp>


namespace boost { namespace boostache { namespace vm { namespace detail
{

	//struct extract_variable_visitor {
	//	template <typename Template>
	//	boost::optional<std::string> operator()(Template const & v) const
	//	{
	//		return boost::none;
	//	}

	//	boost::optional<std::string> operator()(ast::for_each const & v) const
	//	{
	//		return v.name;
	//	}
	//};
	//template <typename Template>
	//boost::optional<std::string> extract_variable(Template const & templ)
	//{
	//	return boost::apply_visitor(extract_variable_visitor(), templ);
	//}




   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack
            , typename CategoryChild
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack
                      , CategoryChild)
   {
      generate(stream, templ, ctx_child, /*ctx_parent,*/ stack);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack
       , extension::associative_attribute)
   {
       generate(stream, templ, ctx_child, stack);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack
       , extension::sequence_attribute)
   {
//       Stack local(ctx_parent, &stack);
       generate(stream, templ, ctx_child, stack);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack
       , extension::variant_attribute)
   {
      boost::apply_visitor(
         boostache::detail::make_unwrap_variant_visitor(
            [&stream,&templ,&ctx_parent,&stack](auto ctx)
            {
               select_context( stream, templ, ctx_parent, ctx, stack
                             , extension::select_category_t<decltype(ctx)>{});
            })
          , ctx_child);
   }


   template <typename Stream, typename Context, typename Stack, typename Category>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Stack const* stack, Category)
   {
      generate(stream, templ.body, ctx, stack);
   }



   template <typename Stream, typename Context, typename Stack>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Stack const* stack
                               , extension::variant_attribute)
   {
      boost::apply_visitor(
         boostache::detail::make_unwrap_variant_visitor(
            [&stream,&templ,&stack](auto ctx)
            {
               select_context_dispatch( stream, templ, ctx, stack
                                      , extension::select_category_t<decltype(ctx)>{});
            })
          , ctx);
   }


   template <typename Stream, typename Context, typename Stack>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Stack const* stack
                               , extension::associative_attribute)
   {
      auto iter = ctx.find(templ.tag);
      if(iter != ctx.end())
      {
         // std::cout << "select_context ctx_parent "
         //           << typeid(ctx).name()
         //           << " ctx_child "
         //           << typeid(iter->second).name()
         //           << " category "
         //           << typeid(extension::select_category_t<decltype(iter->second)>{}).name();

          // store current context for name lookup
          Stack local(ctx, stack);

         select_context( stream, templ.body, ctx, iter->second, &local
                       , extension::select_category_t<decltype(iter->second)>{});
      }
      else
      {
		  //if(templ.make_local)
		  //{
			 // // generate nothing, since we didn't find the base of the variable
		  //}
		  //else
		  //{
		      generate(stream, templ.body, ctx, stack);
		  //}
      }
   }
   // ------------------------------------------------------------------
   // ------------------------------------------------------------------


   template <typename Stream, typename Context, typename Stack>
   void select_context_dispatch(Stream & stream
	   , ast::select_context const & templ
	   , Context const & ctx
       , Stack const* stack
       //   , std::function<void (Stream &, ast::select_context const &)> callback = {}
   )
   {
	   select_context_dispatch(stream, templ, ctx, stack, extension::select_category_t<Context>{});
   }


}}}}


#endif
