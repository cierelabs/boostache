/**
 *  \file stache_ast.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONT_END_STACHE_AST_HPP
#define BOOST_BOOSTACHE_FRONT_END_STACHE_AST_HPP

#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <vector>

namespace boost { namespace boostache { namespace frontend { namespace stache { namespace ast
{
   struct node;

   struct undefined {};

   struct comment {};

   struct identifier : std::string
   {};

   struct literal_text : std::string
   {};

   struct variable
   {
      bool is_unescaped;
      identifier value;
   };

   struct partial : identifier
   {};

   struct section;

   struct node : boost::spirit::extended_variant<
        undefined
      , comment
      , literal_text
      , variable
      , boost::recursive_wrapper<section>
      , partial
      >
   {
      node() : base_type() {}
      node(comment const & rhs) : base_type(rhs) {}
      node(literal_text const & rhs) : base_type(rhs) {}
      node(variable const & rhs) : base_type(rhs) {}
      node(section const & rhs) : base_type(rhs) {}
      node(partial const & rhs) : base_type(rhs) {}
   };

   struct node_list :  std::vector<node> {};

   struct section
   {
      bool is_inverted;
      identifier name;
      node_list nodes;
   };

   struct root : node_list {};

}}}}}

#endif
