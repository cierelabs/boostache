/**
 *  \file ast.hpp
 *
 *  Copyright 2015 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_DJANGO_AST_HPP
#define BOOST_BOOSTACHE_FRONT_END_DJANGO_AST_HPP

#include <boost/spirit/include/support_extended_variant.hpp>

#include <string>
#include <vector>

namespace boost { namespace boostache { namespace frontend { namespace django { namespace ast
{
   struct node;

   struct undefined {};

   struct comment {};

   struct identifier : std::string
   {};

   struct literal_text : std::string
   {};

   struct variable : std::vector<identifier>
   {};

   struct node : boost::spirit::extended_variant<
        undefined
      , comment
      , literal_text
      , variable
      >
   {
      node() : base_type() {}
      node(comment const & rhs) : base_type(rhs) {}
      node(literal_text const & rhs) : base_type(rhs) {}
      node(variable const & rhs) : base_type(rhs) {}
   };

   struct node_list : std::vector<node> {};

   struct root : node_list {};

}}}}}

#endif
