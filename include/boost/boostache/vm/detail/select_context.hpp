/**
 *  \file detail/select_context.hpp
 *
 *  Copyright 2015 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_SELECT_CONTEXT_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/model/select_traits.hpp>
#include <boost/boostache/detail/unwrap_variant_visitor.hpp>


namespace boost { namespace boostache { namespace vm { namespace detail
{
   template < typename Stream, typename Template
            , typename Context1, typename Context2
            , typename CategoryChild
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & ctx_parent
                      , Context2 const & /*ctx_child*/
                      , CategoryChild)
   {
      generate(stream, templ, ctx_parent);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & /*ctx_parent*/
                      , Context2 const & ctx_child
                      , extension::associative_attribute)
   {
      generate(stream, templ, ctx_child);
   }


   template < typename Stream, typename Template
            , typename Context1, typename Context2
            >
   void select_context( Stream & stream, Template const & templ
                      , Context1 const & /*ctx_parent*/
                      , Context2 const & ctx_child
                      , extension::sequence_attribute)
   {
      generate(stream, templ, ctx_child);
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
         generate(stream, templ.body, ctx);
      }
   }
   // ------------------------------------------------------------------
   // ------------------------------------------------------------------

}}}}


#endif
