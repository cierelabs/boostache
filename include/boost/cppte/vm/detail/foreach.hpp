/**
 *  \file detail/foreach.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_VM_DETAIL_FOREACH_HPP
#define BOOST_CPPTE_VM_DETAIL_FOREACH_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>


///// TESTING HACK!!!!
#include <iostream>
template <typename Stream, typename Template, typename Context>
struct generate_specialization
{
   static void apply( Stream & stream
                    , Template const & templ
                    , Context const & context )
   {
      stream << context;
   }
};

template <typename Stream, typename Template, typename Context>
void generate(Stream & stream, Template const & templ, Context const & context)
{
   generate_specialization<Stream,Template,Context>::apply(stream,templ,context);
}
/////////////////////////////////


namespace boost { namespace cppte
{
   template <typename T>
   struct supports_foreach : boost::mpl::identity<boost::mpl::false_>
   {};
}}


namespace boost { namespace cppte { namespace vm { namespace detail
{
   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , boost::mpl::true_)
   {
      for(auto const & next_context : context)
      {
         generate(stream,node,next_context);
      }
   }

   template <typename Stream, typename Node, typename Context>
   void foreach( Stream & stream
               , Node const & node
               , Context const & context
               , boost::mpl::false_)
   {
      generate(stream,node,context);
   }

   /**
    *  Entry point for foreach is here.
    */
   template <typename Stream, typename Node, typename Context>
   void foreach(Stream & stream, Node const & node, Context const & context)
   {
      foreach(stream, node, context, typename supports_foreach<Context>::type{});
   }
   
}}}}


#endif
