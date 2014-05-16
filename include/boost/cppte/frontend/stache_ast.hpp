/**
 *  \file stache_ast.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Jeroen Habraken
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_AST_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_AST_HPP

#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <vector>

namespace boost { namespace cppte { namespace front_end { namespace ast
{
   struct stache_node;

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

   struct section;

   struct partial : identifier
   {};

   struct stache_node : boost::spirit::extended_variant<
        undefined
      , comment
      , literal_text
      , variable
      , boost::recursive_wrapper<section>
      , partial
      >
   {
      stache_node() : base_type() {}
      stache_node(comment const & rhs) : base_type(rhs) {}
      stache_node(literal_text const & rhs) : base_type(rhs) {}
      stache_node(variable const & rhs) : base_type(rhs) {}
      stache_node(section const & rhs) : base_type(rhs) {}
      stache_node(partial const & rhs) : base_type(rhs) {}
   };

   struct section
   {
      bool is_inverted;
      identifier name;
      std::vector<stache_node> nodes;
   };

   struct stache_root : std::vector<stache_node>
   {};

}}}}

#endif

