/**
 *  \file stache_ast.hpp
 * 
 *
 *  Copyright 2014 Michael Caisse
 *
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_CPPTE_FRONT_END_STACHE_AST_HPP
#define BOOST_CPPTE_FRONT_END_STACHE_AST_HPP

#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>

namespace boost { namespace cppte { namespace front_end { namespace ast
{
   struct stache_root;

   struct undefined {};

   struct symbol : std::string
   {};

   struct variable
   {
      bool is_escaped;
      std::string value;
   };

   struct section
   {
      bool is_inverted;
      stache_root node;
   };

   struct stache_root : boost::spirit::extended_variant<
        undefined
      , symbol
      , variable
      , section
      >
   {
      stache_root() : base_type() {}
      stache_root(symbol const & rhs) : base_type(rhs) {}
      stache_root(variable const & rhs) : base_type(rhs) {}
      stache_root(section const & rhs) : base_type(rhs) {}
   };
}}}}

#endif

