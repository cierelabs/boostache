/**
 *  \file detail/select_context.hpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *  Copyright 2017, 2018 Tobias Loew : tobi@die-loews.de
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/model/select_traits.hpp>
#include <boost/boostache/detail/unwrap_variant_visitor.hpp>
#include <boost/boostache/model/basic_test_extension.hpp>


namespace boost { namespace boostache { namespace vm { namespace detail
{


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack, typename Global
            , typename CategoryChild
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack, Global const* global
                      , CategoryChild)
   {
      generate(stream, templ, ctx_child, /*ctx_parent,*/ stack, global);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack, typename Global
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack, Global const* global
       , extension::associative_attribute)
   {
       generate(stream, templ, ctx_child, stack, global);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack, typename Global
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack, Global const* global
       , extension::sequence_attribute)
   {
//       Stack local(ctx_parent, &stack);
       generate(stream, templ, ctx_child, stack, global);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2, typename Stack, typename Global
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & ctx_child
       , Stack const* stack, Global const* global
       , extension::variant_attribute)
   {
      boost::apply_visitor(
         boostache::detail::make_unwrap_variant_visitor(
            [&stream,&templ,&ctx_parent,&stack,&global](auto ctx)
            {
               select_context( stream, templ, ctx_parent, ctx, stack, global
                             , extension::select_category_t<decltype(ctx)>{});
            })
          , ctx_child);
   }





   template <typename Stream, typename Context, typename Stack, typename Global, typename Category>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Stack const* stack, Global const* global, bool global_check, Category)
   {
      generate(stream, templ.body, ctx, stack, global);
   }


   template <typename Stream, typename Context, typename Stack, typename Global>
   void select_context_dispatch(Stream & stream
       , ast::select_context const & templ
       , Context const & ctx, Stack const* stack, Global const* global, bool global_check
       , extension::sequence_attribute)
   {
       size_t pos;
       int n = std::stoi(templ.tag, &pos);
       if (pos == templ.tag.size() && n >= 0 && n < std::size(ctx)) {
           generate(stream, templ.body, ctx[n], stack, global);

       }
   }


   template <typename Stream, typename Context, typename Stack, typename Global>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Stack const* stack, Global const* global, bool global_check
                               , extension::variant_attribute)
   {
      boost::apply_visitor(
         boostache::detail::make_unwrap_variant_visitor(
            [&stream,&templ,&stack, &global, &global_check](auto ctx)
            {
               select_context_dispatch( stream, templ, ctx, stack, global, global_check
                                      , extension::select_category_t<decltype(ctx)>{});
            })
          , ctx);
   }


   template <typename Stream, typename Context, typename Stack, typename Global>
   void select_context_dispatch( Stream & stream
                               , ast::select_context const & templ
                               , Context const & ctx, Stack const* stack, Global const* global, bool global_check
                               , extension::associative_attribute)
   {
       if (!templ.is_context_local)
       {
           auto s = stack;
           while (s) {
               auto&& n = s->name;
               if (n && *n == templ.tag) {
                   select_context(stream, templ.body, ctx, s->current, stack, global, 
                       extension::select_category_t<decltype(s->current)>{});
                   return;
               }
               s = s->parent;
           }
       }






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
          if (templ.push_context) {
              Stack local(ctx, std::string{}, stack);

              select_context(stream, templ.body, ctx, iter->second, &local, global,
                  extension::select_category_t<decltype(iter->second)>{});
          }
          else {
              select_context(stream, templ.body, ctx, iter->second, stack, global,
                  extension::select_category_t<decltype(iter->second)>{});
          }
      }
      else
      {
		  if(templ.is_context_local)
		  {
			  // generate nothing, since we didn't find the base of the variable
              stream << "<< '" << templ.tag << "' NOT FOUND >>";
		  }
		  else
		  {

              if (!global_check) {
                  // get global context and probe
                  select_context_dispatch(stream, templ, global->context, stack, global, true);
              }
              else {
           //       generate(stream, templ.body, ctx, stack, global);
              }
		  }
      }
   }
   // ------------------------------------------------------------------
   // ------------------------------------------------------------------


   template <typename Stream, typename Context, typename Stack, typename Global>
   void select_context_dispatch(Stream & stream
	   , ast::select_context const & templ
	   , Context const & ctx
       , Stack const* stack, Global const* global
       , bool global_check
       //   , std::function<void (Stream &, ast::select_context const &)> callback = {}
   )
   {
	   select_context_dispatch(stream, templ, ctx, stack, global, global_check, extension::select_category_t<Context>{});
   }


}}}}


#endif
