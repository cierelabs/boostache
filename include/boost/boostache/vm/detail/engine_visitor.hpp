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

////////// some test hackery ////////////////////////

namespace boost { namespace boostache { namespace model { namespace customization
{
   template <typename Stream, typename Object>
   void render(Stream & stream, Object const & v)
   {
      stream << v;
   }


   // template <typename Stream, typename Context>
   // void render(Stream & stream, Context const & v, std::string const & name);
   // {
   //    stream << "name{" << name << "} : " << v;
   // }

   template <typename Context>
   bool test(std::string const & name, Context const & c);
}}}}

/////////////////////////////////////////////////////


namespace boost { namespace boostache { namespace vm { namespace detail
{
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
         using boost::boostache::model::customization::render;
         render(stream,v.value);
      }

      void operator()(ast::variable const & v) const
      {}

      void operator()(ast::render const & v) const
      {
         using boost::boostache::model::customization::render;
         render(stream,context,v.name);
      }

      void operator()(ast::for_each const & v) const
      {
      }

      void operator()(ast::if_then_else const & v) const
      {
         using boost::boostache::model::customization::test;

         if(test(v.condition_.name,context))
         {
            boost::apply_visitor(*this, v.then_);
         }
         else
         {
            boost::apply_visitor(*this, v.else_);
         }
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

}}}}


#endif
