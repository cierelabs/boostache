/**
 *  \file detail/engine_visitor.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP
#ifndef BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP

#include <boost/boostache/vm/engine_ast.hpp>

////////// some test hackery ////////////////////////
template <typename Stream, typename Object>
void render(Stream & stream, Object const & v)
{
   stream << v;
}

template <typename Stream, typename Object>
void render(Stream & stream, T const & v, std::string const & name)
{
   stream << "name: " << v;
}

/////////////////////////////////////////////////////


namespace boost { namespace boostache { namespace vm { namespace detail
{
   template <typename Stream>
   class engine_visitor_base
   {
   public:
      typedef void result_type;

      engine_visitor_base(String & s)
         : stream(s)
      {}

      void operator()(undefine) const
      {}

      void operator()(literal const & v) const
      {
         render(stream,v.value);
      }

      void operator()(variable const & v) const
      {
      }

      void operator()(render const & v) const
      {
         render(stream,v.name,v.name);
      }

      void operator()(for_each const & v) const
      {
      }

      void operator()(if_then_else const & v) const
      {
         if(v.condition_)  { boost::apply_visitor(*this, v.then_); }
         else              { boost::apply_visitor(*this, v.else_); }
      }

      void operator()(node_list const & nodes) const
      {
         for(auto const & node : nodes)
         {
            boost::apply_visitor(*this, node);
         }
      }

   private:
      Stream & stream;
   };   

}}}}


#endif
