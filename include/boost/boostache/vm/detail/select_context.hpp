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

	struct extract_variable_visitor {
		template <typename Template>
		boost::optional<std::string> operator()(Template const & v) const
		{
			return boost::none;
		}

		boost::optional<std::string> operator()(ast::for_each const & v) const
		{
			return v.name;
		}
	};
	template <typename Template>
	boost::optional<std::string> extract_variable(Template const & templ)
	{
		return boost::apply_visitor(extract_variable_visitor(), templ);
	}




   template < typename Stream, typename Template
            , typename Context1, typename Context2
            , typename CategoryChild
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
                      , CategoryChild)
   {
      generate(stream, templ, make_stacked_context(ctx_parent, ctx_child, extract_variable(templ)));
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
                      , extension::associative_attribute)
   {
      generate(stream, templ, make_stacked_context(ctx_parent, ctx_child, extract_variable(templ)));
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
                      , extension::sequence_attribute)
   {
      generate(stream, templ, make_stacked_context(ctx_parent, ctx_child, extract_variable(templ)));
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
                      , extension::variant_attribute)
   {
      boost::apply_visitor(
         boostache::detail::make_unwrap_variant_visitor(
            [&stream,&templ,&ctx_parent](auto ctx)
            {
               select_context( stream, templ, ctx_parent, ctx
                             , extension::select_category_t<decltype(ctx)>{});
            })
          , ctx_child);
   }


   template <typename Stream, typename Context, typename Category>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Category)
   {
      generate(stream, templ.body, ctx);
   }


   template <typename Stream, typename Context>
   void select_context_dispatch(Stream & stream
	   , ast::select_context const & templ
	   , Context const & ctx
	   , extension::stacked_context_attribute)
   {
	   if(ctx.bound_variable && *ctx.bound_variable == templ.tag)
	   {
		   // select local variable
		   generate(stream, templ.body, ctx.child);
	   }
	   else if(test(ctx.child, templ.tag, extension::optional_test_tag{}))
	   {
		   if(templ.make_local)
		   {
			   generate(stream, templ.body, ctx.child);
		   }
		   else
		   {
			   generate(stream, templ.body, ctx);
		   }
	   }
	   else
	   {
		   select_context_dispatch(stream, templ, ctx.parent);
		   //std::function<void (Stream &, ast::select_context const &)> callback = [&ctx](Stream & stream, ast::select_context const & templ)
		   //{
			  // select_context_dispatch(stream, templ, ctx);
		   //};
		   //select_context_dispatch(stream, templ, make_multi_context(ctx.parent, callback));
	   }
   }

   template <typename Stream, typename Context>
   void select_context_dispatch(Stream & stream
	   , ast::select_context const & templ
	   , Context const & ctx
	   , extension::multi_context_attribute)
   {
	   if (test(ctx.context, templ.tag, extension::optional_test_tag{}))
	   {
		   generate(stream, templ.body, ctx);
		   //if (templ.make_local)
		   //{
			  // generate(stream, templ.body, ctx.context);
		   //}
		   //else
		   //{
			  // generate(stream, templ.body, ctx);
		   //}
	   }
	   else
	   {
		   ctx.fun(stream, templ);
	   }
   }

   template <typename Stream, typename Context>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx
                               , extension::variant_attribute)
   {
      boost::apply_visitor(
         boostache::detail::make_unwrap_variant_visitor(
            [&stream,&templ](auto ctx)
            {
               select_context_dispatch( stream, templ, ctx
                                      , extension::select_category_t<decltype(ctx)>{});
            })
          , ctx);
   }


   template <typename Stream, typename Context>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx
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
            
         select_context( stream, templ.body, ctx, iter->second
                       , extension::select_category_t<decltype(iter->second)>{});
      }
      else
      {
		  if(templ.make_local)
		  {
			  // generate nothing, since we didn't find the base of the variable
		  }
		  else
		  {
		      generate(stream, templ.body, ctx);
		  }
      }
   }
   // ------------------------------------------------------------------
   // ------------------------------------------------------------------


   template <typename Stream, typename Context>
   void select_context_dispatch(Stream & stream
	   , ast::select_context const & templ
	   , Context const & ctx
	//   , std::function<void (Stream &, ast::select_context const &)> callback = {}
   )
   {
	   select_context_dispatch(stream, templ, ctx, /*callback, */extension::select_category_t<Context>{});
   }


}}}}


#endif
