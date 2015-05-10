/**
 *  \file detail/engine_visitor.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/vm/detail/foreach.hpp>
#include <boost/boostache/model/select_traits.hpp>


////////// some test hackery ////////////////////////

namespace boost { namespace boostache { namespace extension
{
   template <typename Stream, typename Object>
   void render(Stream & stream, Object const & v)
   {
      stream << v;
   }

   template <typename T>
   bool test(T const & context, std::string const & name);

   template <typename T>
   bool test(T const & context);

   template< typename Stream, typename T >
   void render(Stream & stream, T const & context, std::string const & name);
}}}

/////////////////////////////////////////////////////


namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream, typename Context, typename Category>
   void select_context( Stream &, ast::select_context const & v
                      , Context const &, Category);

   template <typename Stream, typename Context>
   void select_context( Stream & stream, ast::select_context const & v
                      , Context const & ctx, extension::associative_attribute);

   template <typename Stream, typename Context>
   class engine_visitor_base
   {
   public:
      typedef void result_type;

      engine_visitor_base(Stream & s, Context const & c)
         : stream(s)
         , context(c)
      {}

      void operator()(ast::undefined) const
      {}

      void operator()(ast::literal const & v) const
      {
         using boost::boostache::extension::render;
         render(stream, v.value);
      }

      void operator()(ast::variable const & v) const
      {}

      void operator()(ast::render const & v) const
      {
         using boost::boostache::extension::render;
         render(stream, context, v.name);
      }

      void operator()(ast::for_each const & v) const
      {
         using boost::boostache::vm::detail::foreach;
         foreach(stream, v, context);
      }

      void operator()(ast::if_then_else const & v) const
      {
         using boost::boostache::extension::test;
         if(test(context, v.condition_.name))
         {
            boost::apply_visitor(*this, v.then_);
         }
         else
         {
            boost::apply_visitor(*this, v.else_);
         }
      }

      void operator()(ast::select_context const & v) const
      {
         select_context( stream, v, context
                       , typename extension::select_category<Context>::type{});
      }

      void operator()(ast::node_list const & nodes) const
      {
         for(auto const & node : nodes.nodes)
         {
            boost::apply_visitor(*this, node);
         }
      }

      void operator()(ast::node const & v) const
      {
         boost::apply_visitor(*this, v);
      }

   private:
      Stream & stream;
      Context const & context;
   };


   template <typename Stream, typename Template, typename Context>
   void generate( Stream & stream
                , Template const & templ
                , Context const & ctx)
   {
      engine_visitor_base<Stream,Context> engine(stream, ctx);
      engine(templ);
   }


   template <typename Stream, typename Context, typename Category>
   void select_context( Stream & stream, ast::select_context const & v
                      , Context const & ctx, Category)
   {
      generate(stream, v.body, ctx);
   }

   template <typename Stream, typename Context>
   void select_context( Stream & stream, ast::select_context const & v
                      , Context const & ctx, extension::associative_attribute)
   {
      auto iter = ctx.find(v.tag);
      if(iter != ctx.end())
      {
         generate(stream, v.body, iter->second);
      }
      else
      {
         generate(stream, v.body, ctx);
      }
   }
}}}}


#endif
